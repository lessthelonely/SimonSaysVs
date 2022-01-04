#ifndef HOW_TO_PLAY_H
#define HOW_TO_PLAY_H

#include "keyboard.h"
#include "screen.h"

/** @defgroup how_to_play how_to_play
 * @{
 *
 * Functions for handling how to play
 */


/**
 * @brief a struct that saves all the graphics from the game. 
*/
extern Screen game;

/**
 * @brief Draws the how to play screen, which appears everytime the user selects the "HOW TO PLAY" option in the main menu.
*/
void draw_how_to_play_graphics();

/**
 * @brief Checks if the key pressed by user was the Backspace key, if so returns to menu.
 * @param key, with the information about the pressed key.
*/
void how_to_play_update_status(struct key_press key);

#endif //HOW_TO_PLAY_H
