/*
 * AS5600Driver.c
 *
 *  Created on: Oct 25, 2025
 *      Author: Z440
 */


#include "AS5600Driver.h"

AS5600Handle_Typedef *AS5600_Create()
{
	AS5600Handle_Typedef *pAS = (AS5600Handle_Typedef *)calloc(1, sizeof(AS5600Handle_Typedef));

	if (pAS == NULL)
	{
		return NULL;
		printf("calloc fail");
		//test change a
	}

	return pAS;
}


