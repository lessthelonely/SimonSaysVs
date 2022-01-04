#include <lcom/lcf.h>

#include <stdint.h>


int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if(lsb==NULL){
    return 1;
  }
  uint16_t sixtlsb=val&0x00ff;
  *lsb=(uint8_t)sixtlsb;
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if(msb==NULL){
    return 1;
  }
  uint16_t sixtmsb=val&0xff00;
  sixtmsb>>=8;
  *msb=(uint8_t)sixtmsb;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  if(value==NULL){
    return 1;
  }
  uint32_t byte; //need to create a variable uint32_t to call sys_inb
  sys_inb(port,&byte);
  *value=(uint8_t)byte;
  return 0;
}
