#ifndef ADC_H
#define ADC_H

#include "stm32f1xx_hal.h"
#include "Config.h"
#include "GPIO.h"

void ADC1_Init(void);
void checkTheBatteryCondition(void);
#endif
