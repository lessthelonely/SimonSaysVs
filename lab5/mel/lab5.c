// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>

#include "vc.h"
#include "kbc.h"
#include "i8042.h"
#include "i8254.h"
#include "graphic.h"

extern uint8_t sc_byte;

extern vbe_mode_info_t mode_info;

extern unsigned int bppixel;

extern unsigned int x_res;

extern unsigned y_res;

extern int counter;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  //Changing to video mode (mode)
  if(set_vbe_mode(mode)==1){ 
    return 1;
  }
  if(get_mode_info(mode)==1){
    return 1;
  }
  sleep(delay); //Waits delay seconds
  if(vg_exit()==1){ //Goes back to Minix's default text mode(function provided by teacher)
    return 1;
  }
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  //Changing to video mode (mode) --- Before we should map video RAM and then change to graphics mode to avoid the error: "Test FAILED: one or more missing/unexpected function calls!"
  if(get_mode_info(mode)==1){
    return 1;
  }
  if(set_vbe_mode(mode)==1){ 
    return 1;
  }

  //We need to enable keyboard interruptions since we need to process keyboard's scancodes to recognize that the user pressed/released the ESC key (ESC BREAKCODE is processed)
  uint8_t bit_no= KBC_IRQ;
  if(kbc_subscribe_int(&bit_no)==1){
    return 1;
  }
  
  int ipc_status;
  message msg;
  int r;
  bool ESC_flag = false;

  vg_draw_rectangle(x,y,width,height,color);

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
            if (sc_byte == ESC_BREAKCODE)
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

  //Unsubscribe keyboard interruptions
  if(kbc_unsubscribe_int()==1) {
    return 1;
  }
  //Return to text mode
  if(vg_exit()==1){ //Goes back to Minix's default text mode(function provided by teacher)
    return 1;
  }
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  //Changing to video mode (mode) --- Before we should map video RAM and then change to graphics mode to avoid the error: "Test FAILED: one or more missing/unexpected function calls!"
  if(get_mode_info(mode)==1){
    return 1;
  }
  if(set_vbe_mode(mode)==1){ 
    return 1;
  }

  //We need to enable keyboard interruptions since we need to process keyboard's scancodes to recognize that the user pressed/released the ESC key (ESC BREAKCODE is processed)
  uint8_t bit_no= KBC_IRQ;
  if(kbc_subscribe_int(&bit_no)==1){
    return 1;
  }

  uint16_t recWidth = x_res / no_rectangles;
  uint16_t recHeight = y_res / no_rectangles;

  uint32_t color = first;

  for (unsigned row = 0; row < no_rectangles; row++) {
    for (unsigned col = 0; col < no_rectangles; col++) {

      //gets the color for the next rectangle
      if(mode_info.MemoryModel==0x04){ //if color mode is indexed (or packed pixel, in VBE jargon)
      color = (first + (row * no_rectangles + col) * step) % (1 << bppixel);
      }
      
      if(mode_info.MemoryModel==0x06){//color mode is direct
      uint32_t red_mask = ((1 << mode_info.RedMaskSize)-1);
      uint32_t red=(((first>>mode_info.RedFieldPosition) & red_mask)+ col * step) % (1 << mode_info.RedMaskSize);

      uint32_t green_mask = ((1 << mode_info.GreenMaskSize)-1);
	    uint32_t green = (((first>>mode_info.GreenFieldPosition ) & green_mask)+ row * step) % (1 << mode_info.GreenMaskSize);
      
      uint32_t blue_mask = ((1 << mode_info.BlueMaskSize)-1);
	    uint32_t blue = (((first<<mode_info.BlueFieldPosition) & blue_mask)+ (col + row) * step) % (1 << mode_info.BlueMaskSize);	

      color=((red << mode_info.RedFieldPosition )| (green << mode_info.GreenFieldPosition)| blue);
      }

      if (vg_draw_rectangle(recWidth * col, recHeight * row, recWidth, recHeight, color)==1){
        return 1;
      } 

    }
  }

  int ipc_status=0;
  message msg;
  int r=0;
  bool ESC_flag=false;

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
            if (sc_byte == ESC_BREAKCODE)
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

  //Unsubscribe keyboard interruptions
  if(kbc_unsubscribe_int()==1) {
    return 1;
  }
  //Return to text mode
  if(vg_exit()==1){ //Goes back to Minix's default text mode(function provided by teacher)
    return 1;
  }
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  //Changing to video mode (mode) --- Before we should map video RAM and then change to graphics mode to avoid the error: "Test FAILED: one or more missing/unexpected function calls!"
  if(get_mode_info(0x114)==1){
     return 1;
   }
  printf("Hello\n");
  if(set_vbe_mode(0x114)==1){
    return 1;
  }


  //We need to enable keyboard interruptions since we need to process keyboard's scancodes to recognize that the user pressed/released the ESC key (ESC BREAKCODE is processed)
  uint8_t bit_no= KBC_IRQ;
  if(kbc_subscribe_int(&bit_no)==1){
    return 1;
  }

  int ipc_status;
  message msg;
  int r;
  bool ESC_flag = false;

  
  

  draw_xpm(x,y,xpm);

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
            if (sc_byte == ESC_BREAKCODE)
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

  //Unsubscribe keyboard interruptions
  if(kbc_unsubscribe_int()==1) {
    return 1;
  }
  //Return to text mode
  if(vg_exit()==1){ //Goes back to Minix's default text mode(function provided by teacher)
    return 1;
  }
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
                       //Changing to video mode (mode) --- Before we should map video RAM and then change to graphics mode to avoid the error: "Test FAILED: one or more missing/unexpected function calls!"
  if(get_mode_info(0x105)==1){
    return 1;
  }
  if(set_vbe_mode(0x105)==1){ 
    return 1;
  }

  //We need to enable keyboard interruptions since we need to process keyboard's scancodes to recognize that the user pressed/released the ESC key (ESC BREAKCODE is processed). Also need to enable timer interruptions since we need to find the number of frames per second
  uint8_t bit_no_kbc= KBC_IRQ;
  if(kbc_subscribe_int(&bit_no_kbc)==1){
    return 1;
  }
  uint8_t bit_no_timer= TIMER0_IRQ;
  if(timer_subscribe_int(&bit_no_timer)==1){
    return 1;
  }

  int ipc_status;
  message msg;
  int r;
  bool ESC_flag = false;

  draw_xpm(xi,yi,xpm);

  uint8_t time_per_frame =sys_hz()/fr_rate;
  int fr_passed=0;

  int32_t distance_to_end;
  
  if(yi==yf){
    distance_to_end =abs(xf-xi);
  }
  else {
    distance_to_end =abs(yf-yi);
  }

  while (!ESC_flag) {
    // Get a request message
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                                 // hardware interrupt notification
          if (msg.m_notify.interrupts & BIT(bit_no_kbc)) { // subscribed interrupt
              kbc_ih(); 
            if (sc_byte == ESC_BREAKCODE)
              ESC_flag = true;
          }
          if (msg.m_notify.interrupts & BIT(bit_no_timer)) { // subscribed interrupt
              timer_int_handler(); 
             if ((counter % time_per_frame == 0) ) {
                 //the end position was reached
                if((xi == xf) && (yi == yf))continue;

              
                //Vertical movement -> only y's matter
                if(xi == xf){

                  if(yf > yi)
                    yi += speed;
                  else yi -= speed;

                  distance_to_end -= speed;

                  if(distance_to_end < 0)
                    yi = yf;
                  
                  
                }
                //Horizontal movement -> only x's matter
                else{
                    if(xf > xi)
                      xi += speed;
                    else xi -= speed;
                    distance_to_end-= speed;
                    if(distance_to_end < 0)
                      xi = xf;
                      
                }
                
                vg_draw_rectangle(0,0,x_res,y_res,0);
                draw_xpm(xi, yi, xpm);
               
            }
            }
            else{

              if (counter % time_per_frame == 0)
                fr_passed++;

              if ((abs(speed)==fr_passed ) && (distance_to_end > 0)){
                fr_passed = 0; 
                if((yi == yf)&&(xi == xf) )
                  continue;

                //Vertical movement
                if(yi != yf){
                  if(yf > yi)
                    yi += 1;
                  else {
                    yi -= 1;
                  }
                }
                //Horizontal movement
                else{

                  if(xf > xi)
                    xi += 1;
                  else {
                    xi -= 1; 
                  }   

                }
                distance_to_end -= 1;
                vg_draw_rectangle(0,0,x_res,y_res,0);
                draw_xpm(xi, yi, xpm);
              }
            }break;
         

        default:
          break; // no other notifications expected: do nothing
      }
    }
    else { //received a standard message, not a notification
      // no standard messages expected: do nothing
    }
  }
   

  //Unsubscribe keyboard interruptions
  if(kbc_unsubscribe_int()==1) {
    return 1;
  }
  //Unsubscribe timer interruptions
  if(timer_unsubscribe_int()==1) {
    return 1;
  }
  //Return to text mode
  if(vg_exit()==1){ //Goes back to Minix's default text mode(function provided by teacher)
    return 1;
  }
  return 0;
                     }
                     
  
