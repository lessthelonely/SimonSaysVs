#ifndef MOUSE_H
#define MOUSE_H

#include <lcom/lcf.h>

/** @defgroup mouse mouse
 * @{
 *
 * Functions for handling mouse
 */

/**
 * @brief Keeps the state of the mouse
 */
struct mouse_state
{

    /**
     * @brief The x position of the mouse
     */
    uint16_t x_pos;

    /**
     * @brief The y position of the mouse
     */
    uint16_t y_pos;

    /**
     * @brief If the left button is being held
     */
    bool left_hold;

    /**
     * @brief If the left button has been clicked
     */
    bool left_click;
};

/**
 * @brief Subscribes mouse interruptions
 * @param bit_no Return argument with the mask bit
 * @param notification Return argument with the notification address
 */
int(mouse_subscribe)(uint8_t *bit_no, int *notification);

/**
 * @brief Unsubscribes mouse interruptions
 * @param notification The correspondent mouse notification address
 */
int (mouse_unsubscribe)(int *notification);

/**
 * @brief Enables data reporting
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_data_reporting_enable();

/**
 * @brief Disables data reporting
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_data_reporting_disable();

/**
 * @brief Checks the mouse state
 * @details Checks if there was an error, if the input or output buffers are full and if there is a pending byte from th emouse
 */
void (mouse_get_state)();


/**
 * @brief Handles the mouse interruption and tries to assemble a mouse packet
 * @param mouse_packet Return argument with the mouse packet
 * @return True if completed the mouse packet, false otherwise
 */
bool mouse_handler(struct packet *mouse_packet);

/**
 * @brief Tries to perform an operation on the mouse through the kbc
 * @param command The command give to the mouse
 * @param args The arguments of the command
 * @param args_size The number of arguments of the command
 * @param ret_arg Output argument with the return value of the command
 * @param has_ret If the command has a return value
 * @return True if successful, false otherwise
 */
bool try_mouse_operation(uint8_t command, uint8_t *args, int args_size, uint8_t *ret_arg, bool has_ret);

/**
 * @brief Writes a command to the mouse
 * @param command The command give to the mouse
 * @param args The arguments of the command
 * @param args_size The number of arguments of the command
 * @param ret_arg Output argument with the return value of the command
 * @param has_ret If the command has a return value
 * @return True if successful, false otherwise
 */
bool mouse_write_command(uint8_t command, uint8_t *args, int args_size, uint8_t *ret_arg, bool has_ret);


/**
 * @brief Writes a byte to the mouse
 * @param byte The byte to be written to the mouse
 * @return True if successful, false otherwise
 */
bool mouse_write_byte(uint8_t byte);

/**
 * @brief Receives the acknowledgment byte from the mouse after issuing a command or argument
 * @return True if the acknowledgment indicates success, false otherwise
 */
bool mouse_write_byte_ack();

/**
 * @brief Assembles a mouse packet from the bytes which have its data
 * @param bytes The bytes from the mouse device
 * @return The mouse packet
 */
struct packet assembly_packet(uint8_t *bytes);

/**
 * @brief Checks if the mouse is synched with the mouse_handler 
 * @details Checks whether or not the number of the byte being checked is the same that is being sent
 * @return True if is not unsynched
 */
bool mouse_is_synched(uint8_t *bytes, int size);


#endif 
