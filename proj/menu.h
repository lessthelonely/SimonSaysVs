#ifndef MENU_H
#define MENU_H
#include <lcom/lcf.h>
#include "vc.h"
#include "keyboard.h"
#include "i8042.h"
#include <stdbool.h>
#include <stdint.h>
#include "screen.h"

/** @defgroup menu menu
 * @{
 *
 * Functions for handling menu
 */


#define MENU_NUM_SELECTIONS 5 /**< @brief Number of possible arrow positions*/
/**
 * @brief a struct that saves all the graphics from the game. 
 */
extern Screen game;

/**
 * @brief Enums which screen should be printed, corresponding to the position of menu arrow.
 */
typedef enum
{
    SELECT_ONEPC,
    SELECT_TWOPC,
    SELECT_HOW_TO_PLAY,
    SELECT_LEADERBOARD,
    SELECT_EXIT
} Menu_Selection;

/**
 * @brief Keeps track which menu screen is begin displayed.
 */
Menu_Selection position; //keep track where we are in menu

/**
 * @brief Draws the menu background, complete with game logo and arrow that based on its position, if enter is pressed will take the user to another screen suggested by the name the user selected.
 */
void draw_menu_graphics();

/**
 * @brief Executes the move between menu screens tracking the press of the up arrow/w keys and the down arrow/s keys. It also switches from the menu screen to others depending which menu option the user selected by using the keyboard key enter.
 * @param key, with the information about the pressed key.
 */
void menu_update_status(struct key_press key);

#endif //MENU_H
