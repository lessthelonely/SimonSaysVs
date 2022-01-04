#ifndef HOST_H
#define HOST_H
#include <lcom/lcf.h>
#include "vc.h"
#include "keyboard.h"
#include "i8042.h"
#include <stdbool.h>
#include <stdint.h>
#include "screen.h"

/** @defgroup host host
 * @{
 *
 * Functions for handling connection between PCs
 */


#define HOST_NUM_SELECTIONS 2 /**< @brief Number of possible arrow positions*/
/**
 * @brief a struct that saves all the graphics from the game. 
 */
extern Screen game;

/**
 * @brief Enums which screen should be printed, corresponding to the position of menu arrow.
 */
typedef enum
{
    HOST_SELECT_HOST,
    HOST_SELECT_CONNECT
} Host_Selection;

/**
 * @brief Keeps track which host screen is begin displayed.
 */
Host_Selection host_position;

/**
 * @brief Draws the host screen, with the two available options
 */
void draw_host_graphics();

/**
 * @brief Executes the move between host screens tracking the press of the up arrow/w keys and the down arrow/s keys. On ENTER key, will either host a server (being prompted the number of rounds) or connect to a server
 * @param key, with the information about the pressed key.
 */
void host_update_status(struct key_press key);

#endif //HOST_H
