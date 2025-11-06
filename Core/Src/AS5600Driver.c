/*
 * AS5600Driver.c
 *
 *  Created on: Oct 25, 2025
 *      Author: Füzi Gergő
 */


#include "AS5600Driver.h"


/**
  * @brief
  *
  * @param
  *
  * @retval
  */
AS5600Handle_Typedef *AS5600_Create(I2C_HandleTypeDef *hi2c,
									uint8_t i2cAddr,
									float MaxAngle,
									float MinAngle)
{
	if(MaxAngle > 360.0f)
	{
		printf("Invalid max angle");
		return NULL;
	}

	if(MaxAngle < 0.0f)
	{
		printf("Invalid min angle");
		return NULL;
	}

	if(MinAngle > MaxAngle)
	{
		printf("Invalid min/max angle");
		return NULL;
	}

	AS5600Handle_Typedef *pAS = (AS5600Handle_Typedef *)calloc(1, sizeof(AS5600Handle_Typedef));

	if (pAS == NULL)
	{
		printf("calloc fail");
		return NULL;
	}

	pAS->htim = (TIM_HandleTypeDef *)calloc(1, sizeof(TIM_HandleTypeDef));

	if (pAS->htim == NULL)
	{
		printf("calloc fail");
		return NULL;
	}


	pAS->hi2c = hi2c;
	pAS->I2CAddress = i2cAddr;
	pAS->MaxAngle = MaxAngle;
	pAS->MinAngle = MinAngle;


	return pAS;
}









/**
  * @brief
  *
  * @param
  *
  * @retval
  */
eInfo AS5600_Configure(AS5600Handle_Typedef *pAS,
							eConf_PowerMode PM,
							eConf_Hysteresis HYST,
							eConf_OutputStage OUTS,
							eConf_PWMFrequency PWMF,
							eConf_SlowFilter SF,
							eConf_FastFilterThreshold FTH,
							eConf_Watchdog WD)
{
	if(pAS == NULL)
	{
		return AS5600_NULL_POINTER;
	}

	pAS->Config.PowerMode = PM;
	pAS->Config.Hysteresis = HYST;
	pAS->Config.OutputStage = OUTS;
	pAS->Config.PWMFrequency = PWMF;
	pAS->Config.SlowFilter = SF;
	pAS->Config.FastFilterTreshold = FTH;
	pAS->Config.Watchdog = WD;

	if (pAS->Config.OutputStage == PWM)
	{
		AS5600_TimerInit(pAS);
		HAL_TIM_IC_Start(pAS->htim, TIM_CHANNEL_1);
		HAL_TIM_IC_Start(pAS->htim, TIM_CHANNEL_2);
	}

	uint16_t temp16_t = 0;

	temp16_t |= ((0x0001 & (uint16_t)pAS->Config.Watchdog) << 13);
	temp16_t |= ((0x0007 & (uint16_t)pAS->Config.FastFilterTreshold) << 10);
	temp16_t |= ((0x0003 & (uint16_t)pAS->Config.SlowFilter) << 8);
	temp16_t |= ((0x0003 & (uint16_t)pAS->Config.PWMFrequency) << 6);
	temp16_t |= ((0x0003 & (uint16_t)pAS->Config.OutputStage) << 4);
	temp16_t |= ((0x0003 & (uint16_t)pAS->Config.Hysteresis) << 2);
	temp16_t |= (0x0003 & (uint16_t)pAS->Config.PowerMode);

	uint8_t temp[2];

	temp[0] = (uint8_t)(0x00FF & (temp16_t >> 8));
	temp[1] = (uint8_t)(0x00FF & temp16_t);

	HAL_StatusTypeDef status;
	status = HAL_I2C_Mem_Write(pAS->hi2c, (pAS->I2CAddress << 1), AS5600_REGISTER_CONF_HIGH, I2C_MEMADD_SIZE_8BIT, temp, 2, 1000);

	if (status != HAL_OK)
	{
		return AS5600_I2C_COMM_ERROR;
	}

	return AS5600_OK;
}


/**
  * @brief
  *
  * @param
  *
  * @retval
  */
eInfo AS5600_UpdateStatus(AS5600Handle_Typedef *pAS)
{
	if(pAS == NULL)
	{
		return AS5600_NULL_POINTER;
	}

	uint8_t temp[1];

	HAL_StatusTypeDef status;
	status = HAL_I2C_Mem_Read(pAS->hi2c, (pAS->I2CAddress << 1), AS5600_REGISTER_STATUS, I2C_MEMADD_SIZE_8BIT, temp, 1, 1000);

	if (status != HAL_OK)
	{
		return AS5600_I2C_COMM_ERROR;
	}

	pAS->Status.MagnetTooStrong = 0x0001 & (temp[0] >> AS5600_MH_BITSHIFT);
	pAS->Status.MagnetTooWeak 	= 0x0001 & (temp[0] >> AS5600_ML_BITSHIFT);
	pAS->Status.MagnetDetected 	= 0x0001 & (temp[0] >> AS5600_MD_BITSHIFT);

	return AS5600_OK;
}







/**
  * @brief
  *
  * @param
  *
  * @retval
  */
eInfo AS5600_ReadRawAngle_I2C(AS5600Handle_Typedef *pAS)
{
	if(pAS == NULL)
	{
		return AS5600_NULL_POINTER;
	}

	uint8_t temp[2];

	HAL_StatusTypeDef status;
	status = HAL_I2C_Mem_Read(pAS->hi2c, (pAS->I2CAddress << 1), AS5600_REGISTER_RAW_ANGLE_HIGH, I2C_MEMADD_SIZE_8BIT, temp, 2, 1000);

	if (status != HAL_OK)
	{
		return AS5600_I2C_COMM_ERROR;
	}

	pAS->RawAngle = 0x0FFF & ((temp[0] << 8) & temp[1]);

	return AS5600_OK;
}







/**
  * @brief
  *
  * @param
  *
  * @retval
  */
eInfo AS5600_ReadAngle_I2C(AS5600Handle_Typedef *pAS)
{
	if(pAS == NULL)
	{
		return AS5600_NULL_POINTER;
	}

	uint8_t temp[2];

	HAL_StatusTypeDef status;
	status = HAL_I2C_Mem_Read(pAS->hi2c, (pAS->I2CAddress << 1), AS5600_REGISTER_ANGLE_HIGH, I2C_MEMADD_SIZE_8BIT, temp, 2, 1000);

	if(status != HAL_OK)
	{
		return AS5600_I2C_COMM_ERROR;
	}
	pAS->Angle = 0x0FFF & ((temp[0] << 8) | temp[1]);

	return AS5600_OK;
}





/**
  * @brief
  *
  * @param
  *
  * @retval
  */
eInfo AS5600_ReadAngle_PWM(AS5600Handle_Typedef *pAS)
{
	if(pAS == NULL)
	{
		return AS5600_NULL_POINTER;
	}

	if(pAS->Config.OutputStage != PWM)
	{
		return AS5600_PWM_MODE_NOT_INITIALIZED;
	}

	uint32_t CCR1_Value;
	uint32_t CCR2_Value;

	CCR1_Value = HAL_TIM_ReadCapturedValue(pAS->htim, TIM_CHANNEL_1);
	CCR2_Value = HAL_TIM_ReadCapturedValue(pAS->htim, TIM_CHANNEL_2);

	if(CCR1_Value == 0)
	{
		return AS5600_DEVIDE_BY_ZERO;
	}


	uint32_t PWMFreq;
	PWMFreq = 48000000 / (CCR1_Value);
	switch(pAS->Config.PWMFrequency)
	{
	case _115Hz:
		if(PWMFreq > 130 || PWMFreq < 100)
		{
			return AS5600_INPUT_PWM_FREQ_ERROR;
		}
		break;

	case _230Hz:
		if(PWMFreq > 260 || PWMFreq < 200)
		{
			return AS5600_INPUT_PWM_FREQ_ERROR;
		}
		break;

	case _460Hz:
		if(PWMFreq > 510 || PWMFreq < 410)
		{
			return AS5600_INPUT_PWM_FREQ_ERROR;
		}
		break;

	case _920Hz:
		if(PWMFreq > 1020 || PWMFreq < 820)
		{
			return AS5600_INPUT_PWM_FREQ_ERROR;
		}
		break;
	}



	float DutyCycle;
	DutyCycle = ((float)CCR2_Value/(float)CCR1_Value) * 100.0f;

	pAS->LastAngle = pAS->Angle;
	pAS->Angle = MapDutycycle2Angle(DutyCycle, pAS->MinAngle, pAS->MaxAngle);

	//HAL_RCC_GetSysClockFreq();

	return AS5600_OK;

}


eInfo AS5600_UpdateAbsolutePosition(AS5600Handle_Typedef *pAS)
{
	float LowLimit = pAS->MinAngle + 60.0f;
	float HighLimit = pAS->MaxAngle - 60.0f;

	switch(pAS->State)
	{
	case Wait:
		if(pAS->Angle > HighLimit)
		{
			pAS->State = WatchForOverflow;
		}
		else if(pAS->Angle < LowLimit)
		{
			pAS->State = WatchForUnderflow;
		}

		break;

	case WatchForOverflow:
		if(pAS->Angle < LowLimit)
		{
			pAS->FullRotationCounter ++;
			pAS->State = WatchForUnderflow;
		}
		else if(pAS->Angle > LowLimit && pAS->Angle < HighLimit)
		{
			pAS->State = Wait;
		}

		break;

	case WatchForUnderflow:
			if(pAS->Angle > HighLimit)
			{
				pAS->FullRotationCounter --;
				pAS->State = WatchForOverflow;
			}
			else if(pAS->Angle > LowLimit && pAS->Angle < HighLimit)
			{
				pAS->State = Wait;
			}

			break;
	}

	pAS->AbsolutePosition = (pAS->FullRotationCounter * pAS->MaxAngle) + pAS->Angle;



	return AS5600_OK;
}







/**
  * @brief
  *
  * @param
  *
  * @retval
  */
float MapDutycycle2Angle(float Duty, float AngleMin, float AngleMax)
{
	//float MinVal = 2.9418f;			// 128 high clock/all clock = 128/4351
	//float MaxVal = 97.0519f;		//100.0f - MinVal

	//
	float PosVal = Duty - (DUTYCYCLE_128_CLOCK);			//Offset
	PosVal = PosVal * (100.0f / DUTYCYCLE_4095_CLOCK);		//Scale
	PosVal = PosVal / 100.0f;								//Normalize


	float PosAngle = (PosVal * (AngleMax - AngleMin)) + AngleMin;
	return PosAngle;
}






