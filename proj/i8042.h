

#ifndef G06_I8042_H
#define G06_I8042_H

#include <lcom/lcf.h>
#include "utils.h"

/** @defgroup i8042 i8042
 * @{
 *
 * Constants for programming the i8042 KBD & Mouse.
 */

#define KBD_IRQ 1 /**< @brief Keyboard 1 IRQ line */
#define MOUSE_IRQ 12 /**< @brief Mouse 12 IRQ line */

#define KBC_OUT_BUF 0X60 /**< @brief KBC's Output Buffer Port.*/
#define KBC_ARG_REG 0x60 /**< @brief KBC's Argument Register.*/
#define KBC_CMD_REG 0x64 /**< @brief KBC's Command Register.*/
#define KBC_STAT_REG 0x64 /**< @brief KBC's Status Register.*/

//KBD 
#define KBC_STAT_REG_PAR BIT(7) /**< @brief Reading the status register - Parity.*/
#define KBC_STAT_REG_TIMEOUT BIT(6) /**< @brief Reading the status register - Timeout.*/
#define KBC_STAT_REG_IBF BIT(1) /**< @brief Reading the status register - Input buffer full.*/
#define KBC_STAT_REG_OBF BIT(0) /**< @brief Reading the status register - Output buffer full.*/


#define TWO_BYTE_SCANCODE_PREFIX 0xE0 /**<@brief Two byte scancode prefix.*/
#define ESC_BREAK_CODE 0x81 /**< @brief ESC breakcode.*/

#define DELAY_US 20000 /**<@brief Delay period.*/

//MOUSE

#define MOUSE_PACKET_SIZE 3 /**<@brief Size of the mouse packet.*/

#define KBD_STAT_REG_AUX BIT(5) /**<@brief Reading the status register - Mouse data.*/

#define BYTE_TO_MOUSE_CMD 0xD4 /**<@brief Write Byte to Mouse command.*/
#define BYTE_TO_MOUSE_SUCCESS 0xFA /**<@brief Acknowledgment byte - everything OK. */

#define MOUSE_DIS_DATA_REP 0xF5 /**<@brief Disable Data Reporting.*/
#define MOUSE_ENAB_DATA_REP 0xF4 /**<@brief Enable Data Reporting.*/

#define PACKET_Y_OVFL BIT(7) /**<@brief Bit representing Y Overflow in the mouse packet's byte 1.*/
#define PACKET_X_OVFL BIT(6) /**<@brief Bit representing X Overflow in the mouse packet's byte 1.*/
#define PACKET_Y_MSB BIT(5) /**<@brief Bit representing MSB Y delta in the mouse packet's byte 1.*/
#define PACKET_X_MSB BIT(4) /**<@brief Bit representing MSB X delta in the mouse packet's byte 1.*/
#define PACKET_MB BIT(2) /**<@brief Bit representing the Middle Button in the mouse packet's byte 1.*/
#define PACKET_RB BIT(1) /**<@brief Bit representing the Right Button in the mouse packet's byte 1.*/
#define PACKET_LB BIT(0) /**<@brief Bit representing the Left Button in the mouse packet's byte 1.*/


#endif //G06_I8042_H

