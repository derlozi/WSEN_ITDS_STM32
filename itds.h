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
#define ITDS_T_OUT_L 0x0D
#define ITDS_T_OUT_H 0x0E
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

//For CTRL_4
#define ITDS_INT0_6D 0x80
#define ITDS_INT0_SINGLE_TAP 0x40
#define ITDS_INT0_WU 0x20
#define ITDS_INT0_FF 0x10
#define ITDS_INT0_TAP 0x08
#define ITDS_INT0_DIFF5 0x04
#define ITDS_INT0_FTH 0x02
#define ITDS_INT0_DRDY 0x01
//For CTRL_5
#define ITDS_INT1_SLEEP_STATE 0x80
#define ITDS_INT1_SLEEP_CHG 0x40
#define ITDS_INT1_BOOT 0x20
#define ITDS_INT1_DRDY_T 0x10
#define ITDS_INT1_OVR 0x08
#define ITDS_INT1_DIFF5 0x04
#define ITDS_INT1_FTH 0x02
#define ITDS_INT1_DRDY 0x01
//for STATUS
#define ITDS_STATUS_FIFO_THS 0x80
#define ITDS_STATUS_WU_IA 0x40
#define ITDS_STATUS_SLEEP_STATE 0x20
#define ITDS_STATUS_DOUBLE_TAP 0x10
#define ITDS_STATUS_SINGLE_TAP 0x08
#define ITDS_STATUS_6D_IA 0x04
#define ITDS_STATUS_FF_IA 0x02
#define ITDS_STATUS_DRDY 0x01
//For CTRL_6
#define ITDS_LOWNOISE_ON 0x04
#define ITDS_LOWNOISE_OFF 0x00

#define ITDS_BWDIV_2 0x00
#define ITDS_BWDIV_4 0x40
#define ITDS_BWDIV_10 0x80
#define ITDS_BWDIV_20 0xC0

#define ITDS_SCALE_2G 0x00
#define ITDS_SCALE_4G 0x10
#define ITDS_SCALE_8G 0x20
#define ITDS_SCALE_16G 0x30

#define ITDS_LOWPASS_ENABLE 0x00
#define ITDS_HIGHPASS_ENABLE 0x08

//other definitions
#define ITDS_MASKOR 0
#define ITDS_MASKAND 1

//structs
struct itds_data{double x; double y; double z;};//contains acceleration values in mg

//Function prototypes
HAL_StatusTypeDef itds_init(I2C_HandleTypeDef* hi2c, uint8_t mode, uint8_t bandwidth, uint8_t scale, uint8_t ODR, uint8_t noiseMode, uint8_t filter);
struct itds_data itds_readData(I2C_HandleTypeDef* hi2c, uint8_t scaleSetting, uint8_t modeSetting);
uint8_t itds_pollData(I2C_HandleTypeDef* hi2c);
void itds_startConversion(I2C_HandleTypeDef* hi2c);
double itds_getTemperature(I2C_HandleTypeDef* hi2c);
void routeInterrupts(I2C_HandleTypeDef* hi2c, uint8_t interrupt, uint8_t interrupts);
uint8_t checkStatus(I2C_HandleTypeDef hi2c, uint8_t statusbit);

#endif /* INC_ITDS_H_ */
