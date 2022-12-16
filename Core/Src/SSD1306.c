/*
 * SSD1306.c
 *
 *  Created on: Apr 29, 2022
 *      Author: Matiss
 */

#include "SSD1306.h"
#include "main.h"
#include <string.h> // where memset prototype can be found
#include "horse_anim.h"

uint8_t	SSD1306_Initialise(SSD1306_t *dev, I2C_HandleTypeDef *i2cHandle){

	// Set struct parameters
	dev->i2cHandle = i2cHandle;

	// Store number of transaction errors (returned at the end of the function)
	uint8_t errNum = 0;
	HAL_StatusTypeDef status;


		status = SSD1306_WriteCommand(dev, SSD1306_REG_DISPLAY_OFF); // Display off

		status = SSD1306_WriteCommand(dev, 0xA8); // Double byte command! Set multiplex ratio (1 to 64).
		status = SSD1306_WriteCommand(dev, 0x3F); // Sets that picture on display starts from top. Permitted entries are between 0x0F - 0x3f as per 0xA8 register describes. Smaller value sets screen start lower. As per datasheet Table 10-1 and 10-2 shows few options.

		status = SSD1306_WriteCommand(dev, 0xD3); // Double byte command! Set display offset.
		status = SSD1306_WriteCommand(dev, 0x00); // After 0xD3 command the offset can be set with this command value. Value can range from 0x00 - 0x3F. Default value after RESET is 0x00.

		status = SSD1306_WriteCommand(dev, 0x40); // Set display start line. Value can range from 0x40 - 0x7F. 0x40 is equal to line 0 and 0x7F will be equal to line 63.

		status = SSD1306_WriteCommand(dev, 0xA1); // Set segment re-map. Value can be A0 or A1.
		status = SSD1306_WriteCommand(dev, 0xC8); // Set COM output scan direction. Value can be C0 or C8.

		status = SSD1306_WriteCommand(dev, 0xDA); // Double byte command! Set COM pins hardware configuration.
		status = SSD1306_WriteCommand(dev, 0x12); // 0x12 uses 8 pages, if you set 0x02, than only 4 pages are represented on display.

		status = SSD1306_WriteCommand(dev, 0x81); // Double byte command! Set contrast control register.
		status = SSD1306_WriteCommand(dev, 0x7F); // After 0x81 command this value sets contrast. Value can range from 0x00 - 0xFF. Default value after RESET is 0x7F.

		status = SSD1306_WriteCommand(dev, SSD1306_REG_ENTIRE_DISPLAY_ON_RAM); // Set display to take data from RAM or ignore RAM data and just be ON.
		status = SSD1306_WriteCommand(dev, SSD1306_REG_DISPLAY_NORMAL); // Set display state to normal or inverse.

		status = SSD1306_WriteCommand(dev, 0xD5); // Double byte command! Set display clock divide ratio/oscillator frequency.
		status = SSD1306_WriteCommand(dev, 0xF0); // Consists of 2 parts: lower 2 bytes sets divide ratio, higher 2 bytes set oscillator frequency. Default value for divide ratio: 0x00, default value for oscillator frequency: 0x08. End value can be set this way (0x08 << 4) | 0x00 and then you get 0x80. How D5h values work can be seen in datasheet. Fosc is set to max value for speed purposes.

		status = SSD1306_WriteCommand(dev, 0x8D); // Double byte command! Charge pump setting up.
		status = SSD1306_WriteCommand(dev, 0x14); // Enable charge pump.

		status = SSD1306_WriteCommand(dev, SSD1306_REG_DISPLAY_ON); // Set entire display ON

	errNum += (status != HAL_OK);
	// Return number of errors (0 if successful initialisation)

	memset(dev->BUFFER, 0x00, sizeof(dev->BUFFER)); //
	/*
	dev->BUFFER[0]= 0xAA;
	dev->BUFFER[17] |= 1 << 1;

	dev->BUFFER[2]= 0x14;
	dev->BUFFER[3]= 0x7F;
	dev->BUFFER[4]= 0x14;
	dev->BUFFER[5]= 0x7F;
	dev->BUFFER[6]= 0x14;

	dev->BUFFER[1019]= 0x14;
	dev->BUFFER[1020]= 0x7F;
	dev->BUFFER[1021]= 0x14;
	dev->BUFFER[1022]= 0x7F;
	dev->BUFFER[1023]= 0x14;
	*/
	//dev->PosX = 1;
	//dev->PosY = 1;
	SSD1306_ScreenUpdate(dev);
	SSD13066_DrawBitmap(dev);


	return errNum;
}

void SSD1306_ScreenUpdate(SSD1306_t *dev){

	//SSD1306_GotoXY (0,0);*/
	//SSD1306_WriteCommand(dev, 0x20);
	//SSD1306_WriteCommand(dev, 0x00);


	//Few pictures to test screen.
	for(uint8_t i=0; i < SSD1306_HEIGHT/8; i++){

		SSD1306_WriteCommand(dev, 0xB0+i);	//	iterates through page start addresses
		SSD1306_WriteCommand(dev, 0x00);	// ??? can it be used without it?
		SSD1306_WriteCommand(dev, 0x10);	// ??? can it be used without it?
		SSD1306_WriteData(dev, &dev->BUFFER[SSD1306_WIDTH*i]);
		//SSD1306_WriteData(dev, &horse1[SSD1306_WIDTH*i]);
		//SSD1306_WriteData(dev, &CLOWN1[SSD1306_WIDTH*i]);

	}
}

void SSD13066_DrawPixel(SSD1306_t *dev){

	uint8_t x = dev->PosX;
	uint8_t y = dev->PosY;

	dev->BUFFER[x+(y/8)*128] |= 1 << (y%8);
}

void SSD13066_DrawBitmap(SSD1306_t *dev){

	uint8_t x = dev->PosX;
	x = 0;
	uint8_t y = dev->PosY;
	y = 0;
	uint8_t i=0, c=0;

	for(; y < 64; y++, x=0){

		for(; x < 8; x++, i++){ // x = 128
		//for(; y < 8; y++){

			if (i>7){
				i=0;
				c++;
			}
			uint8_t a = (CLOWN1[c] & ( 1 << i )) >> i;

			dev->BUFFER[x+(y/8)*128] |= a << (y%8);
			//SSD1306_ScreenUpdate(dev);

		}
		SSD1306_ScreenUpdate(dev);
	}

}


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

