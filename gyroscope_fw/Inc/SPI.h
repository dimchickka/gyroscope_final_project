#ifndef SPI_H
#define SPI_H

#include "stm32f1xx_hal.h"

/* === SPI handles === */
extern SPI_HandleTypeDef hspi1;
//extern SPI_HandleTypeDef hspi2;

void SPI_Init_All(void);
void SPI1_SetPrescaler(void);
void SPI1_SetPrescaler();

#endif