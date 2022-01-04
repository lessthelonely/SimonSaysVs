#include <lcom/lcf.h>
//#include <lcom/liblm.h>
//#include "vc.h"
//#include "i8042.h"
#include <stdbool.h>
#include <stdint.h>
//#include "menu.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/code/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/code/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

/**
 * @brief Main loop of the project.
 * 
 */
int(proj_main_loop)(int argc, char *argv[]) {
  printf("Holo\n");
  /*get_mode_info(0x105);
  set_vbe_mode(0x105);
  
  //jogo();
  vg_exit();*/
  return 0;
}
