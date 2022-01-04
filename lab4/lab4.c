// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "i8254.h"

#include "mouse.h"

extern uint8_t sc_byte;

//used in interrupt loops
uint8_t size = 0; //size of the array of the packets
uint8_t bytes[3];//array with the bytes of the packets

extern int counter;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
 uint8_t bit_no= MOUSE_IRQ;

 //Subscribe interrupts
  if (mouse_subscribe_int(&bit_no) == 1) {
    return 1;
  }
  printf("Mouse subscribed\n");

   //Enable Stream Mode
  if (kbc_command_handler(WRITE_B_MOUSE, MOUSE_ENABLE_REPORT) == 1) {
   printf("ERROR\n");
   return 1;
 }
 printf("Done...I think\n");
 
  unsigned int num_packets = 0; //track number of packets processed
  int ipc_status;
  message msg;
  int r;
  uint32_t irq_set=BIT(bit_no); //avoid error in driver_receive

  while (num_packets < cnt) {
    // Get a request message
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
   
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                 // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set) { // subscribed interrupt
            mouse_ih();

            if (size == 0 && (sc_byte & BIT(3)) == 0) {//checking if it's the first byte--->the first byte has the third bit set to one
              continue;
            }
            else {//if the array doesn't have all the bytes
              bytes[size] = sc_byte;
              size++;
            }

            if (size == 3) {//if the array has 3 bytes, it has a complete packet 
              struct packet pp;
              mouse_parse_packet(&pp, bytes, size);
              mouse_print_packet(&pp);
              num_packets++;
              size = 0; //restart cycle, if we don't have all the packets that the function requires
            }
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

  //Disable stream mode (data reporting)
  if (kbc_command_handler(WRITE_B_MOUSE, MOUSE_DISABLE_REPORT)==1) {
    printf("ERROR");
    return 1;
  }
  printf("Disabled...I think\n");

  //Unsubscribe mouse interrupts
  if (mouse_unsubscribe_int() ==1) {
    return 1;
  }
  printf("Unsubscribed\n");
  return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    uint8_t bit_no_mouse=MOUSE_IRQ;
  if (mouse_subscribe_int(&bit_no_mouse) == 1) { //Subscribe Mouse interrupts
    return 1;
  }
  uint8_t bit_no_timer= TIMER0_IRQ;
  if(timer_subscribe_int(&bit_no_timer)==1){ //Subscribe Timer 0 interrupts
    return 1;
  }
  //Enable Stream Mode
  if (kbc_command_handler(WRITE_B_MOUSE, MOUSE_ENABLE_REPORT) == 1) {
   printf("ERROR\n");
   return 1;
 }
 printf("Done...I think\n");

  int ipc_status;
  message msg;
  int r;
  int seconds = 0;
  int frequency = sys_hz(); //determine timer 0's frequency

  uint32_t irq_set_timer=BIT(bit_no_timer); //avoid error in driver_receive
  uint32_t irq_set_mouse=BIT(bit_no_mouse); //avoid error in driver_receive

  while (seconds != idle_time) {//check if the time passed without packets is enough to stop the loop
    // Get a request message
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_timer) {
            //keeps track of the interrupts and converts them to seconds
            timer_int_handler();

            if (counter % frequency == 0) {
              seconds++;
            }
          }
          if (msg.m_notify.interrupts & irq_set_mouse) {
            //handles the mouse interrupt and reset the seconds and interrupt counters
            mouse_ih();
            counter = 0;
            seconds = 0;

            if (size == 0 && (sc_byte & PK_BIT3) == 0) {//checking if it's the first byte--->the first byte has the third bit set to one
              continue;
            }
            else {//if the array doesn't have all the bytes
              bytes[size] = sc_byte;
              size++;
            }

            if (size == 3) {//if the array has 3 bytes, it has a complete packet 
              struct packet pp;
              mouse_parse_packet(&pp, bytes, size);
              mouse_print_packet(&pp);
              size = 0;
            }
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

  //Disable data reporting
  if (kbc_command_handler(WRITE_B_MOUSE, MOUSE_DISABLE_REPORT)==1) {
    printf("ERROR");
    return 1;
  }
  printf("Disabled...I think\n");
  
 //Unsubscribe mouse interrupts
  if (mouse_unsubscribe_int() ==1) {
    return 1;
  }
  //Unsubscribe timer interrupts
  if (timer_unsubscribe_int() ==1 ) {
    return 1;
  }


  return 0;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  uint8_t irq_bitmask;

  //Enable Stream Mode
  if (kbc_command_handler(WRITE_B_MOUSE, MOUSE_ENABLE_REPORT) ==1) {
    return 1;
  }
  //Subscribe mouse interrupts
  if (mouse_subscribe_int(&irq_bitmask) ==1) {
    return 1;
  }
//interrupt loop variables
  int ipc_status;
  message msg;
  int r;
  struct packet pk;
  struct mouse_ev *evt;

  uint32_t irq_set_mouse=BIT(irq_bitmask); //avoid error in driver_receive

  while (!gesture) {
    // Get a request message
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                 // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set_mouse) { // subscribed interrupt
            mouse_ih();

            if (size == 0 && (sc_byte & PK_BIT3) == 0) {//checking if it's the first byte--->the first byte has the third bit set to one
              continue;
            }
            else {//if the array doesn't have all the bytes
              bytes[size] = sc_byte;
              size++;
            }

            if (size == 3) {//if the array has 3 bytes, it has a complete packet 
              mouse_parse_packet(&pk, bytes, size);
              mouse_print_packet(&pk);
              size = 0;
            }

              evt = mouse_get_event(&pk);
              mouse_detect_gesture(evt, x_len, tolerance);
          }
          break;

        default:
          break; // no other notifications expected: do nothing
      }
    }
  }
  //Disable data reporting
  if (kbc_command_handler(WRITE_B_MOUSE, MOUSE_DISABLE_REPORT) ==1) {
    return 1;
  }
//Unsubscribe mouse interrupts
  if (mouse_unsubscribe_int() ==1) {
    return 1;
  }
  return 0;
}


int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    unsigned int num_packets = 0;

  //turning on read data
  if (kbc_command_handler(WRITE_B_MOUSE, MOUSE_READ_DATA) == 1) {
    printf("mouse_test_remote::Error requesting data from the mouse\n");
    return 1;
  }

  while (true) {
    while (!kbc_check_out_buf()) {//checks if valid data to read from the output
      continue;
    }

    kbc_read_buf(&sc_byte);

    if (size == 0 && (sc_byte & PK_BIT3) == 0) {//checking if out of sync
      continue;
    }
    else {//if the packet array not full
      bytes[size] = sc_byte;
      size++;
    }

    if (size == 3) {//when the array has the full packet
      struct packet pk;
      mouse_parse_packet(&pk, bytes, size);
      mouse_print_packet(&pk);
      num_packets++;
      size = 0;

      //the loop stops when the number of packets displayed is the same as the user specified
      if (cnt == num_packets)
        break;

      //request another packet after period ms
      tickdelay(micros_to_ticks(period * 1000));
      if (kbc_command_handler(WRITE_B_MOUSE, MOUSE_READ_DATA) ==1){
        return 1;
      }
    }
  }
  //sets the streaming mode back on
  if (kbc_command_handler(WRITE_B_MOUSE, MOUSE_STREAM_MODE) != 0) {
    printf("mouse_test_remote::Error reseting mouse to stream mode\n");
    return 1;
  }
  //disables data report mode
  if (kbc_command_handler(WRITE_B_MOUSE, MOUSE_DISABLE_REPORT) != 0) {
    printf("mouse_test_remote::Error disabling data reporting\n");
    return 1;
  }

  uint32_t dflt_cmd_byte = (uint32_t) minix_get_dflt_kbc_cmd_byte();
  //updates to the default command byte from minix
  if (kbc_command_handler(KBC_WRT_COMMAND, dflt_cmd_byte) != 0) {
    printf("mouse_test_remote::Error seting kbc command byte to defaulte\n");
    return 1;
  }

  return 0;
}

