/*
 * AS5600Driver.c
 *
 *  Created on: Oct 25, 2025
 *      Author: Z440
 */


#include "AS5600Driver.h"

AS5600Handle_Typedef *AS5600_Create(I2C_HandleTypeDef *hi2c,
									uint8_t i2cAddr)
{
	AS5600Handle_Typedef *pAS = (AS5600Handle_Typedef *)calloc(1, sizeof(AS5600Handle_Typedef));

	if (pAS == NULL)
	{
		printf("calloc fail");
		return NULL;
	}

	pAS->hi2c = hi2c;
	pAS->I2CAddress = i2cAddr;


	return pAS;
}


void AS5600_ReadRawAngle(AS5600Handle_Typedef *pAS)
{
	uint8_t temp[2];
	HAL_I2C_Mem_Read(pAS->hi2c, (pAS->I2CAddress << 1), AS5600_REGISTER_RAW_ANGLE_HIGH, 1, temp, 2, 1000);

	pAS->RawAngle = 0x0FFF & ((temp[0] << 8) & temp[1]);
}



void AS5600_UpdateStatus(AS5600Handle_Typedef *pAS)
{
	uint8_t temp[1];
	HAL_I2C_Mem_Read(pAS->hi2c, (pAS->I2CAddress << 1), AS5600_REGISTER_STATUS, 1, temp, 1, 1000);

	pAS->Status.MagnetTooStrong = 0x0001 & (temp[0] >> AS5600_MH_BITSHIFT);
	pAS->Status.MagnetTooWeak 	= 0x0001 & (temp[0] >> AS5600_ML_BITSHIFT);
	pAS->Status.MagnetDetected 	= 0x0001 & (temp[0] >> AS5600_MD_BITSHIFT);
}


