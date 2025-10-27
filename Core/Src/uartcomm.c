/*
 * uartcomm.c
 *
 *  Created on: Oct 27, 2025
 *      Author: Z440
 */

#include "uartcomm.h"


void uart_printf(UART_HandleTypeDef *huart, const char *format, ...) {
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    HAL_UART_Transmit_IT(huart, (uint8_t*)buffer, strlen(buffer));
}

/*
void uart_printf(const char *format, ...) {
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    HAL_UART_Transmit_IT(&huart1, (uint8_t*)buffer, strlen(buffer));
}
*/
