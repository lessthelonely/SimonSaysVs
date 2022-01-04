#include <lcom/lcf.h>
#include <stdint.h>

#include "i8042.h"
#include "kbc.h"

int hook_id_kbc;//variable whose value we get in kbc_subscribe_int and use in kbc_unsubscribe_int, hence why we declare it here

uint8_t sc_byte;

int inb_counter=0;

uint8_t stat;


//Modified version of sys_inb. This one also counts the number of calls
int(sys_inb_cnt)(port_t port, uint8_t *value) {
  if(value==NULL){
    return 1;
  }
  uint32_t byte; //need to create a variable uint32_t to call sys_inb
  sys_inb(port,&byte);
  *value=(uint8_t)byte;
  inb_counter++;
  return 0;
}

int (kbc_subscribe_int)(uint8_t *bit_no) {
  hook_id_kbc=(int)*bit_no; //Convert to right type in order to use irqsetpolicy (third argument is int)
  if(sys_irqsetpolicy(KBC_IRQ,IRQ_REENABLE|IRQ_EXCLUSIVE,&hook_id_kbc)==1){
      return 1;
  }
  return 0;
}

int (kbc_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id_kbc)==1){
  return 1;
  }
  return 0;
}


int (kbc_read_status)(uint8_t *st){
  if(sys_inb_cnt(KBC_ST_REG,st)==1){
    return 1;
  }
  return 0;
}

int (kbc_read_buf)(uint8_t *sc_byte){
   if(sys_inb_cnt(KBC_OUT_BUF,sc_byte)==1){
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

void (kbc_ih)(){
  if(kbc_check_status()){
  kbc_read_buf(&sc_byte);
  }
}


int (kbc_wrt_cmd) (uint32_t cmd,uint32_t args){
  uint8_t st;
    if(kbc_read_status(&st)==1){
      return 1;
    }
    if((st&IBF)==0){
      if(sys_outb(KBC_CMD_REG,cmd)==1){
        return 1;
      }
    }
    else{
      if(sys_outb(KBC_ARG_REG,args)==1){
          return 1;
        }
    }
  return 0;
}


