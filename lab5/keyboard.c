//
// Created by henri on 20/10/2020.
//
#include <lcom/lcf.h>

#include "keyboard.h"
#include "i8042.h"

extern uint8_t kbc_ih_output, kbc_ih_stat_reg;
extern bool kbc_ih_err, kbc_ih_obf;
bool kbc_ih_ibf;

int(keyboard_subscribe)(uint8_t *bin_no, int *notification) {
  int call_back = 0;
  int hook_id = KBD_IRQ;
  *bin_no = hook_id; // Return argument (bit used to check against mask interrupt subscription)
  call_back = sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
  *notification = hook_id; // Return argument ("must be used in other calls to specify the interrupt notification to operate on")
  return call_back;
}

int(keyboard_unsubscribe)(int *notification) {
  return sys_irqrmpolicy(notification);
}

void(kbc_ih)() {
  int call_back = 0;
  uint8_t stat_reg, byte = 0;
  bool error, obf, ibf;
  call_back |= util_sys_inb(KBC_STAT_REG, &stat_reg);

  error = stat_reg & (KBC_STAT_REG_PAR | KBC_STAT_REG_TIMEOUT);
  obf = stat_reg & KBC_STAT_REG_OBF;
  ibf = stat_reg & KBC_STAT_REG_IBF;

  if (obf) {
    // Only read if there's something to read
    call_back |= util_sys_inb(KBC_OUT_BUF, &byte);
  }
  //call_back |= util_sys_inb(KBC_OUT_BUF, &byte);
  kbc_ih_err = error;
  kbc_ih_obf = obf;
  kbc_ih_ibf = ibf;
  kbc_ih_stat_reg = stat_reg;
  kbc_ih_output = byte;
}

void keyboard_handler(){
  static bool assemblied_word;
  static uint8_t previous_byte;
  static uint8_t bytes[2];


  kbc_ih();
  if (kbc_ih_err || !kbc_ih_obf) {
      // Error in reading from the buffer
      return;
  }
  assemblied_word = keyboard_get_scancode(bytes, kbc_ih_output, assemblied_word);
  if (assemblied_word) {
      uint8_t size = scancode_get_size(bytes[0]);
      kbd_print_scancode(scancode_is_make(bytes[size - 1]), size, bytes);
  }

  previous_byte = kbc_ih_output; //Unused (might use if needs extra checks in keyboard_get_scancode)

  //Could have returned true or false to see if scancode_is_esc_break and exit the outside loop
}

bool keyboard_get_scancode(uint8_t *bytes, uint8_t this_byte, bool assemblied_word) {
  //The TWO_BYTE_SCANCODE_PREFIX might also be present in the second byte of a two byte scancode which poses a problem. Hence why we solve it this way
  //Needs to be reviewed
  //Return is if the scancode is complete

  //Note: could have made the assemblied_word static and use the return in the keyboard_handler
  if (!assemblied_word && bytes[0] == TWO_BYTE_SCANCODE_PREFIX) {
    bytes[1] = this_byte;
    return true;
  }
  else if (assemblied_word && this_byte == TWO_BYTE_SCANCODE_PREFIX) {
    bytes[0] = this_byte;
    return false;
  }
  else { // if assemblied_word isn't true, then the previous word must have been interrupted
    bytes[0] = this_byte;
    return true;
  }
}

int keyboard_enable_interrupts(uint8_t previous_command_byte) {
  uint8_t command_byte = previous_command_byte | CMD_BYTE_ENABLE_MOUSE_INTERRUPT | CMD_BYTE_ENABLE_KBD_INTERRUPT; //Enables mouse and keyboard interrupts
  return keyboard_write_command_byte(command_byte);

}

int keyboard_read_command_byte(uint8_t *command_byte) {
  int call_back = 0;
  call_back |= keyboard_input(READ_CMD_BYTE);
  kbc_ih();
  //keyboard_output();
  if (!kbc_ih_err && !kbc_ih_obf) { // There was not an error and the output (the current command byte) was read
    *command_byte = kbc_ih_output;
    return call_back;
  }
  return 1; // Doesn't catch the call_back possible errors
            // Instead throw's 1 because there was an error reading from the KBC as stated by kbc_ih_call_back being true
}

int keyboard_write_command_byte(uint8_t command_byte) {
  return keyboard_input_arg(WRITE_CMD_BYTE, command_byte);
}
int keyboard_input(uint8_t byte) {
  int call_back = 0, count = 0;
  while (count < 10) {
    //keyboard_output();
    kbc_ih();
    if (!kbc_ih_err && !kbc_ih_obf && !kbc_ih_ibf) {
      // None of the buffers if full and there wasn't an error
      // Can give the command to the buffer
      call_back |= sys_outb(KBC_CMD_REG, byte);
      return call_back;
    }
    count++;
    //Commented because it didn't have the import
     tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

int keyboard_input_arg(uint8_t byte, uint8_t arg) {
  int call_back = 0, count = 0;
  while (count < 10) {
    kbc_ih();
    //keyboard_output();
    if (!kbc_ih_err && !kbc_ih_obf && !kbc_ih_ibf) {
      // None of the buffers if full and there wasn't an error
      // Can give the command to the buffer
      call_back |= sys_outb(KBC_ARG_REG , arg); // First give the argument
      call_back |= sys_outb(KBC_CMD_REG, byte);
      return call_back;
    }
    count++;
    //Commented because it didn't have the import
    tickdelay(micros_to_ticks(DELAY_US));
  }
  return 1;
}

bool scancode_is_make(uint8_t data_byte) {
  return !(data_byte & BIT(7)); // If different than 0 will return true
}

uint8_t scancode_get_size(uint8_t previous_byte) {
  if (previous_byte == TWO_BYTE_SCANCODE_PREFIX) {
    return 2;
  }
  return 1;
}

bool scancode_is_esc_break(uint8_t data_byte) {
  return data_byte == ESC_BREAK_CODE;
}
