#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <lcom/lcf.h>
#include "vc.h"
#include "keyboard.h"
#include "i8042.h"
#include <string.h>
#include "screen.h"
#include "rtc.h"

/** @defgroup highscore highscore
 * @{
 *
 * Functions for handling highscore
 */

/**
 * @brief a struct that saves all the graphics from the game. 
*/
extern Screen game;

/**
 * @brief Struct used as a text box for input of a player's name.
 * @details Check the data structures or vc.h for it. 
*/ 
static struct text_box new_hiscore;

/**
 * @brief A C string (char array, length 11) for a player's name.
*/
static char player[11];

/**
 * @brief A struct used as intermediate of any new highscore obtained.
 * @details See leaderboard.h or data structures for the parameters definition.
 */
extern cont_winner newWinner;

/**
 * @brief Draws the highscore screen, which appears everytime there's a new highscore, to register the winner's name.
 * @details Used the text_box struct.
*/ 
void draw_highscore_graph();

/**
 * @brief Controls input in the highscore text box.
 * @details Backspace deletes a character, numbers and letters are printed in the textbox, enter confirms the input, which shows the Leaderboard.
 * @param key, with the information about the pressed key.
*/ 
void highscore_update_status(struct key_press key);

/**
 * @brief Calls create_name_box() function, should be done everytime we enter the highscore screen, in order to ensure we aren't writing into an already written textbox.
 * @param score The new hiscore;
*/ 
void reset_hiscore(int score);

/**
 * @brief Creates the text box for the player name's input.
*/ 
void create_name_box();

/**
 * @brief Returns the inputted player's name.
 * @return The player's name in a char array (C String, pointer for the first element).
*/
char* getPlayerName();

/**
 * @brief Transforms a date struct instance into a C string (char array).
 * @return The resulting char array (pointer for the first element).
*/
char* getDate();


#endif //HIGHSCORE_H
