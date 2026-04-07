#ifndef UART_H_
#define UART_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"
#include "Config.h"

// Здесь можно добавить declare всех UART, если их много
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

// Функция инициализации всех UART ь
void UART_Init_All(void);
void uartPrint(const char *s);

#endif /* UART_H_ */