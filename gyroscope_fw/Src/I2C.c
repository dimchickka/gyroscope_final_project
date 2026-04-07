#include "I2C.h"

/* === I2C1 handle === */
I2C_HandleTypeDef hi2c1;

/* === I2C2 handle === */
//I2C_HandleTypeDef hi2c2;

void I2C_Init_All(void)
{
    /* --- Включаем тактирование I2C и GPIO --- */
    __HAL_RCC_I2C1_CLK_ENABLE();
    __HAL_RCC_I2C2_CLK_ENABLE();


    /* --- Настройка I2C1 --- */
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;            // 100 kHz
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&hi2c1);
}
