#ifndef MP_GAME_H
#define MP_GAME_H

#include <lcom/lcf.h>
#include "consts.h"
#include "keyboard.h"
#include "screen.h"
#include "game.h"
#include "mouse.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

/** @defgroup mp_game mp_game
 * @{
 *
 * Functions for handling mp_game
 */

#define TIMER_BUTTON_MP 12 /**< @brief Seconds button will stay lit up*/

/**
 * @brief Enumeration that stores all possible UART commands/situation
*/
typedef enum
{
    ACK,
    ERROR,
    CONNECTED,
    GREEN_REMOTE,
    YELLOW_REMOTE,
    PURPLE_REMOTE,
    BLUE_REMOTE,
    RED_REMOTE,
    WHITE_REMOTE,
    IS_VS,
    IS_NOT_VS,
    READY, // The other player can play
    LOSE,
    TIE_REMOTE
} coms;

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
static struct mouse_state mp_mouse; /** <Uses the functions from game.c, hence needs to use that external variable */

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
 * @brief If the computer is a host
 */
static bool is_host;

/**
 * @brief The number of buttons that the player playing has correctly guessed
 */
static int guessed;

/**
 * @brief Time remaining to perform the action (next button in the sequece or vs action)
 */
static int time_left_for_click;

/**
 * @brief Time remaining on the screen that is being displayed
 */
static int time_left_for_screen;

/**
 * @brief Time remaining for the button to stay lit up
*/
static int timer_counter;

/**
 * @brief If the game has ended
 */
static bool game_end;

/**
 * @brief If the VS button was already activated
 */
static bool VS_was_pressed;

/**
 * @brief If the VS screen is active
 */
static bool VS_screen;

/**
 * @brief If we are in a round when the VS button can be activated
 */
static bool is_vs_round;

/**
 * @brief If the game has started
 */
static bool game_started;

/**
 * @brief Data to send to the uart after the uart thr is empty
 */
static uint8_t uart_data;

/**
 * @brief Uart has data pending
 */
static bool uart_has_data_to_send;

/**
 * @brief Prints the whole game screen
 */
void mp_draw_game_graphics();

/**
 * @brief Prints the right winner or tie after the game ends
 */
void mp_draw_winner_graphics();

/**
 * @brief Prints the game screen with the right button lit up
 */
void mp_draw_buttons();

/**
 * @brief Prints which player's turn it is
 */
void mp_draw_player();

/**
 * @brief Prints the time left to perform the next click
 */
void mp_draw_counter();

/**
 * @brief Prints the mouse on the screen
 */
void mp_draw_mouse();

/**
 * @brief Prints the number of the round on the screen (aka the number of buttons stored)
 */
void mp_draw_round();

/**
 * @brief Updates the status of the mouse_state and deals with mouse clicks
 */
void mp_game_update_status_mouse(struct packet mouse_packet);

/**
 * @brief Gets which button was clicked
 * @return An enum type with the button clicked
 */
button_click_t mp_get_button_click();

/** 
 * @brief Handles the button input, may it be from the remote player or from the  mouse
 */
void mp_buttons_handler(button_click_t click);

/**
 * @brief Handles the logic of click a button
 * @details Checks if the player click the right sequence, or adds a new button if that's the case. Ends the game if there was a mistake or reached maximum number of buttons
 */
void mp_click_button(button_click_t click);

/**
 * @brief Handles the logic when the other player clicks the button
 */
void mp_remote_click_button(button_click_t click);

/**
 * @brief Lights the correct button (or displays the VS activated screen) given a mouse click
 */
void mp_light_button(button_click_t click);

/**
 * @brief Handles the keyboard interruptions
 * @details After games end, on enter click, takes you to the next screen, which can be the leaderboard or the highscore
 */
void mp_game_update_status_key(struct key_press key);

/**
 * @brief Handles the timer interruptions
 * @details Resets mouse left click
 */
void mp_game_update_status_timer();

/**
 * @brief Handles the rtc interruptions
 * @details Determines if the VS action has run out of time or if the player has run out of time to press the next button
 */
void mp_game_update_status_rtc();

/**
 * @brief Handles the uart interruptions
 */
void mp_game_update_status_uart(uint8_t data, uint8_t type);

/**
 * @brief Resets the state of the game to what it was at the first execution
 * @param num_rounds The maximum number of rounds
 * 
 */
void host_mp_game(int num_rounds);

/**
 * @brief Connects to the other pc to play the 2pc game
 */
void connect_mp_game();

/**
 * @brief Gets the data to start the game as a host or a the client
 * @param data The data from the uart interrupt
 * @param empty_thr If the interrupt was a emtpy thr interrupt
 */
void start_game(uint8_t data, bool empty_thr);

/**
 * @brief Handles the game logic upon receival of a data byte
 * @param data Data passem from the remote player
 */
void play_game(uint8_t data);

/**
 * @brief Resets the state of the game to what it was at the first execution
 * @param num_rounds The maximum number of rounds
 * 
 */
void mp_reset_game(int num_rounds);

/**
 * @brief Removes the memory allocated for saving the order of the buttons
 */
void mp_end_game();

/**
 * @brief Sends the winner to the other player (tie, player 1 or player2)
 */
void mp_send_winner();

/**
 * @brief Sends the button to the other player
 * @details Sends the button when the player is still making it throught the sequence
 */
void mp_send_button(button_click_t click);

/**
 * @brief Adds the button to the list of buttons, sending it to the other player
 * @param click The click that we are going to send over comms and that we are going to add to the sequence
 */
void mp_add_button(button_click_t click);

/**
 * @brief Sends the data to the other player so he can play
 */
void mp_change_player();

/**
 * @brief Checks if the mouse click was on the red button
 * @return True if clicked the button, false otherwise
 */
bool mp_get_red_click();

/**
 * @brief Checks if the mouse click was on the green button
 * @return True if clicked the button, false otherwise
 */
bool mp_get_green_click();

/**
 * @brief Checks if the mouse click was on the purple button
 * @return True if clicked the button, false otherwise
 */
bool mp_get_purple_click();

/**
 * @brief Checks if the mouse click was on the blue button
 * @return True if clicked the button, false otherwise
 */
bool mp_get_blue_click();

/**
 * @brief Checks if the mouse click was on the yellow button
 * @return True if clicked the button, false otherwise
 */
bool mp_get_yellow_click();

/**
 * @brief Checks if the mouse click was on the vs/white button
 * @return True if clicked the button, false otherwise
 */
bool mp_get_vs_click();

/**
 * @brief Prints a number indicating it's the player 1 turn to play
 */
void mp_print_player1();

/**
 * @brief Prints a number indicating it's the player 2 turn to play
 */
void mp_print_player2();

/**
 * @brief Uses a random to determine if the next round is a VS round
 */

void mp_is_VS_round();

/** 
 * @brief Checks which player won or if the game ended in a tie and shows screen accordingly (using enum Winners).
*/
void mp_get_winner();

/**
 * @brief A debugging function that prints an image on the screen in order to flag the state of the program since it's impossible to use the console at the same time as graphics mode
 * @param i The y displacement of the debugging image
 */
void mp_debugging(int i);
/** 
 * @brief Checks if the winner's score is higher than the lowest highscore in the leaderboard (uses getLowestHiscore() function from leaderboard.c). If yes, then the screen swtiches to the HISCORE screen so the winner can input their name to be displayed in the leaderboard. If not, the screen simply switches to leaderboard so the players can see the results of previous matches.
*/
void mp_checkNewHiScore();

/**
 * @brief When THR isn't empty, data is stored in a queue for posterior reading.
*/
void uart_set_queue(uint8_t data);

/**
 * @brief When THR is empty, the UART can send the data from the queue to the other machine.
*/ 
void uart_ready();

#endif
