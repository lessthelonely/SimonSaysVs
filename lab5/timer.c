#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "timer.h"
#include "utils.h"
#include "i8254.h"


int timer_freq_counter = 0;
int timer_hook_id;

int(timer_set_frequency)(uint8_t timer, uint32_t freq) {
  int call_back = 0;
  uint8_t st;
  uint8_t timer_control;
  uint16_t half_word_freq;
  int timer_address;

  switch (timer) {
    case 0:
      timer_control = TIMER_SEL0;
      timer_address = TIMER_0;
      break;
    case 1:
      timer_control = TIMER_SEL1;
      timer_address = TIMER_1;
      break;
    case 2:
      timer_control = TIMER_SEL2;
      timer_address = TIMER_2;
      break;

    default:
      return 1;
  }

  if (TIMER_FREQ / freq >= 1 << 16 || freq == 0){
    return 1;
  }

  call_back |= timer_get_conf(timer, &st);

  st = timer_control | TIMER_LSB_MSB | (st & 0xF); // Assembly of the control word
  call_back |= sys_outb(TIMER_CTRL, st);                          // Configure to set the frequency afterwards configuration

  half_word_freq = TIMER_FREQ / freq;
  call_back |= util_get_LSB(half_word_freq, &st);
  call_back |= sys_outb(timer_address, st);
  call_back |= util_get_MSB(half_word_freq, &st);
  call_back |= sys_outb(timer_address, st);

  return call_back;
}

int(timer_subscribe_int)(uint8_t *bit_no) {
  int call_back = 0;
  int hook_id = TIMER0_IRQ;
  *bit_no = hook_id; // Return argument (bit used to check against mask interrupt subscription)
  call_back = sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
  timer_hook_id = hook_id;
  return call_back;
}

int(timer_unsubscribe_int)() {
  return sys_irqrmpolicy(&timer_hook_id); //Unsubscribes notifications from the global variable that stores the hook_id
}

void(timer_int_handler)() {
  timer_freq_counter += 1;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL) {
    return 1;
  }

  int timer_address;
  switch (timer)
  {
  case 0:
    timer_address = TIMER_0;
    break;
  case 1:
    timer_address = TIMER_1;
    break;
  case 2:
    timer_address = TIMER_2;
    break;
  default:
    return 1;
  }
  int call_back;
  //First need to ask for the configuartion
  st[0] = TIMER_RB_CMD | TIMER_RB_COUNT_ | !TIMER_RB_STATUS_ | TIMER_RB_SEL(timer);
  call_back = sys_outb(TIMER_CTRL, st[0]);
  //Now the status is in the timer address

  //Then need to retrive it from the control address
  call_back |= util_sys_inb(timer_address, st);
  //st[0] is set with the status
  return call_back;
}

int(timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  if (timer > 2) {
    // Invalid timer
    return 1;
  }

  union timer_status_field_val conf;

  switch (field) {
    case (tsf_all):
      conf.byte = st;
      break;
    case (tsf_initial):
      conf.in_mode = (0x30 & st) >> 4;
      break;

    case (tsf_mode):
      conf.count_mode = (0xe & st) >> 1;
      break;

    case (tsf_base):
      conf.bcd = st & 1;
      break;
    default:
      return 1;
  }

  return timer_print_config(timer, field, conf);
}
