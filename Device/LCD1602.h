/********************************************************
 * \file      : LCD1602.h
 * \brief     : The code is to provide a easier way to use LCD1602
 *
 * \copyright : Copyright Jiarun LIU
 * \date      : 2022/10/14
 * \author    : Jiarun LIU
 * \version   : V1.1
*********************************************************/

#ifndef LCD1602_H_
#define LCD1602_H_


#ifdef STM32L476xx
	#include "stm32l4xx_hal.h"
#endif

#define LCD1602_MAX_WRITTER_BUFFER_SIZE		256

#define LCD1602_Delay_ms(ms)	(HAL_Delay(ms))

//#define LCD1602_US_TIMER		// uncommit if define the LCD1602_Delay_us(us) function
//#define LCD1602_Delay_us(us)	()

//------------------LCD1602 Connection Mode------------------
#define LCD1602_MODE_RW_STATE	((uint8_t)(1 << 1)) //< 0: Connect RW  		; 1: Not Connect RW
#define LCD1602_MODE_DATA_STATE	((uint8_t)(1 << 2)) //< 0: 8 Pin Connection  ; 1: 4 Pin Connection
#define LCD1602_MODE_INIT_SET_1	((uint8_t)(1 << 3)) //< 0: the lcd1602 pin unset;
													//< 1: the lcd1602 pin set
#define LCD1602_MODE_INIT_SET_2	((uint8_t)(1 << 4)) //< 0: the lcd1602 pin unset;
													//< 1: the lcd1602 pin set
//----------------LCD1602 Connection Mode END------------------

//------------------LCD1602 R/W Mode------------------
#define LCD1602_WRITE_MODE		0
#define LCD1602_READ_MODE		1
//------------------LCD1602 R/W Mode------------------

//------------------LCD1602 RS Mode------------------
#define LCD1602_COMMAND_MODE	0
#define LCD1602_DATA_MODE		1
//------------------LCD1602 RS Mode END------------------


//---------------------------LCD1602 Commands List----------------------------------------------
#define LCD1602_COMMAND_CLEAR					((uint8_t)0x01) //< Clear Display and return cursor to home position

#define LCD1602_COMMAND_CURSOR_HOME				((uint8_t)0x02)	//< Return Cursor to home position(DDRAM Unchanged)

#define LCD1602_COMMAND_ENTRY_MODE  			((uint8_t)0x04)
#define LCD1602_COMMAND_ENTRY_MODE_CURSOR_DIR   ((uint8_t)0x02)	//< Set Cursor Move Direction
																//< 0: Decrement  ; 1: Increment
#define LCD1602_COMMAND_ENTRY_MODE_SHIFT		((uint8_t)0x01) //< 0: NO Display Shift
																//< 1: Have Display Shift

#define LCD1602_COMMAND_DISPLAY_MODE			((uint8_t)0x08) //< Set Module Display Mode
#define LCD1602_COMMAND_DISPLAY_ON_OFF			((uint8_t)0x04) //< 0: Display OFF; 1: Display ON
#define LCD1602_COMMAND_DISPLAY_CURSOR			((uint8_t)0x02) //< 0: Cursor OFF ; 1: Cursor ON
#define LCD1602_COMMAND_DISPLAY_CURSOR_BLINK    ((uint8_t)0x01) //< 0: Cursor Blink off;
																//< 1: Cursor Blink on

#define LCD1602_COMMAND_CURSOR_DISPLAY_MODE		((uint8_t)0x10)
#define LCD1602_COMMAND_CURSOR_SHIFT_ON			((uint8_t)0x08) //< 0: Cursor Move ; 1: Display Shift
#define LCD1602_COMMAND_CURSOR_SHIFT_DIR		((uint8_t)0x04) //< 0: Shift Left  ; 1: Shift Right

#define LCD1602_COMMAND_FUNCTION_SET			((uint8_t)0x20)
#define LCD1602_COMMAND_FUNCTION_DATA_LENGTH	((uint8_t)0x10) //< 0: 4 bit	   ; 1: 8 bit
#define LCD1602_COMMAND_FUNCTION_LINE_NUMBER	((uint8_t)0x08) //< 0: 1/8 Or 1/11 Duty (1 Line)
																//< 1: 1/16 Duty (2 Line)
#define LCD1602_COMMAND_FUNCTION_FONT			((uint8_t)0x04) //< 0: 5*7 dots	   ; 1: 5*19 dots

#define LCD1602_COMMAND_CGRAM_ADDR_SET			((uint8_t)0x40) //< bit 0-5 are CGMAR ADDRESS (Receive OR Write after setting)

#define LCD1602_COMMAND_DDRAM_ADDR_SET			((uint8_t)0x80) //< bit 0-6 are CGMAR ADDRESS (Receive OR Write after setting)

#define LCD1602_COMMAND_READBUSY_ADDR_COUNTER	((uint8_t)0x80) //< when in read mode
//---------------------------LCD1602 Commands List END----------------------------------------------


enum LCD1602_Pin_Name_e{
	LCD1602_RS_PIN  = 0,	//< 0: Command   ; 1: Data
	LCD1602_RW_PIN  = 1,    //< 0: Write     ; 1: Read
	LCD1602_EN_PIN  = 2,    //< 0: Disable   ; 1: Enable
	LCD1602_DB0_PIN = 3,
	LCD1602_DB1_PIN = 4,
	LCD1602_DB2_PIN = 5,
	LCD1602_DB3_PIN = 6,
	LCD1602_DB4_PIN = 7,
	LCD1602_DB5_PIN = 8,
	LCD1602_DB6_PIN = 9,
	LCD1602_DB7_PIN = 10,
};

typedef enum {
	LCD1602_OK 				= 0,
	LCD1602_INIT_PIN_UNSET	= 1,
	LCD1602_NO_READ_PIN 	= 2,
	LCD1602_NO_READ_ADDR 	= 3,

}LCD1602_ERROR_CODE_e;

typedef enum {
	LCD1602_WRITE 	= LCD1602_WRITE_MODE,
	LCD1602_READ    = LCD1602_READ_MODE
}LCD1602_RW_e;

typedef enum {
	LCD1602_COMMAND = LCD1602_COMMAND_MODE,
	LCD1602_DATA    = LCD1602_DATA_MODE
}LCD1602_CMD_DATA_e;


typedef struct{
	struct{
		GPIO_TypeDef* GPIOx;
		uint16_t GPIO_Pin;
	}Pins[12];

	uint8_t ConnectMode;
	LCD1602_RW_e RWMode;

	struct{
		uint16_t cols;
		uint16_t rows;
	}Display_Size;

	uint16_t Cursor_Pos;

}LCD1602_t;



extern LCD1602_ERROR_CODE_e LCD1602_Pins_Set(LCD1602_t* Dev, uint16_t cols, uint16_t rows,
											 GPIO_TypeDef* EN_GPIOx, uint16_t EN_GPIO_Pin,
											 GPIO_TypeDef* RS_GPIOx, uint16_t RS_GPIO_Pin);
extern LCD1602_ERROR_CODE_e LCD1602_RW_Pin_Set(LCD1602_t* Dev, GPIO_TypeDef* RW_GPIOx, uint16_t RW_GPIO_Pin);
extern LCD1602_ERROR_CODE_e LCD1602_4Pin_Set(LCD1602_t* Dev, GPIO_TypeDef* DB0_GPIOx, uint16_t DB0_GPIO_Pin
														   , GPIO_TypeDef* DB1_GPIOx, uint16_t DB1_GPIO_Pin
														   , GPIO_TypeDef* DB2_GPIOx, uint16_t DB2_GPIO_Pin
														   , GPIO_TypeDef* DB3_GPIOx, uint16_t DB3_GPIO_Pin);
extern LCD1602_ERROR_CODE_e LCD1602_8Pin_Set(LCD1602_t* Dev, GPIO_TypeDef* DB0_GPIOx, uint16_t DB0_GPIO_Pin
														   , GPIO_TypeDef* DB1_GPIOx, uint16_t DB1_GPIO_Pin
														   , GPIO_TypeDef* DB2_GPIOx, uint16_t DB2_GPIO_Pin
														   , GPIO_TypeDef* DB3_GPIOx, uint16_t DB3_GPIO_Pin
														   , GPIO_TypeDef* DB4_GPIOx, uint16_t DB4_GPIO_Pin
														   , GPIO_TypeDef* DB5_GPIOx, uint16_t DB5_GPIO_Pin
														   , GPIO_TypeDef* DB6_GPIOx, uint16_t DB6_GPIO_Pin
														   , GPIO_TypeDef* DB7_GPIOx, uint16_t DB7_GPIO_Pin);
extern LCD1602_ERROR_CODE_e LCD1602_Init(LCD1602_t* Dev);
extern LCD1602_ERROR_CODE_e LCD1602_Printf(LCD1602_t* Dev, const char *format,...);
extern LCD1602_ERROR_CODE_e LCD1602_SetCursor(LCD1602_t* Dev, uint16_t cols, uint16_t rows);
extern LCD1602_ERROR_CODE_e LCD1602_Clear(LCD1602_t* Dev);
extern LCD1602_ERROR_CODE_e LCD1602_Cursor_Home(LCD1602_t* Dev);

#endif /* LCD1602_H_ */
