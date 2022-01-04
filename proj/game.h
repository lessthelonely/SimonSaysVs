#ifndef GAME_H
#define GAME_H

#include <lcom/lcf.h>
#include "consts.h"
#include "keyboard.h"
#include "screen.h"
#include "mouse.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

/** @defgroup game game 
 * @{
 *
 * Functions for game logic.
 */

/**
 * @brief a struct that saves all the graphics from the game. 
*/
extern Screen game;

/**
 * @brief Stores the winner of the game
 */
static Winners ws;

/**
 * @brief The buttons that were pressed during the game
 */
static button_click_t *buttons;

/**
 * @brief The button that is being lit up
 */
static Play_Screens ps;

/**
 * @brief The state of the mouse in the game
 */
static struct mouse_state mouse;

/**
 * @brief The current number of buttons that make the sequence
 */
static int buttons_size;

/**
 * @brief The score obtained in this game
 * @details The biggest amount of buttons a player memorized
 */
static int player_score;

/**
 * @brief The max size of the buttons stored in the sequece
 */
static int max_buttons_size;

/**
 * @brief The player that is playing
 * @details True if is the player 1 playing, false if is the player 2
 */
static bool is_player_1;

/**
 * @brief The number of buttons that the player playing has correctly guessed
 */
static int guessed;

/**
 * @brief Time remaining to perform the action (next button in the sequece or vs action)
 */
static int time_left_for_click;

/**
 * @brief If the game has ended
 */
static bool game_end;

/**
 * @brief If the VS button was already activated
 */
static bool VS_was_pressed;

/**
 * @brief If we are in a round when the VS button can be activated
 */
static bool is_vs_round;

/**
 * @brief Prints the whole game screen
 */
void draw_game_graphics();

/**
 * @brief Prints the right winner or tie after the game ends
 */
void draw_winner_graphics();

/**
 * @brief Prints the game screen with the right button lit up
 */
void draw_buttons();

/**
 * @brief Prints which player's turn it is
 */
void draw_player();

/**
 * @brief Prints the time left to perform the next click
 */
void draw_counter();

/**
 * @brief Prints the number of the round on the screen (aka the number of buttons stored)
 */
void draw_round();

/**
 * @brief Updates the status of the mouse_state and deals with mouse clicks
 */
void game_update_status_mouse(struct packet mouse_packet);

/**
 * @brief Gets which button was clicked
 * @return An enum type with the button clicked
 */
button_click_t get_button_click();

/**
 * @brief Handles the logic of click a button
 * @details Checks if the player click the right sequence, or adds a new button if that's the case. Ends the game if there was a mistake or reached maximum number of buttons
 */
void click_button(button_click_t click);

/**
 * @brief Lights the correct button (or displays the VS activated screen) given a mouse click
 */
void light_button(button_click_t click);

/**
 * @brief Handles the keyboard interruptions
 * @details After games end, on enter click, takes you to the next screen, which can be the leaderboard or the highscore
 */
void game_update_status_key(struct key_press key);

//////////////
/**
 * @brief Handles the timer interruptions
 * @details Resets mouse left click
 */
void game_update_status_timer();
//////////////


/**
 * @brief Handles the rtc interruptions
 * @details Determines if the VS action has run out of time or if the player has run out of time to press the next button
 */
void game_update_status_rtc();

/**
 * @brief Prints the mouse on the screen
 */
void draw_mouse();

/**
 * @brief Resets the state of the game to what it was at the first execution
 * @param num_rounds The maximum number of rounds
 * 
 */
void reset_game(int num_rounds);

/**
 * @brief Removes the memory allocated for saving the order of the buttons
 */
void end_game();

/**
 * @brief Checks if the mouse click was on the red button
 * @return True if clicked the button, false otherwise
 */
bool get_red_click();

/**
 * @brief Checks if the mouse click was on the green button
 * @return True if clicked the button, false otherwise
 */
bool get_green_click();

/**
 * @brief Checks if the mouse click was on the purple button
 * @return True if clicked the button, false otherwise
 */
bool get_purple_click();

/**
 * @brief Checks if the mouse click was on the blue button
 * @return True if clicked the button, false otherwise
 */
bool get_blue_click();

/**
 * @brief Checks if the mouse click was on the yellow button
 * @return True if clicked the button, false otherwise
 */
bool get_yellow_click();

/**
 * @brief Checks if the mouse click was on the vs/white button
 * @return True if clicked the button, false otherwise
 */
bool get_vs_click();

/**
 * @brief Prints a number indicating it's the player 1 turn to play
 */
void print_player1();

/**
 * @brief Prints a number indicating it's the player 2 turn to play
 */
void print_player2();

/**
 * @brief Uses a random to determine if the next round is a VS round
 */

void is_VS_round();

/** 
 * @brief Checks which player won or if the game ended in a tie and shows screen accordingly (using enum Winners).
*/
void get_winner();

int get_max_buttons_size();

/** 
 * @brief Checks if the winner's score is higher than the lowest highscore in the leaderboard (uses getLowestHiscore() function from leaderboard.c). If yes, then the screen swtiches to the HISCORE screen so the winner can input their name to be displayed in the leaderboard. If not, the screen simply switches to leaderboard so the players can see the results of previous matches.
*/
void checkNewHiScore();

/** 
 * @brief Converts number of rounds to char and returns it to be used in leaderboard.c.
 * @return Number of rounds converted to char.
*/
char *getScore();

/**
 * @brief A debugging function that prints an image on the screen in order to flag the state of the program since it's impossible to use the console at the same time as graphics mode
 * @param i The y displacement of the debugging image
 */
void debugging(int i);

#endif //GAME_H
