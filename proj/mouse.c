#include <lcom/lcf.h>
#include "mouse.h"
#include "i8042.h"
uint8_t kbc_ih_output;
bool kbc_ih_err, kbc_ih_obf, kbc_ih_ibf, kbc_ih_aux;

int(mouse_subscribe)(uint8_t *bit_no, int *notification) {
  int call_back = 0;
  int hook_id = MOUSE_IRQ;
  *bit_no = hook_id; // Return argument (bit used to check against mask interrupt subscription)
  call_back = sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
  *notification = hook_id; // Return argument ("must be used in other calls to specify the interrupt notification to operate on")
  return call_back;
}

int(mouse_unsubscribe)(int *notification) {
  return sys_irqrmpolicy(notification);
}

int mouse_data_reporting_enable() {
  uint8_t placeholder;
  return try_mouse_operation(MOUSE_ENAB_DATA_REP, &placeholder, 0, &placeholder, false);
}

int mouse_data_reporting_disable() {
  uint8_t placeholder;
  return try_mouse_operation(MOUSE_DIS_DATA_REP, &placeholder, 0, &placeholder, false);
}

void(mouse_get_state)() {
  int call_back = 0;
  uint8_t stat_reg, byte = 0;
  bool error, obf, ibf, aux;
  call_back |= util_sys_inb(KBC_STAT_REG, &stat_reg);

  error = stat_reg & (KBC_STAT_REG_PAR | KBC_STAT_REG_TIMEOUT);
  obf = stat_reg & KBC_STAT_REG_OBF;
  ibf = stat_reg & KBC_STAT_REG_IBF;
  aux = stat_reg & KBD_STAT_REG_AUX;
  if (obf) {
    // Only read if there's something to read
    call_back |= util_sys_inb(KBC_OUT_BUF, &byte);
  }
  kbc_ih_err = error;
  kbc_ih_obf = obf;
  kbc_ih_aux = aux;
  kbc_ih_ibf = ibf;
  //kbc_ih_stat_reg = stat_reg;
  kbc_ih_output = byte;
}

bool mouse_handler(struct packet *mouse_packet) {
  static uint8_t bytes[MOUSE_PACKET_SIZE];
  static int byte_count = 0;

  mouse_get_state();
  if (kbc_ih_err || !kbc_ih_obf || !kbc_ih_aux) {
    byte_count = 0; //Reset function memory
    return false;
  }
  bytes[byte_count] = kbc_ih_output;
  if (!mouse_is_synched(bytes, byte_count)) {
    byte_count = 0; //Reset function memory
    return false;
  }
  byte_count++;

  if (byte_count < 3) {
    return false;
  }

  *mouse_packet = assembly_packet(bytes);
  byte_count = 0;
  return true;
}

bool try_mouse_operation(uint8_t command, uint8_t *args, int args_size, uint8_t *ret_arg, bool has_ret) {
  int count = 0;
  while (count < 10) {
    if (mouse_write_command(command, args, args_size, ret_arg, has_ret)){
      return true;
    }
    count++;
    tickdelay(micros_to_ticks(DELAY_US)); //Just performed a task on the kbc. Need to wait
  }
  return false;
}

bool mouse_write_command(uint8_t command, uint8_t *args, int args_size, uint8_t *ret_arg, bool has_ret) {
  if (!mouse_write_byte(command)) {
    return false;
  }
  for (int i = 0; i < args_size; i++) {
    if (!mouse_write_byte(args[i])) {
      return false;
    }
  }

  if (has_ret) {
    tickdelay(micros_to_ticks(DELAY_US)); //Just performed a task on the kbc. Need to wait
    mouse_get_state();
    if (!kbc_ih_err && kbc_ih_obf && kbc_ih_aux) {
      *ret_arg = kbc_ih_output;
      return true;
    }
    return false; //False if cannot read response
  }

  return true;
}

bool mouse_write_byte(uint8_t byte) {
  int count = 0;
  while (count < 10) {
    mouse_get_state();
    if (!kbc_ih_err && !kbc_ih_obf && !kbc_ih_ibf) {
      // None of the buffers if full and there wasn't an error
      // Can give the command to the buffer
      sys_outb(KBC_ARG_REG, byte); // First give the argument
      sys_outb(KBC_CMD_REG, BYTE_TO_MOUSE_CMD);
      return mouse_write_byte_ack();
    }
    count++;
    //Commented because it didn't have the import
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return false;
}

bool mouse_write_byte_ack() {
  tickdelay(micros_to_ticks(DELAY_US)); //Need to wait, because this happens right after issuing a command

  //mouse_get_state();
  uint8_t ack;
  util_sys_inb(KBC_OUT_BUF, &ack);
  if (ack == BYTE_TO_MOUSE_SUCCESS) {
    return true;
  }
  return false;
}

struct packet assembly_packet(uint8_t *bytes) {

  bool rb = bytes[0] & PACKET_RB;
  bool mb = bytes[0] & PACKET_MB;
  bool lb = bytes[0] & PACKET_LB;
  int16_t dx = cpl2_9_to_16(bytes[1], bytes[0] & PACKET_X_MSB);
  int16_t dy = cpl2_9_to_16(bytes[2], bytes[0] & PACKET_Y_MSB);
  bool x_ov = bytes[0] & PACKET_X_OVFL;
  bool y_ov = bytes[0] & PACKET_Y_OVFL;
  struct packet return_packet;
  return_packet.bytes[0] = bytes[0];
  return_packet.bytes[1] = bytes[1];
  return_packet.bytes[2] = bytes[2];
  return_packet.delta_x = dx;
  return_packet.delta_y = dy;
  return_packet.lb = lb;
  return_packet.mb = mb;
  return_packet.rb = rb;
  return_packet.x_ov = x_ov;
  return_packet.y_ov = y_ov;
  return return_packet;

}

bool mouse_is_synched(uint8_t *bytes, int size) {
  if (size == 1) {
    return (bytes[0] & BIT(3));
  }
  return true;
}

