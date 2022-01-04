#ifndef _LCOM_VC_H_
#define _LCOM_VC_H_

#include <lcom/lcf.h>
#include "menu.h"

/** @defgroup vc vc
 * @{
 *
 * Functions for handling vc
 */

#define INT 0x10             /**< @brief VC IRQ line number. */
#define VBE 0x02             /**< @brief VBE call -- set VBE mode.*/
#define MAX_TEXT_BOX_SIZE 20 /** <@brief Size of the text box.*/

#define WIDTH 800  /**< @brief Value of the width of the screen mode.*/
#define HEIGHT 600 /**< @brief Value of the height of the screen mode.*/

/** 
 * @brief Value of bits per pixel of the mode.
*/
unsigned int bppixel;

/** 
 * @brief X Resolution (Width) of the mode.
*/
unsigned int x_res;

/** 
 * @brief Y Resolution (Height) of the mode.
*/
unsigned y_res;

/** 
 * @brief Color mode of the mode.
*/
unsigned color_mode;

/** 
 * @brief Value of bytes per pixel of the mode.
*/
unsigned int bytes_per_pixel;

/** 
 * @brief VRAM's physical address.
*/
static unsigned int vram_base;

/** 
 * @brief VRAM's size.
*/
static unsigned int vram_size;

/** 
 * @brief Struct vbe_mode_info given by the teacher, Packed VBE Mode Info Block.
*/
vbe_mode_info_t mode_info;

/** 
 * @brief Struct reg86 given by the teacher.
*/
struct reg86 r;

/** 
 * @brief Frame-buffer VM address (Primary buffer).
*/
static void *video_mem;

/** 
 * @brief Secondary buffer.
*/
void *buffer;

/**
 * @brief Text box structure to fill with letters or numbers desired.
 */
struct text_box
{
     /**
     * @brief xpm_image_t array with all the xpm_image_t that are a part of the word/number inside the text box.
     */
     xpm_image_t *char_prop[MAX_TEXT_BOX_SIZE];

     /**
     * @brief Initial coordinate x of the text box.
     */
     uint16_t xi;

     /**
     * @brief Initial coordinate y of the text box.
     */
     uint16_t yi;

     /**
     * @brief Maximum x until which a box will expand or accept letters to (towards the right side of the screen).
     */
     uint16_t max_x;

     /**
     * @brief Maximum x until which a box will expand or accept letters to (towards the left side of the screen).
     */
     uint16_t min_x;

     /**
     * @brief Current x coordinate
     */
     uint16_t cursor_x; // Calculatable but rather have them stored as well

     /**
     * @brief  Number of characters that are in the textbox at the moment.
     */
     int char_size;

     /**
     * @brief Maximum size of the text box.
     */
     int char_max_size;
     // We have a max_x and and a char_max_size since not all images might have the same size, and allows us not to calculate the max pixel width manually
};

/**
     * @brief Our version of the function vbe_get_mode_info(), provided by the teacher. This function retrieves information about the input VBEmode, including the physical address of the frame buffer.
     * @param mode mode whose information should be returned 
     * @param vmi_p address of vbe_mode_info_t structure to be initialized 
     * @return 0 on success, non-zero otherwise 
*/
int vbe_get_mode_inf(uint16_t mode, vbe_mode_info_t *vmi_p);

/**
     * @brief Size of the text box.
     * @param mode mode whose information should set.
     * @return 0 on success, non-zero otherwise 
*/
int set_vbe_mode(uint16_t mode);

/**
     * @brief Get mode information and maps memory.
     * @param mode mode whose information should be returned and processed.
     * @return 0 on success, non-zero otherwise 
*/
int get_mode_info(uint16_t mode);

/**
     * @brief Returns width of the mode.
     * @return constant WIDTH.
*/
int get_mode_width();

/**
     * @brief Returns height of the mode.
     * @return constant HEIGHT.
*/
int get_mode_height();

/**
     * @brief Draws a pixel in coordinate specified by coordinates x and y, paints said pixel in color specified by parameter color and stores information in buffer.
     * @param x x coordinate of the screen where pixel should be drawn.
     * @param y y coordinate of the screen where pixel should be drawn.
     * @param color color which pixel should be filled with.
     * @return 0 on success, non-zero otherwise 
*/
int draw_pixel_mouse(uint16_t x, uint16_t y, uint32_t color);

/**
     * @brief Copies parameter screen to buffer using memcpy (implementation of double buffering).
     * @param screen background image.
     * @return 0 on success, non-zero otherwise 
*/
int draw_xpm(xpm_image_t screen);

/**
     * @brief Prints image specified in parameter screen in the coordinates of the screen specified by parameters x and y. Calls draw_pixel as auxilary function (draws image pixel by pixel and stores it in buffer).
     * @param screen image that should be printed.
     * @param x coordinate x of where image should be printed to.
     * @param y coordinate y of where image should be printed to.
     * @return 0 on success, non-zero otherwise 
*/
int draw_xpm_x_y(xpm_image_t screen, uint16_t x, uint16_t y);

/**
     * @brief Copy contents of buffer to video_mem. Sets buffer to zero.
*/
void copy_to_video();

/**
     * @brief Creates a writeable text box, a text box where the user can input information in.
     * @param cursor_x
     * @param cursor_y
     * @param min_x
     * @param max_x
     * @param max_size
     * @return 0 on success, non-zero otherwise 
*/
struct text_box create_writeable_text_box(uint16_t cursor_x, uint16_t cursor_y, uint16_t min_x, uint16_t max_x, int max_size);

/**
  * @brief Adds char specified in parameter byte to text box, by using the auxiliary function get_prop_from_char, which returns the xpm_image_t of said char.
  * @param byte char to be added to text box. 
  * @param box text box in question.
  * @return True if it's possible to add char, false if not.
*/
bool add_to_text_box(char byte, struct text_box *box);

/**
     * @brief Creates a static text box, a text box which serves to only display information, uses as an auxiliary function, add_to_text_box.
     * @param x coordinate x of the screen where text box should be printed.
     * @param y coordinate y of the screen where text box should be printed.
     * @param char_props char array of information to be added/printed in text box.
     * @param size size of char_props[] array.
     * @return struct text_box with information from char_props[] stored.
*/
struct text_box create_static_text_box(uint16_t x, uint16_t y, char char_props[], int size);

/**
     * @brief Draws text box specified in param box, using auxiliary function draw_xpm_x_y.
     * @param box text box that should be printed.
*/
void draw_text_box(struct text_box *box);

/**
     * @brief Checks if char byte can be added to text box specified in parameter box.
     * @param byte char we wish to add to text box.
     * @param box text box.
     * @return True if char can be added to text box, false otherwise.
*/
bool add_to_centered_text_box(char byte, struct text_box *box);

/**
     * @brief Deletes last char input in text box by putting the empty image in the place the char was, calls draw_xpm_x_y as an auxiliary function.
     * @param box text box in question.
*/
void erase_text_box(struct text_box *box);

/**
     * @brief Delete last char input in text box, calling erase_text_box as an auxiliary function.
     * @param box text box in question.
*/
void remove_from_centered_text_box(struct text_box *box);

#endif
