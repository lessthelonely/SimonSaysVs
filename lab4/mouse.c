#include <lcom/lcf.h>
#include <stdint.h>

#include "mouse.h"
#include "i8042.h"

int hook_id_mouse;//variable whose value we get in mouse_subscribe_int and use in mouse_unsubscribe_int, hence why we declare it here

uint8_t sc_byte;

bool gesture = false;

//Modified version of sys_inb
int(sys_inb_mouse)(port_t port, uint8_t *value) {
  if(value==NULL){
    return 1;
  }
  uint32_t byte; //need to create a variable uint32_t to call sys_inb
  sys_inb(port,&byte);
  *value=(uint8_t)byte;
  return 0;
}

int (kbc_read_status)(uint8_t *st){
  if(sys_inb_mouse(KBC_ST_REG,st)==1){
    return 1;
  }
  return 0;
}

int (kbc_read_buf)(uint8_t *sc_byte){
   if(sys_inb_mouse(KBC_OUT_BUF,sc_byte)==1){
    return 1;
  }
  return 0;
}

bool (kbc_check_status)(){
  uint8_t st;
  if(kbc_read_status(&st)==1){
    return false;
  }
  if(OBF&st){
    if((st&(PAR|TIMEOUT))==1){
      return false;
    }
    else{
      return true;
    }
  }
  return false;
}

int (mouse_subscribe_int)(uint8_t *bit_no) {
  hook_id_mouse=(int)*bit_no; //Convert to right type in order to use irqsetpolicy (third argument is int)
  if(sys_irqsetpolicy(MOUSE_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&hook_id_mouse)==1){
      return 1;
  }
  return 0;
}

int (mouse_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id_mouse)==1){
  return 1;
  }
  return 0;
}

void (mouse_ih)(){
  if(kbc_check_out_buf()){
   kbc_read_buf(&sc_byte);
}
}

void(mouse_parse_packet)(struct packet *pp, uint8_t bytes[], uint8_t size) {
  //checking bit of the first byte for button clicks
  pp->rb = (bytes[0] & PK_RB);
  pp->mb = (bytes[0] & PK_MB);
  pp->lb = (bytes[0] & PK_LB);

//processing the distance and it's sign
  int16_t dx = bytes[1], dy = bytes[2];

  if ((bytes[0] & PK_XSIGN) != 0)
    dx = dx - 256;
  if ((bytes[0] & PK_YSIGN) != 0)
    dy = dy - 256;
//storying the correct values
  pp->delta_x = dx;
  pp->delta_y = dy;
  pp->x_ov = (bytes[0] & PK_XOVF);
  pp->y_ov = (bytes[0] & PK_YOVF);

  for (int i = 0; i < size; i++) {
    pp->bytes[i] = bytes[i];
  }
}

bool(kbc_check_out_buf)() {
  uint8_t status;//temporary variable for status

  //first reads the status register
  if (kbc_read_status(&status)==1) {
    return 1;
  }

  //checks if the ouput buffer has data to read
  if (status & OBF) {
    //checks if the data is valid (i.e. if there are no errors)
    if ((status & (PAR | TIMEOUT)) == 0) {
      return true;
    }
    else {
      return false; //data has errors
    }
  }
  else
    return false; //no data to read
}

bool(kbc_check_in_buf)() {
  uint8_t status;//temporary variable for status

  //first reads the status register
  if (kbc_read_status(&status) ==1) {
    return 1;
  }

  //checks if the input buffer is full
  if ((status & IBF) == 0) {
    return true;
  }
  else
    return false; //no data to read
}

int(kbc_write_command)(uint32_t cmd) {
  //uint32_t tmp;
  int tries = 0; //counts the number of times the function tries to write to the kbc

  while (tries < 3) {
    if (kbc_check_in_buf()) {
      //writes the command
      if (sys_outb(KBC_CMD_REG, cmd) ==1) {
        return 1;
      }
      return 0;
    }

    //if it didn't succeed, it will count one more try and wait 20ms to give room for the kbc to respond
    tries++;
    tickdelay(micros_to_ticks(DELAY_US));
  }

  printf("Exceeded number of tries (3) and KBC was not ready for writing\n");
  return 1;
}

int(kbc_write_argument)(uint32_t arg) {
  int tries = 0; //counts the number of times the function tries to write to the kbc

  while (tries < 3) {
    if (kbc_check_in_buf()) {
      //writes the argument
      if (sys_outb( KBC_ARG_REG, arg) ==1) {
        return 1;
      }

      return 0;
    }

    //if it didn't succeed, it will count one more try and wait 20ms to give room for the kbc to respond
    tries++;
    tickdelay(micros_to_ticks(DELAY_US));
  }

  printf("Exceeded number of tries (3) and KBC was not ready for writing\n");
  return 1;
}

int(mouse_write_command)(uint32_t cmd, uint8_t *resp) {
  uint32_t tmp;
  int tries = 0; //counts the number of times the function tries to write to the kbc

  while (tries < 3) {
    if (kbc_check_in_buf()) {
      //writes the argument
      if (sys_outb( KBC_ARG_REG, cmd) ==1) {
        return 1;
      }

      tickdelay(micros_to_ticks(DELAY_US));

      if (sys_inb(KBC_OUT_BUF, &tmp) ==1) {
        return 1;
      }
      *resp = (uint8_t) tmp;
      return 0;
    }

    //if it didn't succeed, it will count one more try and wait 20ms to give room for the kbc to respond
    tries++;
    tickdelay(micros_to_ticks(DELAY_US));
  }

  printf("Exceeded number of tries (3) and KBC was not ready for writing\n");
  return 1;
}

int(kbc_command_handler)(uint32_t cmd, uint32_t arg) {
  uint8_t resp;
  uint8_t tries = 0;

  do {//check the type of command and handles it's Acknowledgement
    if (kbc_write_command(cmd) ==1) {
      return 1;
    }

    if (cmd == WRITE_B_MOUSE) {
      if (mouse_write_command(arg, &resp) ==1) {
        return 1;
      }

      if (resp == ACK) {
        return 0;
      }
      else if (resp == NACK) {
        tries++;
      }
      else if (resp == ERROR) //2 NACKS = ERROR, so the function will always return smth
      {
       return 1;
      }
    }
    else if (cmd ==KBC_WRT_COMMAND) {
      if (kbc_write_argument(arg) == 1) {
        return 1;
      }
      return 0;
    }

  } while (tries < 3);

  printf("Exceeded number of tries(3)\n");
  return 1;
}

struct mouse_ev *mouse_get_event(struct packet *pp) {
  static struct packet oldpk; // storing the previous packet for comparison
  static struct mouse_ev event;
  static bool first = true;

  uint8_t temp = pp->bytes[0];

  if (first) {
    event.type = BUTTON_EV;
    first = false;
  }
  else {
    if ((temp & PK_LB) == PK_LB && ((oldpk.bytes[0] & PK_LB) == 0)) {
      event.type = LB_PRESSED;
    }
    else if ((temp & PK_RB) == PK_RB && ((oldpk.bytes[0] & PK_RB) == 0)) {
      event.type = RB_PRESSED;
    }
    else if ((temp & PK_MB) == PK_MB && ((oldpk.bytes[0] & PK_MB) == 0)) {
      event.type = BUTTON_EV;
    }
    else if (((temp & PK_LB) == 0) && ((oldpk.bytes[0] & PK_LB) == PK_LB)) {
      event.type = LB_RELEASED;
    }
    else if (((temp & PK_RB) == 0) && ((oldpk.bytes[0] & PK_RB) == PK_RB)) {
      event.type = RB_RELEASED;
    }
    else if (((temp & PK_MB) == 0) && ((oldpk.bytes[0] & PK_MB) == PK_MB)) {
      event.type = BUTTON_EV;
    }
    else if (pp->bytes[1] != 0 || pp->bytes[2] != 0) {
      event.type = MOUSE_MOV;
      event.delta_x = pp->delta_x;
      event.delta_y = pp->delta_y;
    }
  }

  oldpk = *pp;
  return &event;
}

void mouse_detect_gesture(struct mouse_ev *evt, uint8_t x_len, uint8_t tolerance) {
  static enum state st = init;
  static bool accepted_line = false;
  static int16_t sum_x = 0;
//based on the state of the state machine
//it expects and handles the events returned by the mouse_get_event
  switch (st) {

    case init:
      if (evt->type == LB_PRESSED) {
        st = drag1;
      }
      break;

    case drag1:
      if (evt->type == MOUSE_MOV) {
        if(evt->delta_y  == 0 || evt->delta_x == 0)
        {
          evt->delta_y += 1;
          evt->delta_x += 1;
        }
        int slope = evt->delta_y / evt->delta_x;

        if (slope <= 1 && (abs(evt->delta_x) > tolerance || abs(evt->delta_y) > tolerance)) {
          st = init;
        }
        else {
          sum_x += evt->delta_x;
          if (abs(sum_x) >= x_len) {
            accepted_line = true;
          }
        }
      }
      else if (evt->type == LB_RELEASED && accepted_line) {
        sum_x = 0;
        accepted_line = false;
        st = wait;
      }
      else {
        st = init;
      }
      break;

    case wait:
      if (evt->type == MOUSE_MOV) {
        if (abs(evt->delta_x) > tolerance || abs(evt->delta_y) > tolerance) {
          st = init;
        }
      }
      else if (evt->type == RB_PRESSED) {
        st = drag2;
      }
      else if (evt->type == LB_PRESSED) {
        st = drag1;
      }
      else{
        st = init;
      }
      break;

    case drag2:
      if (evt->type == MOUSE_MOV) {
        int slope = evt->delta_y / evt->delta_x;
        if (slope >= -1 && (abs(evt->delta_x) > tolerance || abs(evt->delta_y) > tolerance)) {
          st = init;
        }
        else {
          sum_x += evt->delta_x;
          if (abs(sum_x) >= x_len) {
            accepted_line = true;
          }
        }
      }
      else if (evt->type == RB_RELEASED && accepted_line) {
        sum_x = 0;
        accepted_line = false;
        gesture = true;
        return;
      }
      else {
        st = init;
      }
      break;
  }
}
