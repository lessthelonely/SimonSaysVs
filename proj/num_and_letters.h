#ifndef NUM_AND_LETTERS
#define NUM_AND_LETTERS

#include <lcom/lcf.h>
#include "graphs.h"

/** @defgroup num_and_letters num_and_letters
 * @{
 *
 * Functions for handling num_and_letters
 */

/**
 * @brief an array of xpm_image_t of all the 42x44 letters (plus empty used to delete characters and a space image). 
*/
static xpm_image_t letters[28];

/**
 * @brief an array of xpm_image_t of all the 42x44 numbers (0 to 9).
*/
static xpm_image_t numbers[10];

/**
 * @brief an array of xpm_image_t of all the 42x44 letters (plus a bar (/) image used in dates). 
*/
static xpm_image_t lilletters[27];

/**
 * @brief an array of xpm_image_t of all the 19x28 numbers (0 to 9).
*/
static xpm_image_t lilnumbers[10];

/**
 * @brief a struct that saves all the graphics from the game. 
*/
static xpm_image_t mouse_img;

/**
 * @brief Loads all numbers xpm images (42x44) into xpm_image_t array numbers
*/
void loadnums();

/**
 * @brief Loads all numbers xpm images (19x28) into xpm_image_t array lilnumbers
*/
void loadlilnums();

/**
 * @brief Loads all letters xpm images (42x44) into xpm_image_t array letters
 */
void loadlet();

/**
 * @brief Loads all numbers xpm images (19x28) into xpm_image_t lilletters
*/
void loadlilet();

/** 
 * @brief Given a char byte, searches through arrays letters and numbers in search of xpm image of said char.
 * @param char byte, which we want the corresponding xpm image
 * @return xpm_image_t of said char.
 */
xpm_image_t *get_prop_from_char(char byte);

/** 
 * @brief Given a char byte, searches through arrays lilletters and lilnumbers in search of xpm image of said char.
 * @param char byte, which we want the corresponding xpm image
 * @return xpm_image_t of said char.
 */
xpm_image_t *get_lilprop_from_char(char byte);

/**
 * @brief Checks if parameter byte is a digit or letter from the alphabet.
   @param byte, character which we would like to know if it's digit, letter from the alphabet or not.
   @return true if it's a digit, letter from the alphabet, false if not.
*/
bool is_alpha_numberical(char byte);

/**
 * @brief Checks if parameter byte is a digit.
   @param byte, character which we would like to know if it's digit or not.
   @return true if it's a digit, false if not.
*/
bool is_numerical(char byte);

/**
 * @brief Returns xpm image of "empty", which is used when user wants to erase the last character input, in the place where the character was, empty is placed.
   @return xpm_image_t "empty_xpm"
*/
xpm_image_t get_empty();

/**
 * @brief Loads mouse cursor xpm image.
*/
xpm_image_t *get_mouse();

#endif //NUM_AND_LETTERS
