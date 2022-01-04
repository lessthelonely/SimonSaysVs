//
// Created by henri on 20/10/2020.
//

#ifndef G06_I8042_H
#define G06_I8042_H

#include <lcom/lcf.h>
#include "utils.h"

#define KBD_IRQ 1/**< @brief Keyboard 1 IRQ line */
#define MOUSE_IRQ 12

#define KBC_OUT_BUF 0X60
#define KBC_ARG_REG 0x60
#define KBC_CMD_REG 0x64
#define KBC_STAT_REG 0x64

//KBD
#define KBC_STAT_REG_PAR BIT(7)
#define KBC_STAT_REG_TIMEOUT BIT(6)
#define KBC_STAT_REG_IBF BIT(1)
#define KBC_STAT_REG_OBF BIT(0)


#define TWO_BYTE_SCANCODE_PREFIX 0xE0
#define ESC_BREAK_CODE 0x81

#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0x60

#define CMD_BYTE_ENABLE_KBD_INTERRUPT BIT(0)
#define CMD_BYTE_ENABLE_MOUSE_INTERRUPT BIT(1)

#define DELAY_US 20000

//MOUSE

#define MOUSE_PACKET_SIZE 3

#define KBD_STAT_REG_AUX BIT(5)

#define BYTE_TO_MOUSE_CMD 0xD4
#define BYTE_TO_MOUSE_FAILED 0xFE
#define BYTE_TO_MOUSE_SUCCESS 0xFA

#define MOUSE_DIS_DATA_REP 0xF5
#define MOUSE_ENAB_DATA_REP 0xF4
#define MOUSE_GET_CONF 0xE9

#define PACKET_Y_OVFL BIT(7)
#define PACKET_X_OVFL BIT(6)
#define PACKET_Y_MSB BIT(5)
#define PACKET_X_MSB BIT(4)
#define PACKET_MB BIT(2)
#define PACKET_RB BIT(1)
#define PACKET_LB BIT(0)



#endif //G06_I8042_H
