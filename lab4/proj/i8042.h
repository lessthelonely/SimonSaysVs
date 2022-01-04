#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define ESC_BREAKCODE 0x81 //breakcode of the ESC key
#define ESC_MAKECODE 0x01 //makecode of the ESC key
#define KBC_IRQ 1 //KBC's IRQ line
#define KBC_ST_REG 0x64 //Status Register - Read The KBC state
#define KBC_CMD_REG 0x64 //Input Buffer - Write Commands to the KBC 
#define KBC_ARG_REG 0x60 //Input Buffer - Write Arguments of KBC commands
#define KBC_OUT_BUF 0x60 //Output Buffer
#define KBC_RBC_COMMAND 0x20 //Read Command Byte
#define KBC_WRT_COMMAND 0x60 //Write Command Byte
#define KBC_CHECK 0xAA //Check KBC
#define KB_INT_CHECK 0xAB //Check Keyboard
#define DIS_KBD_INT 0xAD //Disable KBD interface
#define ENB_KBD_INT 0xAE //Enable KBD interface
#define FB_OF_TB 0xE0 //First byte of two-byte long scancodes
#define PAR BIT(7) //Parity error - invalid data
#define TIMEOUT BIT(6) //Timeout error - invalid data
#define AUX BIT(5) //Mouse data
#define INH BIT(4) //Inhibit flag: 0 if keyboard is inhibited
#define IBF BIT(1) //Input buffer full
#define OBF BIT(0) //Output buffer full
#define DELAY_US 20000 


#endif
