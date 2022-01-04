#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8254.h"

extern int counter; //used in interrupt loop in timer_test_int and incremented in timer_int_handler from timer.c

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  uint8_t st;
  //check not valid values for the timer
  if(timer<0){
    return 1;
  }
  if(timer>2){
    return 1;
  }
  if(timer_get_conf(timer,&st)==1){ //if function is unsuccessful
       return 1;
  }
  if(timer_display_conf(timer,st,field)==1){ //if function is unsuccessful
       return 1;
  }
  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  //check not valid values for the timer
  if(timer<0){
    return 1;
  }
  if(timer>2){
    return 1;
  }
  if(timer_set_frequency(timer,freq)==1){ //if function is unsuccessful
     return 1;
  }
  return 0;
}

int(timer_test_int)(uint8_t time) {
  uint8_t bit_no= TIMER0_IRQ;
  if(timer_subscribe_int(&bit_no)==1){ //Subscribe Timer 0 interrupts
    return 1;
  }
  int r;
  int ipc_status;
  message msg;
 
  while(counter<(time*60)) { //Timer 0 is configurated to generate interrupts at a fixed rate, by default 60 Hz. Then, on every Timer 0 interrupt, Minix 3 increments a counter variable, which it uses to measure the time -- it expects that counter to be incremented by 60 every second.
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
        continue;
    }
     if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */				
                if (msg.m_notify.interrupts & BIT(bit_no)) { //kernel activates the bit whose number has been passed in the hook_id argument of the sys_irqsetpolicy() call that subscribed that interrupt
                  timer_int_handler(); //increments counter variable on every interrupt
                  if ((counter % 60) == 0) //if a second has passed, counter is a multiple of 60
                  {
                  timer_print_elapsed_time(); //prints message at 1 second intervals
                  }
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
  }
  if(timer_unsubscribe_int()==1){ //Unsubscribe Timer 0 at the end
    return 1;
  }
  return 0;
}
