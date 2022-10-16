/********************************************************
 * \file      : LCD1602.c
 * \brief     : The code is to provide a easier way to use LCD1602
 *
 * \copyright : Copyright Jiarun LIU
 * \date      : 2022/10/14
 * \author    : Jiarun LIU
 * \version   : V1.1
*********************************************************/
#include "LCD1602.h"
#include <stdarg.h>


static LCD1602_ERROR_CODE_e LCD1602_RW_Change(LCD1602_t* Dev);
static LCD1602_ERROR_CODE_e LCD1602_Write_Byte(LCD1602_t* Dev, LCD1602_CMD_DATA_e mode, uint8_t byte);
static LCD1602_ERROR_CODE_e LCD1602_Read_Byte(LCD1602_t* Dev, LCD1602_CMD_DATA_e mode, uint8_t* byte);

/**
 * \brief     : This Function is used to set the RS and EN Pin connected to lcd1602
 *
 * \copyright : Copyright Jiarun LIU
 * \date      : 2022/10/15
 * \author    : Jiarun LIU
 *
 * \param[in] : Dev: the address of the device structure
 * \param[in] : max_character: used to identity the device max length
 * \param[in] : EN_GPIOx: GPIOx of the pin used to connected to LCD1602
 * \param[in] : EN_GPIO_Pin: GPIOx of the pin used to connected to LCD1602
 * \return    : used to detect the error @LCD1602_ERROR_CODE_e
 *
 * \include   : <stm32??xx_hal_gpio.h>	?? is based on the core type(for stm32l4xx: <stm32l4xx_hal_gpio.h>)
 */
extern LCD1602_ERROR_CODE_e LCD1602_Pins_Set(LCD1602_t* Dev, uint16_t cols, uint16_t rows,
											 GPIO_TypeDef* EN_GPIOx, uint16_t EN_GPIO_Pin,
											 GPIO_TypeDef* RS_GPIOx, uint16_t RS_GPIO_Pin)
{
	Dev->ConnectMode |= LCD1602_MODE_INIT_SET_1;

	Dev->Display_Size.cols = cols;
	Dev->Display_Size.rows  = rows;

	Dev->Pins[LCD1602_EN_PIN].GPIOx = EN_GPIOx;
	Dev->Pins[LCD1602_EN_PIN].GPIO_Pin = EN_GPIO_Pin;
	Dev->Pins[LCD1602_RS_PIN].GPIOx = RS_GPIOx;
	Dev->Pins[LCD1602_RS_PIN].GPIO_Pin = RS_GPIO_Pin;

	// EN PIN Init
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = EN_GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(EN_GPIOx, &GPIO_InitStruct);

	// RS PIN Init
	GPIO_InitStruct.Pin = RS_GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(RS_GPIOx, &GPIO_InitStruct);

	HAL_GPIO_WritePin(Dev->Pins[LCD1602_EN_PIN].GPIOx, Dev->Pins[LCD1602_EN_PIN].GPIO_Pin, GPIO_PIN_RESET);

	return LCD1602_OK;
}

/**
 * \brief     : This Function is used to set the RW Pin if the pin is connected to lcd1602
 *
 * \copyright : Copyright Jiarun LIU
 * \date      : 2022/10/15
 * \author    : Jiarun LIU
 *
 * \param[in] : Dev: the address of the device structure
 * \param[in] : RW_GPIOx: GPIOx of the pin used to connected to LCD1602
 * \param[in] : RW_GPIO_Pin: GPIOx of the pin used to connected to LCD1602
 * \return    : used to detect the error @LCD1602_ERROR_CODE_e
 *
 * \include   : <stm32??xx_hal_gpio.h>	?? is based on the core type(for stm32l4xx: <stm32l4xx_hal_gpio.h>)
 */
extern LCD1602_ERROR_CODE_e LCD1602_RW_Pin_Set(LCD1602_t* Dev, GPIO_TypeDef* RW_GPIOx, uint16_t RW_GPIO_Pin)
{
	Dev->ConnectMode |= LCD1602_MODE_RW_STATE;

	Dev->Pins[LCD1602_EN_PIN].GPIOx = RW_GPIOx;
	Dev->Pins[LCD1602_EN_PIN].GPIO_Pin = RW_GPIO_Pin;

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = RW_GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(RW_GPIOx, &GPIO_InitStruct);

	HAL_GPIO_WritePin(Dev->Pins[LCD1602_RW_PIN].GPIOx, Dev->Pins[LCD1602_RW_PIN].GPIO_Pin, GPIO_PIN_RESET); // default is write

	return LCD1602_OK;
}

/**
 * \brief     : This Function is used to set a 4 pin connection lcd1602
 *
 * \copyright : Copyright Jiarun LIU
 * \date      : 2022/10/15
 * \author    : Jiarun LIU
 *
 * \param[in] : Dev: the address of the device structure
 * \param[in] : DB?_GPIOx: GPIOx of the pin used to connected to LCD1602
 * \param[in] : DB?_GPIO_Pin: GPIOx of the pin used to connected to LCD1602
 * \return    : used to detect the error @LCD1602_ERROR_CODE_e
 *
 * \include   : <stm32??xx_hal_gpio.h>	?? is based on the core type(for stm32l4xx: <stm32l4xx_hal_gpio.h>)
 */
extern LCD1602_ERROR_CODE_e LCD1602_4Pin_Set(LCD1602_t* Dev, GPIO_TypeDef* DB0_GPIOx, uint16_t DB0_GPIO_Pin
														   , GPIO_TypeDef* DB1_GPIOx, uint16_t DB1_GPIO_Pin
														   , GPIO_TypeDef* DB2_GPIOx, uint16_t DB2_GPIO_Pin
														   , GPIO_TypeDef* DB3_GPIOx, uint16_t DB3_GPIO_Pin)
{
	Dev->ConnectMode |= LCD1602_MODE_DATA_STATE;
	Dev->ConnectMode |= LCD1602_MODE_INIT_SET_2;

	Dev->RWMode = LCD1602_WRITE;

	Dev->Pins[LCD1602_DB0_PIN].GPIOx = DB0_GPIOx;
	Dev->Pins[LCD1602_DB0_PIN].GPIO_Pin = DB0_GPIO_Pin;
	Dev->Pins[LCD1602_DB1_PIN].GPIOx = DB1_GPIOx;
	Dev->Pins[LCD1602_DB1_PIN].GPIO_Pin = DB1_GPIO_Pin;
	Dev->Pins[LCD1602_DB2_PIN].GPIOx = DB2_GPIOx;
	Dev->Pins[LCD1602_DB2_PIN].GPIO_Pin = DB2_GPIO_Pin;
	Dev->Pins[LCD1602_DB3_PIN].GPIOx = DB3_GPIOx;
	Dev->Pins[LCD1602_DB3_PIN].GPIO_Pin = DB3_GPIO_Pin;


	for(int pin_num = LCD1602_DB0_PIN; pin_num <= LCD1602_DB3_PIN; pin_num++)
	{
		GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_InitStruct.Pin = Dev->Pins[pin_num].GPIO_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(Dev->Pins[pin_num].GPIOx, &GPIO_InitStruct);
	}

	return LCD1602_OK;
}

/**
 * \brief     : This Function is used to set a 8 pin connection lcd1602
 *
 * \copyright : Copyright Jiarun LIU
 * \date      : 2022/10/15
 * \author    : Jiarun LIU
 *
 * \param[in] : Dev: the address of the device structure
 * \param[in] : DB?_GPIOx: GPIOx of the pin used to connected to LCD1602
 * \param[in] : DB?_GPIO_Pin: GPIOx of the pin used to connected to LCD1602
 * \return    : used to detect the error @LCD1602_ERROR_CODE_e
 *
 * \include   : <stm32??xx_hal_gpio.h>	?? is based on the core type(for stm32l4xx: <stm32l4xx_hal_gpio.h>)
 */
extern LCD1602_ERROR_CODE_e LCD1602_8Pin_Set(LCD1602_t* Dev, GPIO_TypeDef* DB0_GPIOx, uint16_t DB0_GPIO_Pin
														   , GPIO_TypeDef* DB1_GPIOx, uint16_t DB1_GPIO_Pin
														   , GPIO_TypeDef* DB2_GPIOx, uint16_t DB2_GPIO_Pin
														   , GPIO_TypeDef* DB3_GPIOx, uint16_t DB3_GPIO_Pin
														   , GPIO_TypeDef* DB4_GPIOx, uint16_t DB4_GPIO_Pin
														   , GPIO_TypeDef* DB5_GPIOx, uint16_t DB5_GPIO_Pin
														   , GPIO_TypeDef* DB6_GPIOx, uint16_t DB6_GPIO_Pin
														   , GPIO_TypeDef* DB7_GPIOx, uint16_t DB7_GPIO_Pin)
{
	Dev->ConnectMode |= LCD1602_MODE_INIT_SET_2;

	Dev->RWMode = LCD1602_WRITE;

	Dev->Pins[LCD1602_DB0_PIN].GPIOx = DB0_GPIOx;
	Dev->Pins[LCD1602_DB0_PIN].GPIO_Pin = DB0_GPIO_Pin;
	Dev->Pins[LCD1602_DB1_PIN].GPIOx = DB1_GPIOx;
	Dev->Pins[LCD1602_DB1_PIN].GPIO_Pin = DB1_GPIO_Pin;
	Dev->Pins[LCD1602_DB2_PIN].GPIOx = DB2_GPIOx;
	Dev->Pins[LCD1602_DB2_PIN].GPIO_Pin = DB2_GPIO_Pin;
	Dev->Pins[LCD1602_DB3_PIN].GPIOx = DB3_GPIOx;
	Dev->Pins[LCD1602_DB3_PIN].GPIO_Pin = DB3_GPIO_Pin;	Dev->RWMode = LCD1602_WRITE;
	Dev->Pins[LCD1602_DB4_PIN].GPIOx = DB4_GPIOx;
	Dev->Pins[LCD1602_DB4_PIN].GPIO_Pin = DB4_GPIO_Pin;
	Dev->Pins[LCD1602_DB5_PIN].GPIOx = DB5_GPIOx;
	Dev->Pins[LCD1602_DB5_PIN].GPIO_Pin = DB5_GPIO_Pin;
	Dev->Pins[LCD1602_DB6_PIN].GPIOx = DB6_GPIOx;
	Dev->Pins[LCD1602_DB6_PIN].GPIO_Pin = DB6_GPIO_Pin;
	Dev->Pins[LCD1602_DB7_PIN].GPIOx = DB7_GPIOx;
	Dev->Pins[LCD1602_DB7_PIN].GPIO_Pin = DB7_GPIO_Pin;

	for(int pin_num = LCD1602_DB0_PIN; pin_num <= LCD1602_DB7_PIN; pin_num++)
	{
		GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_InitStruct.Pin = Dev->Pins[pin_num].GPIO_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(Dev->Pins[pin_num].GPIOx, &GPIO_InitStruct);
	}

	return LCD1602_OK;
}

/**
 * \brief     : This Function is used to provide the display data (same as printf)
 *
 * \copyright : Copyright Jiarun LIU
 * \date      : 2022/10/15
 * \author    : Jiarun LIU
 *
 * \param[in] : Dev: the address of the device structure
 * \param[in] : format: the format of the input data
 * \param[in] : ...: the input variable
 * \return    : used to detect the error @LCD1602_ERROR_CODE_e
 *
 * \include   : <stm32??xx_hal_gpio.h>	?? is based on the core type(for stm32l4xx: <stm32l4xx_hal_gpio.h>)
 */
extern LCD1602_ERROR_CODE_e LCD1602_Printf(LCD1602_t* Dev, const char *format,...)
{
	va_list ap;
	uint16_t len = 0;
	char buffer[LCD1602_MAX_WRITTER_BUFFER_SIZE] = {0};

	va_start(ap,format);

	len = vsprintf(((char*) buffer), format, ap);

	if(len > (Dev->Display_Size.cols * Dev->Display_Size.rows)){
		len = (Dev->Display_Size.cols * Dev->Display_Size.rows);
	}

	va_end(ap);

	for(int i = 0; i < len; i++)
	{
		LCD1602_Write_Byte(Dev, LCD1602_DATA, buffer[i]);
	}

	return LCD1602_OK;
}

extern LCD1602_ERROR_CODE_e LCD1602_SetCursor(LCD1602_t* Dev, uint16_t cols, uint16_t rows)
{
	uint16_t target_position = rows*Dev->Display_Size.cols + cols;

	if(target_position > Dev->Cursor_Pos)
	{
		int distance = target_position - Dev->Cursor_Pos;
		for(int i = 0; i < distance;i++)
		{
			LCD1602_Write_Byte(Dev, LCD1602_COMMAND, LCD1602_COMMAND_ENTRY_MODE |
													 LCD1602_COMMAND_ENTRY_MODE_CURSOR_DIR);
		}
	} else {
		int distance = Dev->Cursor_Pos - target_position;

		// todo: add code to judge the start point is now or set to 0


		for(int i = 0; i < distance;i++)
		{
			LCD1602_Write_Byte(Dev, LCD1602_COMMAND, LCD1602_COMMAND_ENTRY_MODE);
		}
	}

	return LCD1602_OK;
}

extern LCD1602_ERROR_CODE_e LCD1602_Clear(LCD1602_t* Dev)
{
	LCD1602_Delay_ms(1);
	return LCD1602_Write_Byte(Dev, LCD1602_COMMAND, LCD1602_COMMAND_CLEAR);
}

extern LCD1602_ERROR_CODE_e LCD1602_Cursor_Home(LCD1602_t* Dev)
{
	LCD1602_Delay_ms(1);
	return LCD1602_Write_Byte(Dev, LCD1602_COMMAND, LCD1602_COMMAND_CURSOR_HOME);
}






/**
 * \brief     : This Function is used to Init the device
 *
 * \copyright : Copyright Jiarun LIU
 * \date      : 2022/10/15
 * \author    : Jiarun LIU
 *
 * \param[in] : Dev: the address of the device structure
 * \return    : used to detect the error @LCD1602_ERROR_CODE_e
 *
 * \include   : <stm32??xx_hal_gpio.h>	?? is based on the core type(for stm32l4xx: <stm32l4xx_hal_gpio.h>)
 */
extern LCD1602_ERROR_CODE_e LCD1602_Init(LCD1602_t* Dev)
{
	if((Dev->ConnectMode | LCD1602_MODE_INIT_SET_1) && (Dev->ConnectMode | LCD1602_MODE_INIT_SET_2))
	{

	} else {
		return LCD1602_INIT_PIN_UNSET;
	}

	LCD1602_Write_Byte(Dev, LCD1602_COMMAND, 0x38);
	LCD1602_Delay_ms(5);
	LCD1602_Write_Byte(Dev, LCD1602_COMMAND, 0x38);
#ifdef LCD1602_US_TIMER
	LCD1602_Delay_us(101);
#else
	LCD1602_Delay_ms(2);
#endif
	LCD1602_Write_Byte(Dev, LCD1602_COMMAND, 0x38);
	LCD1602_Delay_ms(2);
	LCD1602_Write_Byte(Dev, LCD1602_COMMAND, LCD1602_COMMAND_DISPLAY_MODE);
	LCD1602_Delay_ms(2);
	LCD1602_Write_Byte(Dev, LCD1602_COMMAND, LCD1602_COMMAND_CLEAR);
	LCD1602_Delay_ms(2);
	LCD1602_Write_Byte(Dev, LCD1602_COMMAND, LCD1602_COMMAND_ENTRY_MODE | LCD1602_COMMAND_ENTRY_MODE_CURSOR_DIR);
	LCD1602_Delay_ms(2);
	LCD1602_Write_Byte(Dev, LCD1602_COMMAND, LCD1602_COMMAND_DISPLAY_MODE |
											 LCD1602_COMMAND_DISPLAY_ON_OFF |
											 LCD1602_COMMAND_DISPLAY_CURSOR |
											 LCD1602_COMMAND_DISPLAY_CURSOR_BLINK);
	LCD1602_Delay_ms(2);

	if(!(Dev->ConnectMode & LCD1602_MODE_DATA_STATE))
	{
		// 8 Pin connection code
		LCD1602_Write_Byte(Dev, LCD1602_COMMAND, LCD1602_COMMAND_FUNCTION_SET |
												 LCD1602_COMMAND_FUNCTION_DATA_LENGTH);


	} else if ((Dev->ConnectMode & LCD1602_MODE_DATA_STATE)){
		// 4 Pin Connection code
		//todo: add 4 Pin connection code
	}

	LCD1602_Clear(Dev);

	return LCD1602_OK;
}



















/**
 * \brief     : This Function is used to change the data pin mode
 * 				when the Read and Write function is changed
 *
 * \copyright : Copyright Jiarun LIU
 * \date      : 2022/10/15
 * \author    : Jiarun LIU
 *
 * \param[in] : Dev: the address of the device structure
 * \return    : used to detect the error @LCD1602_ERROR_CODE_e
 *
 * \include   : <stm32??xx_hal_gpio.h>	?? is based on the core type(for stm32l4xx: <stm32l4xx_hal_gpio.h>)
 */
static LCD1602_ERROR_CODE_e LCD1602_RW_Change(LCD1602_t* Dev)
{
	uint32_t Pin_Mode = (Dev->RWMode == LCD1602_WRITE)?(GPIO_MODE_OUTPUT_PP):(GPIO_MODE_INPUT);

	if((Dev->ConnectMode | LCD1602_MODE_RW_STATE) == 0)
	{
		GPIO_PinState RW_State = (Dev->RWMode == LCD1602_WRITE)?(GPIO_PIN_RESET):(GPIO_PIN_SET);

		HAL_GPIO_WritePin(Dev->Pins[LCD1602_RW_PIN].GPIOx, Dev->Pins[LCD1602_RW_PIN].GPIO_Pin, RW_State);
	}

	uint8_t max_data_pin_num = LCD1602_DB7_PIN;
	if(!(Dev->ConnectMode & LCD1602_MODE_DATA_STATE)){
		// 8 Pin connection code
		max_data_pin_num = LCD1602_DB7_PIN;
	} else if (Dev->ConnectMode & LCD1602_MODE_DATA_STATE){
		// 4 Pin Connection code
		max_data_pin_num = LCD1602_DB3_PIN;
	}

	for(int pin_num = LCD1602_DB0_PIN; pin_num <= max_data_pin_num; pin_num++)
	{
		GPIO_InitTypeDef GPIO_InitStruct = {0};

		GPIO_InitStruct.Pin = Dev->Pins[pin_num].GPIO_Pin;
		GPIO_InitStruct.Mode = Pin_Mode;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(Dev->Pins[pin_num].GPIOx, &GPIO_InitStruct);
	}
	return LCD1602_OK;
}

/**
 * \brief     : This Function is used to Write the command or data to the device
 *
 * \copyright : Copyright Jiarun LIU
 * \date      : 2022/10/15
 * \author    : Jiarun LIU
 *
 * \param[in] : Dev: the address of the device structure
 * \param[in] : mode: indicate the command or data @LCD1602_CMD_DATA_e
 * \param[in] : byte: the data in byte need to write
 * \return    : used to detect the error @LCD1602_ERROR_CODE_e
 *
 * \include   : <stm32??xx_hal_gpio.h>	?? is based on the core type(for stm32l4xx: <stm32l4xx_hal_gpio.h>)
 */
static LCD1602_ERROR_CODE_e LCD1602_Write_Byte(LCD1602_t* Dev, LCD1602_CMD_DATA_e mode, uint8_t byte)
{
	// adjust the device between read and write
	if(Dev->RWMode != LCD1602_WRITE)
	{
		Dev->RWMode = LCD1602_WRITE;

		LCD1602_RW_Change(Dev);
	}

	if(mode == LCD1602_COMMAND_MODE){
		HAL_GPIO_WritePin(Dev->Pins[LCD1602_RS_PIN].GPIOx, Dev->Pins[LCD1602_RS_PIN].GPIO_Pin, GPIO_PIN_RESET);
	} else if(mode == LCD1602_DATA_MODE){
		HAL_GPIO_WritePin(Dev->Pins[LCD1602_RS_PIN].GPIOx, Dev->Pins[LCD1602_RS_PIN].GPIO_Pin, GPIO_PIN_SET);
	}

	HAL_GPIO_WritePin(Dev->Pins[LCD1602_EN_PIN].GPIOx, Dev->Pins[LCD1602_EN_PIN].GPIO_Pin, GPIO_PIN_RESET);

	if(!(Dev->ConnectMode & LCD1602_MODE_DATA_STATE))
	{
		// 8 Pin connection code
		for(int pin_num = LCD1602_DB0_PIN; pin_num <= LCD1602_DB7_PIN; pin_num++)
		{
			GPIO_PinState pin_state = (byte & (1 << (pin_num - LCD1602_DB0_PIN)));
			HAL_GPIO_WritePin(Dev->Pins[pin_num].GPIOx, Dev->Pins[pin_num].GPIO_Pin, pin_state);
		}

	} else if ((Dev->ConnectMode & LCD1602_MODE_DATA_STATE)){
		// 4 Pin Connection code
		//todo: add 4 Pin connection code
	}

	LCD1602_Delay_ms(1);
	HAL_GPIO_WritePin(Dev->Pins[LCD1602_EN_PIN].GPIOx, Dev->Pins[LCD1602_EN_PIN].GPIO_Pin, GPIO_PIN_SET);
	LCD1602_Delay_ms(1);
	HAL_GPIO_WritePin(Dev->Pins[LCD1602_EN_PIN].GPIOx, Dev->Pins[LCD1602_EN_PIN].GPIO_Pin, GPIO_PIN_RESET);

	return LCD1602_OK;
}

/**
 * \brief     : This Function is used to Write the command or data to the device
 *
 * \copyright : Copyright Jiarun LIU
 * \date      : 2022/10/15
 * \author    : Jiarun LIU
 *
 * \param[in] : Dev: the address of the device structure
 * \param[in] : mode: indicate the command or data @LCD1602_CMD_DATA_e
 * \param[in] : byte: the data in byte need to write
 * \return    : used to detect the error @LCD1602_ERROR_CODE_e
 *
 * \include   : <stm32??xx_hal_gpio.h>	?? is based on the core type(for stm32l4xx: <stm32l4xx_hal_gpio.h>)
 */
static LCD1602_ERROR_CODE_e LCD1602_Read_Byte(LCD1602_t* Dev, LCD1602_CMD_DATA_e mode, uint8_t* byte)
{
	// if the read data pointer is not given
	if(byte == NULL)
	{
		return LCD1602_NO_READ_ADDR;
	}

	// if the LCD1602 isn't connect the RW Pin
	if((Dev->ConnectMode | LCD1602_MODE_RW_STATE) == 1)
	{
		return LCD1602_NO_READ_PIN;
	}

	// adjust the device between read and write
	if(Dev->RWMode != LCD1602_READ)
	{
		Dev->RWMode = LCD1602_READ;

		LCD1602_RW_Change(Dev);
	}

	if(mode == LCD1602_COMMAND_MODE)	{
		HAL_GPIO_WritePin(Dev->Pins[LCD1602_RS_PIN].GPIOx, Dev->Pins[LCD1602_RS_PIN].GPIO_Pin, GPIO_PIN_RESET);
	} else if(mode == LCD1602_DATA_MODE){
		HAL_GPIO_WritePin(Dev->Pins[LCD1602_RS_PIN].GPIOx, Dev->Pins[LCD1602_RS_PIN].GPIO_Pin, GPIO_PIN_SET);
	}

	*byte = 0;
	if(!(Dev->ConnectMode & LCD1602_MODE_DATA_STATE))
	{
		// 8 Pin connection code
		for(uint8_t pin_num = LCD1602_DB0_PIN; pin_num <= LCD1602_DB7_PIN; pin_num++)
		{
			GPIO_PinState pin_state = HAL_GPIO_ReadPin(Dev->Pins[pin_num].GPIOx, Dev->Pins[pin_num].GPIO_Pin);
			*byte |= ((uint8_t)pin_state << (uint8_t)(pin_num - LCD1602_DB0_PIN));
		}

	} else if (Dev->ConnectMode & LCD1602_MODE_DATA_STATE){
		// 4 Pin Connection code
		//todo: add 4 Pin connection code
	}

	return LCD1602_OK;
}


