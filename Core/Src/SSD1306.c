/*
 * SSD1306.c
 *
 *  Created on: Apr 29, 2022
 *      Author: Matiss
 */

#include "SSD1306.h"
#include "main.h"

uint8_t	SSD1306_Initialise(SSD1306 *dev, I2C_HandleTypeDef *i2cHandle){

	// Set struct parameters
	dev->i2cHandle = i2cHandle;

	// Store number of transaction errors (returned at the end of the function)
	uint8_t errNum = 0;
	HAL_StatusTypeDef status;

	status = SSD1306_WriteRegister(dev, SSD1306_REG_ENTIRE_DISPLAY_ON_NO_RAM);
	errNum += (status != HAL_OK);


	status = SSD1306_WriteRegister(dev, SSD1306_REG_DISPLAY_ON);
	errNum += (status != HAL_OK);


	// Return number of errors (0 if successful initialisation)
	return errNum;
}


/*
HAL_StatusTypeDef SSD1306_ReadRegister(SSD1306 *dev, uint8_t *data){

	return HAL_I2C_Mem_Read(dev->i2cHandle, SSD1306_I2C_ADDR, SSD1306_REG_COMMAND, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef SSD1306_ReadRegisters(SSD1306 *dev, uint8_t reg, uint8_t *data, uint8_t lenght){

	return HAL_I2C_Mem_Read(dev->i2cHandle, SSD1306_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, lenght, HAL_MAX_DELAY);
}
*/

HAL_StatusTypeDef SSD1306_WriteRegister(SSD1306 *dev, uint8_t data){

	return HAL_I2C_Mem_Write(dev->i2cHandle, SSD1306_I2C_ADDR, SSD1306_DATA, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY);
}
