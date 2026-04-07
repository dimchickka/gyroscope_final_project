#include "main.h"
#include "stm32f1xx_it.h"
#include "Config.h"

void NMI_Handler(void)
{
    while (1) {}
}

void HardFault_Handler(void)
{
    while (1) {}
        HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, 1);
}

void MemManage_Handler(void)
{
    while (1) {}
}

void BusFault_Handler(void)
{
    while (1) {}
}

void UsageFault_Handler(void)
{
    while (1) {}
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void TIM2_IRQHandler(void){
  HAL_TIM_IRQHandler(&htim2);

}



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if(htim==&htim2){
        Timer2++;
        counterToCheckBatteryCondition++;
        if(!is_charging && (counterToCheckBatteryCondition == 100)){
            counterToCheckBatteryCondition = 0;
            isBatteryConditionNeedsToBeChecked = true; //проверка будет каждую секунду
        }
    }
}
