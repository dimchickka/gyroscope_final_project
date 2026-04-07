#ifndef CONFIG_H
#define CONFIG_H


#include "USART.h"
#include "I2C.h"
#include "l3g4200d.h"
#include "adc.h"
#include "processFunctions.h"
#include "GPIO.h"
#include "LED.h"
#include "SPI.h"
#include "Timers.h"
#include "SD.h"
#include "common_defs.h"


//-----------------------//Declare Functions//------------
void SystemClock_Config(void);
void Error_Handler(void);
void MCU_Init(void);

#endif
