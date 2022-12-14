/*
 * SSD1306.h
 *
 *  Created on: Apr 29, 2022
 *      Author: Matiss
 */

#ifndef SSD1306_H_
#define SSD1306_H_

#include "stm32l4xx_hal.h"

/*	011110 + SA0 + R/W# - address of the device
 *
 *	SA0 is set to 0 on the pcb
 *	R/W# 0 =  write mode, 1 = read mode
 */
#define SSD1306_I2C_ADDR	 0x78 // logic analyzer show that real addressed address is 0x3C when 0x78 is used and on the pcb address 0x78 is labeled.

#define SSD1306_COMMAND 0x00
#define SSD1306_DATA 0x40

// REGISTERS

#define SSD1306_REG_ENTIRE_DISPLAY_ON_RAM		0xA4	// Show RAM content on display
#define SSD1306_REG_ENTIRE_DISPLAY_ON_NO_RAM	0xA5	// Forces the entire display ON regardless of RAM content

#define SSD1306_REG_DISPLAY_NORMAL				0xA6	//	RAM data value of 1 indicates ON pixel
#define SSD1306_REG_DISPLAY_INVERSE				0xA7	//	RAM data value of 0 indicates ON pixel

#define SSD1306_REG_DISPLAY_ON					0xAF	// Turn on SSD1306 panel
#define SSD1306_REG_DISPLAY_OFF					0xAE	// Turn off SSD1306 panel

#define SSD1306_REG_LOW_COL						0x00	//	Set lower column start address
#define SSD1306_REG_HIGH_COL					0x10	//	Set higher column start address


//#define SSD1306_REG_							0x00	//


// DISPLAY PARAMETERS

#define SSD1306_WIDTH		128
#define SSD1306_HEIGHT		64


// SENSOR STRUCT

typedef struct{

	I2C_HandleTypeDef *i2cHandle;	// Place where to store i2c Handler
	char BUFFER[SSD1306_WIDTH*SSD1306_HEIGHT/8]; // Place to store all pixel values for display
	uint8_t PosX;	// X (horizontal) position on screen (0 to SSD1306_WIDTH -1)
	uint8_t PosY;	// Y (vertical) position on screen	 (0 to SSD1306_HEIGHT -1)
	uint8_t BitmapWidth;
	uint8_t BitmapHeight;

}SSD1306_t;


// INITIALISATION

uint8_t	SSD1306_Initialise(SSD1306_t *dev, I2C_HandleTypeDef *i2cHandle);

// FUNCTIONALITY
void SSD1306_ScreenUpdate(SSD1306_t *dev);
void SSD13066_DrawPixel(SSD1306_t *dev);
void SSD13066_DrawBitmap(SSD1306_t *dev);

// LOW-LEVEL FUNCTIONS

HAL_StatusTypeDef SSD1306_ReadRegister(SSD1306_t *dev, uint8_t *data);
HAL_StatusTypeDef SSD1306_ReadRegisters(SSD1306_t *dev, uint8_t reg, uint8_t *data, uint8_t length);

//HAL_StatusTypeDef SSD1306_WriteRegister(SSD1306_t *dev, uint8_t ComOrData, uint8_t data);
HAL_StatusTypeDef SSD1306_WriteCommand(SSD1306_t *dev, uint8_t command);
HAL_StatusTypeDef SSD1306_WriteData(SSD1306_t *dev, uint8_t* data);

#endif /* SSD1306_H_ */
