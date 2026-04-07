#include "LED.h"

void blinkTheDiod(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
    HAL_Delay(1000);
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
    HAL_Delay(1000);
}