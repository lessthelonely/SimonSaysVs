#ifndef NUMBER_OF_ROUNDS_H
#define NUMBER_OF_ROUNDS_H
#include <lcom/lcf.h>
#include "vc.h"
#include "keyboard.h"
#include "i8042.h"
#include "screen.h"
#include <stdbool.h>
#include <stdint.h>

/** @defgroup number_of_rounds number_of_rounds
 * @{
 *
 * Functions for handling num_of_rounds
 */

/**
 * @brief a struct that saves all the graphics from the game. 
*/
extern Screen game;

/**
 * @brief Struct used as a text box for input of the number of rounds intended to play by the players.
 * @details Check the data structures or vc.h for it. 
*/
static struct text_box num_rounds;

/**
 *@brief Unsigned int where the value of the number of rounds is store
 *
*/
static unsigned int num_rounds_value;

/**
 * @brief If the game is multiplayer or not
 */
static bool is_mp;

/**
 * @brief Draws the number of rounds screen, which appears everytime a new game is started.
 * @details Used the text_box struct.
*/
void draw_num_of_rounds_graphics();

/**
 * @brief Controls input in the number_of_rounds text box.
 * @details Backspace deletes a character, numbers are printed in the textbox, enter confirms the input, screen switches to start of the game (shows default board game).
 * @param key, with the information about the pressed key.
*/
void number_of_rounds_update_status(struct key_press key);

/**
 * @brief Calls create_num_box() function, should be done everytime we enter the number_of_rounds screen, in order to ensure we aren't writing into an already written textbox.
 * @param If we are playing on 1 computer or in 2
*/
void reset_number_of_rounds(bool mp);

/**
 * @brief Creates the text box for the players number of rounds' input.
*/
void create_num_box();

#endif //NUMBER_OF_ROUNDS_H
