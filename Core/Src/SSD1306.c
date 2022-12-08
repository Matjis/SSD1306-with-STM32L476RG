/*
 * SSD1306.c
 *
 *  Created on: Apr 29, 2022
 *      Author: Matiss
 */

#include "SSD1306.h"
#include "main.h"
#include <string.h> // where memset prototype can be found

uint8_t	SSD1306_Initialise(SSD1306_t *dev, I2C_HandleTypeDef *i2cHandle){

	// Set struct parameters
	dev->i2cHandle = i2cHandle;

	// Store number of transaction errors (returned at the end of the function)
	uint8_t errNum = 0;
	HAL_StatusTypeDef status;

	//SSD1306_GotoXY (0,0);*/

	status = SSD1306_WriteCommand(dev, SSD1306_REG_DISPLAY_OFF); //display off
		status = SSD1306_WriteCommand(dev, 0x20); //Set Memory Addressing Mode
		status = SSD1306_WriteCommand(dev, 0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
		status = SSD1306_WriteCommand(dev, 0xB0); //Set Page Start Address for Page Addressing Mode,0-7
		status = SSD1306_WriteCommand(dev, 0xC8); //Set COM Output Scan Direction
		status = SSD1306_WriteCommand(dev, 0x00); //---set low column address
		status = SSD1306_WriteCommand(dev, 0x10); //---set high column address
		status = SSD1306_WriteCommand(dev, 0x40); //--set start line address
		status = SSD1306_WriteCommand(dev, 0x81); //--set contrast control register
		status = SSD1306_WriteCommand(dev, 0xFF);
		status = SSD1306_WriteCommand(dev, 0xA1); //--set segment re-map 0 to 127
		status = SSD1306_WriteCommand(dev, 0xA6); //--set normal display
		status = SSD1306_WriteCommand(dev, 0xA8); //--set multiplex ratio(1 to 64)
		status = SSD1306_WriteCommand(dev, 0x3F); //
		status = SSD1306_WriteCommand(dev, 0xA4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
		status = SSD1306_WriteCommand(dev, 0xD3); //-set display offset
		status = SSD1306_WriteCommand(dev, 0x00); //-not offset
		status = SSD1306_WriteCommand(dev, 0xD5); //--set display clock divide ratio/oscillator frequency
		status = SSD1306_WriteCommand(dev, 0xF0); //--set divide ratio
		status = SSD1306_WriteCommand(dev, 0xD9); //--set pre-charge period
		status = SSD1306_WriteCommand(dev, 0x22); //
		status = SSD1306_WriteCommand(dev, 0xDA); //--set com pins hardware configuration
		status = SSD1306_WriteCommand(dev, 0x12);
		status = SSD1306_WriteCommand(dev, 0xDB); //--set vcomh
		status = SSD1306_WriteCommand(dev, 0x20); //0x20,0.77xVcc
		status = SSD1306_WriteCommand(dev, 0x8D); //--set DC-DC enable
		status = SSD1306_WriteCommand(dev, 0x14); //
		status = SSD1306_WriteCommand(dev, SSD1306_REG_DISPLAY_ON); //--turn on SSD1306 panel
	errNum += (status != HAL_OK);
	// Return number of errors (0 if successful initialisation)

	memset(dev->BUFFER, 0x11, sizeof(dev->BUFFER));
	for(uint8_t i=0; i < SSD1306_HEIGHT/8; i++){
	status = SSD1306_WriteCommand(dev, 0xB0+i);	//	iterates through page start addresses
	status = SSD1306_WriteCommand(dev, 0x00);	// ??? can it be used without it?
	status = SSD1306_WriteCommand(dev, 0x10);	// ??? can it be used without it?
	status = SSD1306_WriteData(dev, &dev->BUFFER[SSD1306_WIDTH*i]);
	}

	return errNum;
}

uint8_t SSD1306_ScreenUpdate();

/*
HAL_StatusTypeDef SSD1306_ReadRegister(SSD1306_t *dev, uint8_t *data){

	return HAL_I2C_Mem_Read(dev->i2cHandle, SSD1306_I2C_ADDR, SSD1306_REG_COMMAND, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef SSD1306_ReadRegisters(SSD1306_t *dev, uint8_t reg, uint8_t *data, uint8_t length){

	return HAL_I2C_Mem_Read(dev->i2cHandle, SSD1306_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);
}
*/

HAL_StatusTypeDef SSD1306_WriteCommand(SSD1306_t *dev, uint8_t command){

	return HAL_I2C_Mem_Write(dev->i2cHandle, SSD1306_I2C_ADDR, SSD1306_COMMAND, I2C_MEMADD_SIZE_8BIT, &command, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef SSD1306_WriteData(SSD1306_t *dev, uint8_t *data){

	return HAL_I2C_Mem_Write(dev->i2cHandle, SSD1306_I2C_ADDR, SSD1306_DATA, I2C_MEMADD_SIZE_8BIT, data, SSD1306_WIDTH, HAL_MAX_DELAY);
}

