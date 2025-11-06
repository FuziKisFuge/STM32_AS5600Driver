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
#include "stm32f0xx_hal_tim.h"
#include "stm32f0xx_hal_tim.h"
#include "stm32f0xx_hal_tim_ex.h"
#include "stm32f0xx_hal_conf.h"

#endif

#ifndef HAL_TIM_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
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



//Egyéb define
#define DUTYCYCLE_128_CLOCK 2.9418524477f  // 128/4351
#define DUTYCYCLE_4095_CLOCK 94.1162951f	//4095/4351

typedef enum{
	AS5600_OK,
	AS5600_NULL_POINTER,
	AS5600_I2C_COMM_ERROR,
	AS5600_PWM_MODE_NOT_INITIALIZED,
	AS5600_INPUT_PWM_FREQ_ERROR,
	AS5600_INPUT_PWM_DUTYCYCLE_ERROR,
	AS5600_DEVIDE_BY_ZERO,


	AS5600_OTHER_ERROR,
}eInfo;

typedef enum{
	NOM = 0,
	LPM1 = 1,
	LPM2 = 2,
	LPM3 = 3
}eConf_PowerMode;

typedef enum{
	OFF = 0,
	LSB1 = 1,
	LSB2 = 2,
	LSB3 = 3
}eConf_Hysteresis;

typedef enum{
	AnalogFullRange = 0,
	AnalogReducedRange = 1,
	PWM = 2
}eConf_OutputStage;

typedef enum{
	_115Hz = 0,
	_230Hz = 1,
	_460Hz = 2,
	_920Hz = 3
}eConf_PWMFrequency;

typedef enum{
	_16x = 0,
	_8x = 1,
	_4x = 2,
	_2x = 3
}eConf_SlowFilter;

typedef enum{
	SlowFilterOnly = 0,
	LSB6 = 1,
	LSB7 = 2,
	LSB9 = 3,
	LSB18 = 4,
	LSB21 = 5,
	LSB24 = 6,
	LSB10 = 7
}eConf_FastFilterThreshold;

typedef enum{
	Off = 0,
	On = 1,
}eConf_Watchdog;

typedef enum{
	Wait,
	WatchForOverflow,
	WatchForUnderflow,
}RotationStateMachine;


typedef struct{
	uint8_t MagnetTooStrong;
	uint8_t MagnetTooWeak;
	uint8_t MagnetDetected;

}AS5600Status_Typedef;

typedef struct{
	eConf_PowerMode PowerMode;
	eConf_Hysteresis Hysteresis;
	eConf_OutputStage OutputStage;
	eConf_PWMFrequency PWMFrequency;
	eConf_SlowFilter SlowFilter;
	eConf_FastFilterThreshold FastFilterTreshold;
	eConf_Watchdog Watchdog;

}AS5600Config_Typedef;

typedef struct{
	I2C_HandleTypeDef *hi2c;
	TIM_HandleTypeDef *htim;
	uint8_t I2CAddress;
	float MaxAngle;
	float MinAngle;
	uint16_t RawAngle;
	float Angle;
	float LastAngle;
	int64_t FullRotationCounter;
	float AbsolutePosition;
	RotationStateMachine State;

	AS5600Status_Typedef Status;
	AS5600Config_Typedef Config;

}AS5600Handle_Typedef;



AS5600Handle_Typedef *AS5600_Create(I2C_HandleTypeDef *hi2c,
									TIM_HandleTypeDef *htim,
									uint8_t i2cAddr,
									float MaxAngle,
									float MinAngle);
eInfo AS5600_Configure(AS5600Handle_Typedef *pAS,
							eConf_PowerMode PM,
							eConf_Hysteresis HYST,
							eConf_OutputStage OUTS,
							eConf_PWMFrequency PWMF,
							eConf_SlowFilter SF,
							eConf_FastFilterThreshold FTH,
							eConf_Watchdog WD);
eInfo AS5600_UpdateStatus(AS5600Handle_Typedef *pAS);

eInfo AS5600_ReadRawAngle_I2C(AS5600Handle_Typedef *pAS);
eInfo AS5600_ReadAngle_I2C(AS5600Handle_Typedef *pAS);
eInfo AS5600_ReadAngle_PWM(AS5600Handle_Typedef *pAS);
eInfo AS5600_UpdateAbsolutePosition(AS5600Handle_Typedef *pAS);


float MapDutycycle2Angle(float Duty, float AngleMin, float AngleMax);



//--------------PERIPHERAL----------------
void AS5600_TimerInit(AS5600Handle_Typedef *pAS);
void AS5600_ErrorHandler();


#endif /* INC_AS5600DRIVER_H_ */
