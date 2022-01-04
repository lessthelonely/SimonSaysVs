#ifndef VBE_H
#define VBE_H
#include <lcom/lcf.h>

#define INTNO 0x10

//VBE_CALL_REG 0xAH
#define VBE_CALL_REG_VAL 0x4F 
//VBE_FUNCTION_REG 0xAL
//VBE_FUNCTOIN_CALLBACK 0xAX

// AH register return
#define VBE_SUCCESS 0x00
#define VBE_FUNCTION_CALL_FAILED 0x01
#define VBE_HW_DOESNT_SUPPORT 0x02
#define VBE_INVALID_VIDEO_MODE 0x03

// Functions
#define VBE_CONTROLLER_INFO 0x00
#define VBE_MODE_INFO 0x01
//VBE_SET_MODE_REG 0xBX
#define VBE_SET_MODE 0x02

// Video modes
#define VBE_TEXT 0x03
#define VBE_1024 0x105
#define VBE_640 0x110
#define VBE_800  0x115
#define VBE_1280 0x11A
#define VBE_1152 0x14C

#define SET_LINEAR BIT(14)

#endif //VBE_H
