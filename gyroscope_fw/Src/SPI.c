#include "SPI.h"

/* === SPI1 handle === */
SPI_HandleTypeDef hspi1;
//SPI_HandleTypeDef hspi2; // если будет второй SPI

void SPI_Init_All(void)
{
    /* --- Включаем тактирование SPI и GPIO --- */
    __HAL_RCC_SPI1_CLK_ENABLE();
    //__HAL_RCC_SPI2_CLK_ENABLE(); // если нужен SPI2

    /* --- Настройка SPI1 --- */
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;               // Режим мастер
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;    // Full-duplex
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;        // 8 бит
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;      // Полярность тактового сигнала
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;          // Фаза
    hspi1.Init.NSS = SPI_NSS_SOFT;                  // Управление CS программно
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; // Скорость SPI
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;         // Передача старшего бита первым
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;         // Без режима TI
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE; // Без CRC
    hspi1.Init.CRCPolynomial = 7;

    HAL_SPI_Init(&hspi1);
 
}

//Ускоряем шину SPI
void SPI1_SetPrescaler()
{
    __HAL_SPI_DISABLE(&hspi1);          // остановить SPI
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    HAL_SPI_Init(&hspi1);              // применить новое значение
    __HAL_SPI_ENABLE(&hspi1);          // включить обратно (обычно HAL и так включает)
}

