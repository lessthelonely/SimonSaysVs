#ifndef SCREEN_H
#define SCREEN_H
#include <lcom/lcf.h>
#include "graphs.h"
#include <stdbool.h>
#include <stdint.h>

/** @defgroup screen screen 
 * @{
 *
 * Functions for handling screen
 */

/**
 * @brief Enums the various modes of the game, based on what screen appears.
 */
typedef enum
{
    MENU,
    HOW_TO_PLAY,
    LEADERBOARD,
    NUM_ROUNDS,
    GAME,
    HIGHSCORE,
    HOST,
    MP_GAME,
    EXIT
} Game_Modes;

/**
 * @brief Keeps Screen information.
 */
typedef struct Screen
{
    /**
 * @brief Every single game screen stored in xpm_image_t array.
 */
    xpm_image_t s[26];
/**
 * @brief Keeps the current game mode.
 */
    Game_Modes mode;
} Screen;

/**
 * @brief Load all xpm images of the screen and create object Screen
 * @return Said object Screen
 * 
 */
Screen loadscreens();

#endif //SCREEN
