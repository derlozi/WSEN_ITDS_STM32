/*
 * itds.c
 *
 *  Created on: 22.09.2022
 *      Author: loren
 */
#include <itds.h>

void readReg(I2C_HandleTypeDef* hi2c, uint8_t regAddress, uint8_t* buf);
void writeReg(I2C_HandleTypeDef* hi2c, uint8_t Address, uint8_t* buf);

HAL_StatusTypeDef itds_init(I2C_HandleTypeDef* hi2c, uint8_t mode, uint8_t bandwidth, uint8_t scale, uint8_t ODR, uint8_t noiseMode, uint8_t filter)
{
	uint8_t buf;
	readReg(hi2c, ITDS_DEVICE_ID, &buf);
	if(buf != 0x44)
	{
		return HAL_ERROR;
	}
	buf = 0x0C;	//as recommended by datasheet
				//enable block update(registers only change after L and H have been read) and multi read auto address increment
	writeReg(hi2c, ITDS_CTRL_2, &buf);
	buf = ODR | mode;
	writeReg(hi2c, ITDS_CTRL_1, &buf);
	if(mode == ITDS_SINGLECONV_12B || mode == ITDS_SINGLECONV_14B)//SLP_MODE_SEL Bit in CTRL_3 needs to be set
	{
		buf=0x02;
		writeReg(hi2c, ITDS_CTRL_3, &buf);
	}
	buf=bandwidth | scale | filter | noiseMode;
	writeReg(hi2c, ITDS_CTRL_6, &buf);

	return HAL_OK;
}

//reads DRDY bit
uint8_t itds_pollData(I2C_HandleTypeDef* hi2c)
{
	uint8_t buf;
	readReg(hi2c, ITDS_STATUS, &buf);
	return (buf&0x01);//DRDY bit
}

struct itds_data itds_readData(I2C_HandleTypeDef* hi2c, uint8_t scaleSetting, uint8_t modeSetting)
{
	uint8_t buf[6];
	HAL_I2C_Mem_Read(hi2c, ITDS_SLAVE_ADDRESS, ITDS_X_OUT_L, 1, buf, 6, HAL_MAX_DELAY);
	uint8_t bitNum;
	uint8_t scale;
	if(modeSetting == ITDS_SINGLECONV_12B || modeSetting == ITDS_LOWPOWER_12B)
	{
		bitNum = 12;
	}
	else
	{
		bitNum = 14;
	}
	switch (scaleSetting) {
		case ITDS_SCALE_2G:
			scale = 2;
			break;
		case ITDS_SCALE_4G:
			scale = 4;
			break;
		case ITDS_SCALE_8G:
			scale = 8;
			break;
		case ITDS_SCALE_16G:
			scale = 16;
			break;
		default:
			scale = 0;
			break;
	}
	struct itds_data accData;
	if(bitNum == 12)
	{
		accData.x = 2*(float)scale/(1<<bitNum)*1000.0*((int16_t)(buf[0] | buf[1]<<8)>>4);//(1<<bitnum) is used as 2^bitnum (efficiency!)
		accData.y = 2*(float)scale/(1<<bitNum)*1000.0*((int16_t)(buf[2] | buf[3]<<8)>>4);
		accData.z = 2*(float)scale/(1<<bitNum)*1000.0*((int16_t)(buf[4] | buf[5]<<8)>>4);
	}
	else
	{
		accData.x = 2*(float)scale/(1<<bitNum)*1000.0*((int16_t)(buf[0] | buf[1]<<8)>>2);
		accData.y = 2*(float)scale/(1<<bitNum)*1000.0*((int16_t)(buf[2] | buf[3]<<8)>>2);
		accData.z = 2*(float)scale/(1<<bitNum)*1000.0*((int16_t)(buf[4] | buf[5]<<8)>>2);
	}
	return accData;
}

void itds_startConversion(I2C_HandleTypeDef* hi2c)
{
	uint8_t buf = 0x03;
	writeReg(hi2c, ITDS_CTRL_3, &buf);
}

float itds_getTemperature(I2C_HandleTypeDef* hi2c)
{
	int16_t readOut;
	HAL_I2C_Mem_Read(hi2c, ITDS_SLAVE_ADDRESS, ITDS_T_OUT_L, 1, (uint8_t*) &readOut, 1, HAL_MAX_DELAY);
	return (((float_t)readout / 16.0) + 25.0);
}
//interrupts can be OR-Connection of constants of the form ITDS_INT0_XY or ITDS_INT1_XY
void routeInterrupts(I2C_HandleTypeDef* hi2c, uint8_t interrupt, uint8_t interrupts)
{
	uint8t_t data = interrupts;
	if(interrupt == 0)
	{
		writeReg(hi2c, ITDS_CTRL_4, &data);
	}
	else if(interrupt == 1)
	{
		writeReg(hi2c, ITDS_CTRL_4, &data);
	}
}

//Check bits in the status register
uint8_t checkStatus(I2C_HandleTypeDef hi2c, uint8_t statusbit)
{
	uint8_t data;
	readReg(&hi2c, ITDS_STATUS, &data);
	if(data&statusbit)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//maskfunction should either be ITDS_MASKOR or ITDS_MASKAND
void maskReg(I2C_HandleTypeDef* hi2c, uint8_t maskFunction, uint8_t regAddress, uint8_t mask)
{
	uint8_t data;
	readReg(hi2c, regAddress, &data);
	if(maskFunction==ITDS_MASKOR)
	{
		data |= mask;
	}
	else
	{
		data &= mask;
	}
	writeReg(hi2c, regAddress, &data);
}

//helper function to avoid having to enter same arguments every time Mem_read is called
void readReg(I2C_HandleTypeDef* hi2c, uint8_t regAddress, uint8_t* buf)
{
	HAL_I2C_Mem_Read(hi2c, ITDS_SLAVE_ADDRESS, regAddress, 1, buf, 1, HAL_MAX_DELAY);
}

//helper function to avoid having to enter same arguments every time Mem_write is called
void writeReg(I2C_HandleTypeDef* hi2c, uint8_t Address, uint8_t* buf)
{
	HAL_I2C_Mem_Write(hi2c, ITDS_SLAVE_ADDRESS, Address, 1, buf, 1, HAL_MAX_DELAY);
}
