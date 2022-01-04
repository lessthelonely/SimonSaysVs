#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <lcom/lcf.h>

/** @defgroup keyboard keyboard
 * @{
 *
 * Functions for handling keyboard
 */


/**
 * @brief Breakcodes for the keypresses that can be detected
 */
enum uint16_t
{
    KEY_A = 0x9e,
    KEY_B = 0xb0,
    KEY_C = 0xae,
    KEY_D = 0xa0,
    KEY_E = 0x92,
    KEY_F = 0xa1,
    KEY_G = 0xa2,
    KEY_H = 0xa3,
    KEY_I = 0x97,
    KEY_J = 0xa4,
    KEY_K = 0xa5,
    KEY_L = 0xa6,
    KEY_M = 0xb2,
    KEY_N = 0xb1,
    KEY_O = 0x98,
    KEY_P = 0x99,
    KEY_Q = 0x90,
    KEY_R = 0x93,
    KEY_S = 0x9f,
    KEY_T = 0x94,
    KEY_U = 0x96,
    KEY_V = 0xaf,
    KEY_W = 0x91,
    KEY_X = 0xad,
    KEY_Y = 0x95,
    KEY_Z = 0xac,
    KEY_1 = 0x82,
    KEY_2 = 0x83,
    KEY_3 = 0x84,
    KEY_4 = 0x85,
    KEY_5 = 0x86,
    KEY_6 = 0x87,
    KEY_7 = 0x88,
    KEY_8 = 0x89,
    KEY_9 = 0x8a,
    KEY_0 = 0x8b,
    KEY_UP_ARROW = 0xc8,
    KEY_DOWN_ARROW = 0xd0,
    KEY_ESC = 0x81,
    KEY_ENTER = 0x9c,
    KEY_BACKSPACE = 0x8e
} break_codes;

/**
 * @brief Char codes for some special keys
 */
enum
{
    UP_ARROW = 19,
    DOWN_ARROW = 20,
    ENTER = 10,
    BACKSPACE = 8,
    ESC = 27
} special_keys;

/**
 * @brief Stores the data from a keypress
 */
struct key_press
{
    /**
     * @brief The char code of the key pressed
     * @details Arrows are not the standard ascii codes as they don't have representation
     */
    char key;

    /**
     * @brief If was a breakcode or a makecode
     * @details True if is a makecode, false otherwise
     */
    bool down;
};

/**
 * @brief Subscribes keyboard interruptions
 * @param bit_no Return argument with the mask bit
 * @param notification Return argument with the notification address
 */
int(keyboard_subscribe)(uint8_t *bit_no, int *notification);

/**
 * @brief Unsubscribes keyboard interruptions
 * @param notificatoin The correspondent keyboard notification address
 */
int(keyboard_unsubscribe)(int *notification);

/**
 * @brief Handles the keyboard interruptions and tries to assemble a key press
 * @param key Return argument with the key press
 * @return True if successfully got a key press, false otherwise
 */
bool keyboard_handler(struct key_press *key);

/**
 * @brief Checks if completed a scancode and stores it
 * @param bytes Where the bytes from the scancode are stored
 * @param this_byte The byte received from the kbc
 * @param assemblied_word If the previous interruption resulted in completing a scancode
 * @return True if completed a scancode, false otherwise
 */
bool keyboard_get_scancode(uint8_t *bytes, uint8_t this_byte, bool assemblied_word);

/**
 * @brief Assembles a keypress from the kbc data
 * @param is_make True if is a makecode, false otherwise
 * @param size Size of the scancode
 * @param bytes Where the information from the keypress comes from
 * @return The key_press 
 */
struct key_press assemble_key_press(bool is_make, uint8_t size, uint8_t bytes[2]);

/**
 * @brief If the scancode is a makecode or a breakcode
 * @param data_byte The byte received
 * @return True if is a makecode, false if is breakcode
 */
bool scancode_is_make(uint8_t data_byte);

/**
 * @brief Gets the size of the scancode
 * @param first_byte The first byte of the scancode
 * @return The size of the scancode
 */
uint8_t scancode_get_size(uint8_t first_byte);

/**
 * @brief If the scancode is an esc breakcode
 * @details Used to check if should exit a program or a loop after receiving  the last output from the kbc
 * @return True if is esc breakcode, false otherwise
 */
bool scancode_is_esc_break();

#endif //G06_KEYBOARD_H
