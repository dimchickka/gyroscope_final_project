#include "main.h"

char buffer[512]; //Буфер данных для записи/чтения
uint32_t numberOfPoints = 0;
volatile uint16_t Timer2=0; //Глобальная переменная для таймера-счётчика
volatile uint16_t counterToCheckBatteryCondition = 0;
bool isChargeOnBatteryEnough = true;
ADC_HandleTypeDef hadc1;
TIM_HandleTypeDef htim2;

int main(void){
    HAL_GPIO_WritePin(SD_PWR_EN_PORT, SD_PWR_EN_PIN, 0);
    MCU_Init();
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, 1);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, 0);
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, 1);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, 0);
    HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, 1);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PIN, 0);
    GyroData_t gyroData = {0};
    if(isErrorFlag) HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PIN, 1);
    while (1){
       processAlgorithm(buffer, &gyroData);
     }
}

