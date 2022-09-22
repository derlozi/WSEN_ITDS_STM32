/*
 * itdsen.h
 *
 *  Created on: 22.09.2022
 *      Author: loren
 */

#ifndef INC_ITDS_H_
#define INC_ITDS_H_

#include "stm32l0xx_hal.h"

#define ITDS_SLAVE_ADDRESS 0x19<<1

//Register definitions
#define ITDS_DEVICE_ID 0x0F
#define ITDS_CTRL_1 0x20
#define ITDS_CTRL_2 0x21
#define ITDS_CTRL_3 0x22
#define ITDS_CTRL_4 0x23
#define ITDS_CTRL_5 0x24
#define ITDS_CTRL_6 0x25
#define ITDS_T_OUT 0x26
#define ITDS_STATUS 0x27
#define ITDS_X_OUT_L 0x28
#define ITDS_X_OUT_H 0x29
#define ITDS_Y_OUT_L 0x2A
#define ITDS_Y_OUT_H 0x2B
#define ITDS_Z_OUT_L 0x2C
#define ITDS_Z_OUT_H 0x2D
//FIFO and Events not implemented yet
#define ITDS_STATUS_DETECT 0x37
#define ITDS_CTRL_7 0x3F

//sub-register definitions
//for CTRL_1
#define ITDS_NORMAL_14B 0x02
#define ITDS_HIGHPERFORMANCE_14B 0x06
#define ITDS_SINGLECONV_14B 0x0A
#define ITDS_SINGLECONV_12B 0x08
#define ITDS_LOWPOWER_12B 0x00

#define ITDS_ODR_0 0x00
#define ITDS_ODR_1 0x10
#define ITDS_ODR_2 0x20
#define ITDS_ODR_3 0x30
#define ITDS_ODR_4 0x40
#define ITDS_ODR_5 0x50
#define ITDS_ODR_6 0x60
#define ITDS_ODR_7 0x70
#define ITDS_ODR_8 0x80
#define ITDS_ODR_9 0x90


//For CTRL_6
#define ITDS_LOWNOISE_ON 0x04
#define ITDS_LOWNOISE_OFF 0x00

#define ITDS_BWDIV_2 0x00
#define ITDS_BWDIV_4 0x40
#define ITDS_BWDIV_10 0x80
#define ITDS_BWDIV_20 0xA0

#define ITDS_SCALE_2G 0x00
#define ITDS_SCALE_4G 0x10
#define ITDS_SCALE_8G 0x20
#define ITDS_SCALE_16G 0x30

#define ITDS_LOWPASS_ENABLE 0x00
#define ITDS_HIGHPASS_ENABLE 0x08

//structs
struct itds_data{double x; double y; double z;};//contains acceleration values in mg

//Function prototypes
HAL_StatusTypeDef itds_init(I2C_HandleTypeDef* hi2c, uint8_t mode, uint8_t bandwidth, uint8_t scale, uint8_t ODR, uint8_t noiseMode, uint8_t filter);
struct itds_data itds_readData(I2C_HandleTypeDef* hi2c, uint8_t scaleSetting, uint8_t modeSetting);
uint8_t itds_pollData(I2C_HandleTypeDef* hi2c);

#endif /* INC_ITDS_H_ */
