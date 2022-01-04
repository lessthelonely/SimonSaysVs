#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "kbc.h"

#include "i8254.h"

extern int inb_counter;

extern uint8_t sc_byte;

uint8_t size = 0;

uint8_t bytes[2];

extern int counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  uint8_t bit_no= KBC_IRQ;

  if (kbc_subscribe_int(&bit_no) == 1) {
    return 1;
  }

  int ipc_status;
  message msg;
  int r;
  bool ESC_flag = false;

  while (!ESC_flag) {
    // Get a request message
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                 // hardware interrupt notification
          if (msg.m_notify.interrupts & BIT(bit_no)) { // subscribed interrupt
              kbc_ih(); 
            if (sc_byte == FB_OF_TB) {
              bytes[0] = sc_byte;
              size++;
            }
            else {
              bool make;
              bytes[size] = sc_byte;
              size++;
               if ((BIT(7) & sc_byte) == BIT(7)) {
                make = false;
              }
              else {
                make = true;
              }
              kbd_print_scancode(make, size, bytes); //printing the scancode
              size = 0;                              //reseting size
            }
            if (bytes[0] == ESC_BREAKCODE)
              ESC_flag = true;
          }
          break;

        default:
          break; // no other notifications expected: do nothing
      }
    }
    else { //received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }
  kbd_print_no_sysinb(inb_counter);

  if(kbc_unsubscribe_int()==1) {
    return 1;
  }

  return 0;
}


int(kbd_test_poll)() {
  bool ESC_flag=false;
  do{
    while(!kbc_check_status()){ //while there is no data in the buffer or it's full of errors, the function won't move on (won't go get the data)
      continue;
    }
    kbc_read_buf(&sc_byte);
    if(sc_byte==FB_OF_TB){
      bytes[0]=sc_byte;
      size++;
    }
    else {
              bool make;
              bytes[size] = sc_byte;
              size++;
               if ((BIT(7) & sc_byte) == BIT(7)) {
                make = false;
              }
              else {
                make = true;
              }
              kbd_print_scancode(make, size, bytes); //printing the scancode
              size = 0;                              //reseting size
            }
            if (bytes[0] == ESC_BREAKCODE)
              ESC_flag = true;
          }while(!ESC_flag);

      kbd_print_no_sysinb(inb_counter);

      uint8_t kbc_cmdbyte; //preparing to reactivate kbd interrupts
      if(kbc_wrt_cmd(KBC_RBC_COMMAND,0)==1){
        return 1;
      }

      tickdelay(micros_to_ticks(DELAY_US)); //give kbc time to respond 

      if(kbc_read_buf(&kbc_cmdbyte)==1){
        return 1;
      }

      uint32_t eint= kbc_cmdbyte |  ENB_KBD_INT; //preserve previous command byte + enables interruptions
      
      if(kbc_wrt_cmd(KBC_WRT_COMMAND,eint)==1){ //writes new command 
        return 1; 
      }
      return 0;
}

int(kbd_test_timed_scan)(uint8_t n) {
  uint8_t bit_no_kbc= KBC_IRQ;
  if (kbc_subscribe_int(&bit_no_kbc) == 1) { //Subscribe Keyboard interrupts
    return 1;
  }
  uint8_t bit_no_timer= TIMER0_IRQ;
  if(timer_subscribe_int(&bit_no_timer)==1){ //Subscribe Timer 0 interrupts
    return 1;
  }
  bool ESC_flag=false;
  int r;
  int ipc_status;
  message msg;
 
  while(!ESC_flag && (counter<(n*60))) { //Timer 0 is configurated to generate interrupts at a fixed rate, by default 60 Hz. Then, on every Timer 0 interrupt, Minix 3 increments a counter variable, which it uses to measure the time -- it expects that counter to be incremented by 60 every second (counter<n*60 is the condition to break the loop on the timer's behalf). We should also break the loop if the ESC key is release (ESC breakcode is processed)
     if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
         printf("driver_receive failed with: %d", r);
        continue;
    }
     if (is_ipc_notify(ipc_status)) { /* received notification */
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: /* hardware interrupt notification */		
            if (msg.m_notify.interrupts & BIT(bit_no_kbc)) { // subscribed interrupt
              kbc_ih(); 
              counter=0; //reset timer counter, to count the idle seconds during which no scancode is received, in other to break the loop if it's equal to n
            if (sc_byte == FB_OF_TB) {
              bytes[0] = sc_byte;
              size++; 
            }
            else {
              bool make;
              bytes[size] = sc_byte;
              size++;
               if ((BIT(7) & sc_byte) == BIT(7)) {
                make = false;
              }
              else {
                make = true;
              }
              kbd_print_scancode(make, size, bytes); //printing the scancode
              size = 0;                              //reseting size
            }
            if (bytes[0] == ESC_BREAKCODE)
              ESC_flag = true;
          }		
                if (msg.m_notify.interrupts & BIT(bit_no_timer)) { //kernel activates the bit whose number has been passed in the hook_id_timer argument of the sys_irqsetpolicy() call that subscribed that interrupt
                  timer_int_handler(); //increments counter variable on every interrupt
                }
                break;
            default:
                break; /* no other notifications expected: do nothing */	
        }
    }
  }
  if(kbc_unsubscribe_int()==1) { //Unsubscribe kbc at the end
    return 1;
  }
  if(timer_unsubscribe_int()==1){ //Unsubscribe Timer 0 at the end
    return 1;
  }
  return 0;
}

