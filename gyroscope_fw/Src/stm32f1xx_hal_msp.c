#include "main.h"

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base){
  if(htim_base->Instance==TIM2){
    __HAL_RCC_TIM2_CLK_ENABLE();
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  }

}