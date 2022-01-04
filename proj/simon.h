#ifndef SIMON_H
#define SIMON_H

#include "mouse.h"
#include "keyboard.h"
#include "timer.h"
#include "screen.h"

/** @defgroup simon simon
 * @{
 *
 * Functions for handling simon
 */

/**
 * @brief Struct to process mouse operations (clicks, movement... et cetera).
 * @details Provided by LCOM teacher.
*/
static struct packet mouse_packet;

/**
 * @brief Stores the data from a keypress.
 * @details Designed by us, definition done in keyboard.h. Also seeable in data structures page.
*/
static struct key_press keyboard_packet;

/**
 * @brief Stores the data from the uart
 */
static uint8_t data_packet;

/**
 * @brief Boolean value to know whether a mouse packet has been completed or not.
*/
static bool got_mouse_packet;

/**
 * @brief Boolean value to know whether a key press on the keyboard has been processed or not.
*/
static bool got_key_press;

/**
 * @brief Boolean value to know whether a timer interrupt has been processed or not.
*/
static bool timer_interrupt;

/**
 * @brief Boolean value to know whether a rtc interrupt has been processed or not.
 */
static bool rtc_interrupt;

/** 
 * @brief Boolean value to know wheter a uart interrupt has been processed or not.
 */
static bool uart_interrupt;

/**
 * @brief Type of interrupt the uart received
 */
static uint8_t uart_type_interrupt;

/**
 * @brief Keeps Screen information.
 * @details See definition in screen.h or the data structures page.
*/
Screen game;

/**
 * @brief Variable used to force the game to ignore the first enter it detects.
 * @details Necessary because when running the game, it registers the enter used to confirm the "lcom_run proj" command in Minix.
*/
static int count = 0;

/**
 * @brief Function for the game's setup and main execution control.
 * @details Sets graphic mode, loads all graphics and screens, subscribes interruptions, calls event_handler, unsubscribes interruptions, sets text mode.
*/
void(run)();

/**
 * @brief Function that emulates the game's state machine.
 * @details Switches from the game's screens, calling any functions needed.
*/
bool event_handler();
#endif //SIMON_H
