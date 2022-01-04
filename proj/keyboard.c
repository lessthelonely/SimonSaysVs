#include <lcom/lcf.h>

#include "keyboard.h"
#include "i8042.h"

static uint8_t kbc_ih_output, kbc_ih_stat_reg;
static bool kbc_ih_err, kbc_ih_obf;
static bool kbc_ih_ibf;

int(keyboard_subscribe)(uint8_t *bin_no, int *notification)
{
  int call_back = 0;
  int hook_id = KBD_IRQ;
  *bin_no = hook_id; // Return argument (bit used to check against mask interrupt subscription)
  call_back = sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
  *notification = hook_id; // Return argument ("must be used in other calls to specify the interrupt notification to operate on")
  return call_back;
}

int(keyboard_unsubscribe)(int *notification)
{
  return sys_irqrmpolicy(notification);
}

void(kbc_ih)()
{
  int call_back = 0;
  uint8_t stat_reg, byte = 0;
  bool error, obf, ibf;
  call_back |= util_sys_inb(KBC_STAT_REG, &stat_reg);

  error = stat_reg & (KBC_STAT_REG_PAR | KBC_STAT_REG_TIMEOUT);
  obf = stat_reg & KBC_STAT_REG_OBF;
  ibf = stat_reg & KBC_STAT_REG_IBF;

  if (obf)
  {
    // Only read if there's something to read
    call_back |= util_sys_inb(KBC_OUT_BUF, &byte);
  }
  kbc_ih_err = error;
  kbc_ih_obf = obf;
  kbc_ih_ibf = ibf;
  kbc_ih_stat_reg = stat_reg;
  kbc_ih_output = byte;
}

bool keyboard_handler(struct key_press *key)
{
  static bool assemblied_word;
  //static uint8_t previous_byte;
  static uint8_t bytes[2];

  kbc_ih();
  if (kbc_ih_err || !kbc_ih_obf)
  {
    // Error in reading from the buffer
    return false;
  }
  assemblied_word = keyboard_get_scancode(bytes, kbc_ih_output, assemblied_word);
  if (assemblied_word)
  {
    uint8_t size = scancode_get_size(bytes[0]);
    bool is_make = scancode_is_make(bytes[size - 1]);
    kbd_print_scancode(is_make, size, bytes);
    *key = assemble_key_press(is_make, size, bytes);
  }
  return true;
}

bool keyboard_get_scancode(uint8_t *bytes, uint8_t this_byte, bool assemblied_word)
{

  if (!assemblied_word && bytes[0] == TWO_BYTE_SCANCODE_PREFIX)
  {
    bytes[1] = this_byte;
    return true;
  }
  else if (assemblied_word && this_byte == TWO_BYTE_SCANCODE_PREFIX)
  {
    bytes[0] = this_byte;
    return false;
  }
  else
  { // if assemblied_word isn't true, then the previous word must have been interrupted
    bytes[0] = this_byte;
    return true;
  }
}

struct key_press assemble_key_press(bool is_make, uint8_t size, uint8_t bytes[2])
{
  struct key_press key;
  key.down = is_make;
  uint8_t byte = bytes[size - 1];
  if (size == 1)
  {
    switch (byte)
    //Break codes
    {
    case KEY_A:
      key.key = 'a';
      break;

    case KEY_B:
      key.key = 'b';
      break;

    case KEY_C:
      key.key = 'c';
      break;

    case KEY_D:
      key.key = 'd';
      break;

    case KEY_E:
      key.key = 'e';
      break;

    case KEY_F:
      key.key = 'f';
      break;

    case KEY_G:
      key.key = 'g';
      break;

    case KEY_H:
      key.key = 'h';
      break;

    case KEY_I:
      key.key = 'i';
      break;

    case KEY_J:
      key.key = 'j';
      break;

    case KEY_K:
      key.key = 'k';
      break;
    case KEY_L:
      key.key = 'l';
      break;

    case KEY_M:
      key.key = 'm';
      break;

    case KEY_N:
      key.key = 'n';
      break;

    case KEY_O:
      key.key = 'o';
      break;

    case KEY_P:
      key.key = 'p';
      break;

    case KEY_Q:
      key.key = 'q';
      break;

    case KEY_R:
      key.key = 'r';
      break;

    case KEY_S:
      key.key = 's';
      break;

    case KEY_T:
      key.key = 't';
      break;

    case KEY_U:
      key.key = 'u';
      break;

    case KEY_V:
      key.key = 'v';
      break;

    case KEY_W:
      key.key = 'w';
      break;

    case KEY_X:
      key.key = 'x';
      break;

    case KEY_Y:
      key.key = 'y';
      break;

    case KEY_Z:
      key.key = 'z';
      break;

    case KEY_1:
      key.key = '1';
      break;

    case KEY_2:
      key.key = '2';
      break;

    case KEY_3:
      key.key = '3';
      break;

    case KEY_4:
      key.key = '4';
      break;

    case KEY_5:
      key.key = '5';
      break;

    case KEY_6:
      key.key = '6';
      break;

    case KEY_7:
      key.key = '7';
      break;

    case KEY_8:
      key.key = '8';
      break;

    case KEY_9:
      key.key = '9';
      break;

    case KEY_0:
      key.key = '0';
      break;

    case KEY_ESC:
      key.key = ESC;
      break;

    case KEY_ENTER:
      key.key = ENTER;
      break;

    case KEY_BACKSPACE:
      key.key = BACKSPACE;
      break;

    default:
      key.key = 0;
      break;
    }
  }
  else if (size == 2)
  {
    switch (byte)
    {
    case KEY_UP_ARROW:
      key.key = UP_ARROW;
      break;

    case KEY_DOWN_ARROW:
      key.key = DOWN_ARROW;
      break;
    default:
      break;
    }
  }

  return key;
}

bool scancode_is_make(uint8_t data_byte)
{
  return !(data_byte & BIT(7)); // If different than 0 will return true
}

uint8_t scancode_get_size(uint8_t previous_byte)
{
  if (previous_byte == TWO_BYTE_SCANCODE_PREFIX)
  {
    return 2;
  }
  return 1;
}

bool scancode_is_esc_break()
{
  return kbc_ih_output == ESC_BREAK_CODE;
}
