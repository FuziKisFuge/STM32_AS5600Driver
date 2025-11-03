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
									TIM_HandleTypeDef *htim,
									uint8_t i2cAddr)
{
	AS5600Handle_Typedef *pAS = (AS5600Handle_Typedef *)calloc(1, sizeof(AS5600Handle_Typedef));

	if (pAS == NULL)
	{
		printf("calloc fail");
		return NULL;
	}

	pAS->hi2c = hi2c;
	pAS->htim = htim;
	pAS->I2CAddress = i2cAddr;


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

	HAL_StatusTypeDef stat;

	stat = HAL_I2C_Mem_Write(pAS->hi2c, (pAS->I2CAddress << 1), AS5600_REGISTER_CONF_HIGH, I2C_MEMADD_SIZE_8BIT, temp, 2, 1000);

	if (stat != HAL_OK)
	{
		while(1);
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
	HAL_I2C_Mem_Read(pAS->hi2c, (pAS->I2CAddress << 1), AS5600_REGISTER_STATUS, I2C_MEMADD_SIZE_8BIT, temp, 1, 1000);

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
	HAL_I2C_Mem_Read(pAS->hi2c, (pAS->I2CAddress << 1), AS5600_REGISTER_RAW_ANGLE_HIGH, I2C_MEMADD_SIZE_8BIT, temp, 2, 1000);

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
	HAL_I2C_Mem_Read(pAS->hi2c, (pAS->I2CAddress << 1), AS5600_REGISTER_ANGLE_HIGH, I2C_MEMADD_SIZE_8BIT, temp, 2, 1000);

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

	uint32_t CCR1_Value;
	uint32_t CCR2_Value;
	uint32_t PWMFreq;
	float DutyCycle;

	CCR1_Value = HAL_TIM_ReadCapturedValue(pAS->htim, TIM_CHANNEL_1);
	CCR2_Value = HAL_TIM_ReadCapturedValue(pAS->htim, TIM_CHANNEL_2);

	//HAL_RCC_GetSysClockFreq();

	PWMFreq = 48000000 / (CCR1_Value);
	DutyCycle = ((float)CCR2_Value/(float)CCR1_Value) * 100.0f;

	return AS5600_OK;

}









float MapDutycycle2Angle(float Duty, float AngleMin, float AngleMax)
{

}






