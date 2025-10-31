/*
 * AS5600Driver.h
 *
 *  Created on: Oct 25, 2025
 *      Author: Z440
 */

#ifndef INC_AS5600DRIVER_H_
#define INC_AS5600DRIVER_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

#define __IO	volatile

#include <stdarg.h>
#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#if defined(STM32F091xC) || defined(STM32F098xx) || defined(STM32F042x6) || defined(STM32F048xx) || \
    defined(STM32F030x6) || defined(STM32F031x6) || defined(STM32F038xx) || defined(STM32F070x6) || \
    defined(STM32F070xB) || defined(STM32F030x6) || defined(STM32F051x8)


#include "stm32f0xx_hal.h"


#endif

//Regiszter címek
#define AS5600_REGISTER_ZMCO 0x00
#define AS5600_REGISTER_ZPOS_HIGH 0x01
#define AS5600_REGISTER_ZPOS_LOW 0x02
#define AS5600_REGISTER_MPOS_HIGH 0x03
#define AS5600_REGISTER_MPOS_LOW 0x04
#define AS5600_REGISTER_MANG_HIGH 0x05
#define AS5600_REGISTER_MANG_LOW 0x06
#define AS5600_REGISTER_CONF_HIGH 0x07
#define AS5600_REGISTER_CONF_LOW 0x08

#define AS5600_REGISTER_RAW_ANGLE_HIGH 0x0C
#define AS5600_REGISTER_RAW_ANGLE_LOW 0x0D
#define AS5600_REGISTER_ANGLE_HIGH 0x0E
#define AS5600_REGISTER_ANGLE_LOW 0x0F

#define AS5600_REGISTER_STATUS 0x0B
#define AS5600_REGISTER_AGC 0x1A
#define AS5600_REGISTER_MAGNITUDE_HIGH 0x1B
#define AS5600_REGISTER_MAGNITUDE_LOW 0x1C
#define AS5600_REGISTER_BURN 0xFF

//Bit poziciók
#define AS5600_MD_BITSHIFT	5
#define AS5600_ML_BITSHIFT	4
#define AS5600_MH_BITSHIFT	3

//Bitmaszkok
#define 	AS5600_RAW_ANGLE_UPPER_8_BIT_MASK	0b00001111
#define 	AS5600_RAW_ANGLE_LOWER_8_BIT_MASK	0b11111111
#define		AS5600_RAW_ANGLE_16_BIT_MASK		0b0000111111111111

#define 	AS5600_MD_BIT_MASK	0b00100000
#define 	AS5600_ML_BIT_MASK	0b00010000
#define 	AS5600_MH_BIT_MASK	0b00001000

typedef struct
{
  __IO uint8_t ZMCO;
  __IO uint8_t ZPOS2;
  __IO uint8_t ZPOS1;
  __IO uint8_t MPOS2;
  __IO uint8_t MPOS1;
  __IO uint8_t MANG2;
  __IO uint8_t MANG1;
  __IO uint8_t CONF2;
  __IO uint8_t CONF1;
  __IO uint8_t RAW_ANGLE2;
  __IO uint8_t RAW_ANGLE1;
  __IO uint8_t ANGLE2;
  __IO uint8_t ANGLE1;
  __IO uint8_t STATUS;
  __IO uint8_t AGC;
  __IO uint8_t MAGNITUDE2;
  __IO uint8_t MAGNITUDE1;
  __IO uint8_t BURN;
} AS5600Register_TypeDef;

typedef struct{
	uint8_t MagnetTooStrong;
	uint8_t MagnetTooWeak;
	uint8_t MagnetDetected;

}AS5600Status_Typedef;

typedef struct{
	uint8_t PowerMode;
	uint8_t Hysteresis;
	uint8_t OutputStage;
	uint8_t PWMFrequency;
	uint8_t SlowFilter;
	uint8_t FastFilterTreshold;
	uint8_t Watchdog;

}AS5600Config_Typedef;

typedef struct{
	I2C_HandleTypeDef *hi2c;
	uint8_t I2CAddress;
	uint16_t RawAngle;
	uint16_t Angle;
	uint64_t AbsolutePosition;

	AS5600Status_Typedef Status;
	AS5600Config_Typedef Config;

}AS5600Handle_Typedef;



AS5600Handle_Typedef *AS5600_Create(I2C_HandleTypeDef *hi2c,uint8_t i2cAddr);
void AS5600_ReadRawAngle(AS5600Handle_Typedef *pAS);
void AS5600_ReadAngle(AS5600Handle_Typedef *pAS);
void AS5600_UpdateStatus(AS5600Handle_Typedef *pAS);

#endif /* INC_AS5600DRIVER_H_ */
