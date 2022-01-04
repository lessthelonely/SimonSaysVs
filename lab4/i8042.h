#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define ESC_BREAKCODE 0x81 //breakcode of the ESC key
#define ESC_MAKECODE 0x01 //makecode of the ESC key

#define KBC_IRQ 1 //KBC's IRQ line
#define MOUSE_IRQ 12 //Mouse's IRQ line

#define KBC_ST_REG 0x64 //Status Register - Read The KBC state
#define KBC_CMD_REG 0x64 //Input Buffer - Write Commands to the KBC 
#define KBC_ARG_REG 0x60 //Input Buffer - Write Arguments of KBC commands
#define KBC_OUT_BUF 0x60 //Output Bufferf
#define KBC_RBC_COMMAND 0x20 //Read Command Byte
#define KBC_WRT_COMMAND 0x60 //Write Command Byte
#define KBC_CHECK 0xAA //Check KBC
#define KB_INT_CHECK 0xAB //Check Keyboard
#define DIS_KBD_INT 0xAD //Disable KBD interface
#define ENB_KBD_INT 0xAE //Enable KBD interface
#define FB_OF_TB 0xE0 //First byte of two-byte long scancodes

#define CHECK_MOUSE_IFC 0xA9 //Check Mouse interface : returns 0 if OK 
#define DISABLE_MOUSE 0xA7  //Disable Mouse interface 
#define ENABLE_MOUSE 0xA8   //Enable Mouse interface 
#define WRITE_B_MOUSE 0xD4   //Write Command Byte
#define MOUSE_RESET 0xFF    //Reset mouse 
#define MOUSE_ENABLE_REPORT 0xF4  //Enable Mouse data reporting*/
#define MOUSE_DISABLE_REPORT 0xF5 //Disable Mouse data reporting
#define MOUSE_REMOTE_MODE 0xF0    //Enable Mouse Remote Mode 
#define MOUSE_STREAM_MODE 0xEA    //Enable Mouse Stream Mode 
#define MOUSE_READ_DATA 0xEB    //Read data from mouse
#define ACK 0xFA   //Acknowledged command or argument 
#define NACK 0xFE  //Invalid byte
#define ERROR 0xFC //Second consecutive invalid byte 
#define PK_LB BIT(0)  //Left button flag 
#define PK_RB BIT(1)  //Right button flag 
#define PK_MB BIT(2)  //Middle button flag 
#define PK_BIT3 BIT(3)  //Bit 3, always set on the first byte 
#define PK_XSIGN BIT(4)  //X delta sign 
#define PK_YSIGN BIT(5)  //Y delta sign 
#define PK_XOVF BIT(6)  //X overflow flag 
#define PK_YOVF BIT(7)  //Y overflow flag 

#define ENABLE_MOUSE_IR BIT(1) //Enable Mouse interrupts on OBF 
#define ENABLE_KBD_IR BIT(0)//Enable Keyboard interrupts on OBF

#define PAR BIT(7) //Parity error - invalid data
#define TIMEOUT BIT(6) //Timeout error - invalid data
#define AUX BIT(5) //Mouse data
#define INH BIT(4) //Inhibit flag: 0 if keyboard is inhibited
#define IBF BIT(1) //Input buffer full
#define OBF BIT(0) //Output buffer full

#define DELAY_US 20000 


#endif
