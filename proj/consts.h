#ifndef CONSTS_H
#define CONSTS_H

/* POSITIONS OF THE BUTTONS' CENTERS */
#define GREEN_X 218  /**< @brief Horizontal position of the green button's center. */
#define GREEN_Y 218  /**< @brief Vertical position of the green button's center. */
#define BLUE_X 507   /**< @brief Horizontal position of the blue button's center. */
#define BLUE_Y 413   /**< @brief Vertical position of the blue button's center. */
#define PURPLE_X 293 /**< @brief Horizontal position of the purple button's center. */
#define PURPLE_Y 413 /**< @brief Vertical position of the purple button's center. */
#define YELLOW_X 583 /**< @brief Horizontal position of the yellow button's center. */
#define YELLOW_Y 225 /**< @brief Vertical position of the yellow button's center. */
#define RED_X 400    /**< @brief Horizontal position of the red button's center. */
#define RED_Y 92     /**< @brief Vertical position of the red button's center. */
#define VS_X 400     /**< @brief Horizontal position of the VS button's center. */
#define VS_Y 260     /**< @brief Vertical position of the VS button's center. */

/* GRAPHICS' SIZE RELATED CONSTANTS */
#define BUTTON_WIDTH 89 /**< @brief Button radius. */
#define MOUSE_WIDTH 42  /**< @brief Horizontal size of the mouse cursor's graphic. */
#define MOUSE_HEIGHT 44 /**< @brief Vertical size of the mouse cursor's graphic. */

/* RTC RELATED CONSTANTS */
#define INTERVAL 3 + 1     /**< @brief 3 seconds (plus in mid second tolerance) for each button press.  */
#define VS_INTERVAL 5 + 1  /**< @brief 5 seconds  (plus in mid second tolerance)  for lucky VS round. */

/**
 * @brief Enums which screen should be printed, corresponding to the button that is being pressed
 */
typedef enum
{
    DEFAULT,
    GREEN,
    YELLOW,
    PURPLE,
    BLUE,
    RED,
    VS,
    WHITE
} Play_Screens;

/**
 * @brief Enums the button that was pressed
 */
typedef enum
{
    GREEN_B,
    YELLOW_B,
    PURPLE_B,
    BLUE_B,
    RED_B,
    VS_B,
    NO_BUTTON
} button_click_t;

/**
 * @brief Enums the end state of the game
 */
typedef enum
{
    WINNER1,
    WINNER2,
    TIE
} Winners;

#endif //CONSTS_H
