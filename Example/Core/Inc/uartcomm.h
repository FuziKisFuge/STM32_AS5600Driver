/*
 * uartcomm.h
 *
 *  Created on: Oct 27, 2025
 *      Author: Z440
 */

#ifndef INC_UARTCOMM_H_
#define INC_UARTCOMM_H_

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

//extern UART_HandleTypeDef huart1;



void uart_printf(UART_HandleTypeDef *huart, const char *format, ...);
//void uart_printf(const char *format, ...);

#endif /* INC_UARTCOMM_H_ */
