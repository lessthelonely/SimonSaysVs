#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int counter=0; //counter used in interrupt loop in timer_test_int from lab2.c
int hook_id_timer; //variable whose value we get in timer_subscribe_int and use in timer_unsubscribe_int, hence why we declare it here

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if(freq>TIMER_FREQ){ //Check if freq goes beyond the clock frequency
    return 1;
  }
  uint8_t st;
  if(timer_get_conf(timer,&st)==1){ //Reading timer configuration 
    return 1;
  }
  //Write control word to configure timer
  uint8_t savefb=(st&0xf); //the 4 least signficant bits are saved
  int port;
  uint8_t ctrl_word;
  if(timer==0){
    ctrl_word=  TIMER_SEL0| TIMER_LSB_MSB | savefb;
    port= TIMER_0;
  }
  if(timer==1){
    ctrl_word=  TIMER_SEL1 | TIMER_LSB_MSB | savefb;
     port=TIMER_1;
  }
  if(timer==2){
    ctrl_word=  TIMER_SEL2 | TIMER_LSB_MSB | savefb;
     port=TIMER_2;
  }
  if (sys_outb(TIMER_CTRL,ctrl_word)==1){ //Write control word to control register
    return 1;
  }
  uint32_t d_value=TIMER_FREQ/freq; //Calculate value of the divisor
  uint16_t new_d_value= (uint16_t)d_value; //Convert to correct type 
  uint8_t new_msb,new_lsb;
  //Get initial values
  util_get_LSB(new_d_value,&new_lsb);
  util_get_MSB(new_d_value,&new_msb);
  //Load timer's register
  if(sys_outb(port,new_lsb)==1){
    return 1;
  }
   if(sys_outb(port,new_msb)==1){
    return 1;
   }
   return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  hook_id_timer=(int)*bit_no; //Convert to right type in order to use irqsetpolicy (third argument is int)
  if(sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook_id_timer)==1){
      return 1;
  }
  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&hook_id_timer)==1){
  return 1;
  }
  return 0;
}

void (timer_int_handler)() {
  counter++;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st)
{
  uint32_t rbc=TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
if(sys_outb(TIMER_CTRL,rbc)==1){ //checks if it can write readback command
  return 1;
}
int port;
if(timer==0){
  port= TIMER_0;
}
if(timer==1){
  port=TIMER_1;
}
if(timer==2){
  port=TIMER_2;
}
if(util_sys_inb(port,st)==1){
   return 1;
} 
return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val conf; //declare third argument of timer_print_config
  if(field==tsf_all){ //configuration or status
    conf.byte=st;
  }
  if(field==tsf_initial){ //initialization mode bits 5,4
     conf.in_mode=((st&TIMER_LSB_MSB)>>4); /*there's four possible values: 3 (11 in binary-->Initialization of LSB and MSB, in this order); 2 (10 in binary-->MSB only); 1 (01 in binary-->LSB only) and 0 (invalid initialization mode)*/
  }
  if(field==tsf_mode){ //operating mode bits 3,2,1
    conf.count_mode=((st&0xf)>>1);
  }
  if(field==tsf_base){ //see if bit 0 indicates bcd or binary
     if((TIMER_BIN & st)==1){ //counting base is binary 
       conf.bcd=false;
     }
     else{ //counting base is bcd
       conf.bcd=true;
     }
  }
  if(timer_print_config(timer,field,conf)==1){
    return 1;
  }
  else{
    return 0;
  }
}
