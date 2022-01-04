#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include "keyboard.h"
#include "screen.h"

/** @defgroup leaderboard leaderboard
 * @{
 *
 * Functions for handling leaderboard 
 */

#define SIZE 5 /**< @brief Maximum size of the leaderboard struct array.*/

/**
 * @brief A struct to save a Leaderboard-contemplated highscore.
*/
typedef struct
{
	/**
	 * @brief A C string (char array, length 11) with the date where a score was first obtained/registered.
	*/
	char date[11];

	/**
	 * @brief A C string (char array, length 11) with the name registered for the highscore's winner.
	*/
	char name[11];

	/**
	 * @brief Integer number for the score (for comparissons).
	*/
	int int_score;

} cont_winner;

/**
 * @brief "cont_winner" struct filled with zeros to fill empty spaces in the leaderboard.
*/
static cont_winner dummy;

/**
 * @brief "cont_winner" array that saves all the contemplated highscores, thus representing the game's leaderboard in full.
*/
static cont_winner leaderboard[SIZE];

/**
 * @brief "cont_winner" struct used as intermediate in the process of registering a new highscore.
*/
cont_winner newWinner;

/**
 * @brief a struct that saves all the graphics from the game. 
*/
extern Screen game;

/* //FOR MELISSA!
extern char s[3];
*/

// FUNCTIONS PERTAINING TO GRAPHICS.

/**
 * @brief Draws the leaderboard background and the players contemplated in it.
 * @details Works with a lowercase alphabet and background picture (all XPMs), drawn in fixed positions, using data from a .txt file.
 */
void draw_leaderboard_graphics();

/**
 * @brief Executes the return to the game's Main Menu through a backspace press.
 * @param key, with the information about the pressed key.
 * @details 
 */
void leaderboard_update_status(struct key_press key);

//FUNCTIONS PERTAINING TO THE LEADERBOARD DATA AND WINNERS.

/**
 * @brief Loads a dummy structure to be potentially used in the printing of the Leaderboard.
 * @details Uses a cont_winner struck filled with zeros.
 */
void loadDummySt();

/**
 * @brief Determines where a new highscore must be placed.
 * @details Tries to place the score in 1st place (0), last place (4) or in between the two scores where it fits best (1, 2, 3).
 * @return an integer number (0-4) to be used in an array.
 */
int getNewHiScorePos();

/**
 * @brief Updates the leaderboard data structure with a newly obtained highscore.
 * @details Always gets right of 1 old score to make space for the new one.
 */
void updateData();

/**
 * @brief Returns the score saved in the last position (5th place) of the leaderboard (struct array "leaderboard")-
 */
int getLowestHiScore();

/**
 * @brief Saves the leaderboard's data in a .txt file.
 * @details Unpacks the data from the leaderboard array into the .txt file with a specified format.
 */
void saveData();

/**
 * @brief Gets the data from "leaderboard.txt" and puts it into the leaderboard data structure.
 * @details Reads name, score and date of each score, saving it in the adequate position in the leaderboard struct array.
 */
void retrieveData();

#endif //LEADERBOARD_h
