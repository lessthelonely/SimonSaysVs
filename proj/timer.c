#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "timer.h"
#include "utils.h"
#include "i8254.h"

int timer_freq_counter = 0;
int timer_hook_id;

int(timer_subscribe_int)(uint8_t *bit_no)
{
  int call_back = 0;
  int hook_id = TIMER0_IRQ;
  *bit_no = hook_id; // Return argument (bit used to check against mask interrupt subscription)
  call_back = sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id);
  timer_hook_id = hook_id;
  return call_back;
}

int(timer_unsubscribe_int)()
{
  return sys_irqrmpolicy(&timer_hook_id); //Unsubscribes notifications from the global variable that stores the hook_id
}
