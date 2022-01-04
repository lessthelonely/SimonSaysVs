//
// Created by henri on 20/10/2020.
//

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>

/*
 * The subscription / unsubscription should go to utils.h in the project, with another argument in the subscribe for the correspondent device
 */
int(keyboard_subscribe)(uint8_t *bit_no, int *notification);

int (keyboard_unsubscribe)(int *notification);

void keyboard_handler();

bool keyboard_get_scancode(uint8_t *bytes, uint8_t this_byte, bool assemblied_word);

int keyboard_enable_interrupts(uint8_t previous_command_byte);

int keyboard_read_command_byte(uint8_t *command_byte);

int keyboard_write_command_byte(uint8_t command_byte);

int keyboard_input(uint8_t byte);

int keyboard_input_arg(uint8_t byte, uint8_t arg);

bool scancode_is_make(uint8_t data_byte);

uint8_t scancode_get_size(uint8_t first_byte);

bool scancode_is_esc_break(uint8_t data_byte);

#endif //G06_KEYBOARD_H
