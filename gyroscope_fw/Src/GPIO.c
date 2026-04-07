#include "GPIO.h"

/* ===========================================================
 * Включение тактирования GPIO
 * =========================================================== */
static void GPIO_EnableClocks(void){
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();  //для usart1 и для SPI
    __HAL_RCC_GPIOB_CLK_ENABLE(); //для hi2c1  pins PB6 and PB7
}

void GPIO_Init_All(void){
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_SWJ_NOJTAG();
    
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* ---------- Включаем тактирование ---------- */
    GPIO_EnableClocks();
    
    /* =======================================================
     *                LED BLUE
     * ======================================================= */
    GPIO_InitStruct.Pin = LED_BLUE_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_BLUE_PORT, &GPIO_InitStruct);
    
    /* =======================================================
     *                LED RED
     * ======================================================= */
    GPIO_InitStruct.Pin = LED_RED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_RED_PORT, &GPIO_InitStruct);
    
    /* =======================================================
     *                LED GREEN
     * ======================================================= */
     
    GPIO_InitStruct.Pin = LED_GREEN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GREEN_PORT, &GPIO_InitStruct);
    
    /* === USART1 TX === */
    GPIO_InitStruct.Pin = USART1_TX_PIN;    // PA9
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // Альтернативная функция push-pull
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(USART1_TX_PORT, &GPIO_InitStruct);

    /* === USART1 RX === */
    GPIO_InitStruct.Pin = USART1_RX_PIN;   // PA10
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // Вход для RX
    GPIO_InitStruct.Pull = GPIO_NOPULL;     // без подтяжки
    HAL_GPIO_Init(USART1_RX_PORT, &GPIO_InitStruct);

    /* === I2C1 SCL/SDA === */
    GPIO_InitStruct.Pin = I2C1_SCL_PIN | I2C1_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;    // Open-drain для I2C
    GPIO_InitStruct.Pull = GPIO_PULLUP;        // подтяжка к VCC
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C1_PORT, &GPIO_InitStruct);
    
    /* SPI1 SCK и MOSI */
    GPIO_InitStruct.Pin = SPI1_SCK_PIN | SPI1_MOSI_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SPI1_PORT, &GPIO_InitStruct);

    /* SPI1 MISO */
    GPIO_InitStruct.Pin = SPI1_MISO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(SPI1_PORT, &GPIO_InitStruct);

    /* Программный CS */
    GPIO_InitStruct.Pin = SPI1_CS_PIN; // например PA4
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;// подтяжка к VCC. SD карта будет выбрана при старте
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SPI1_PORT, &GPIO_InitStruct);
    
    /* SD Power Enable. Может не настраиваться, т.к. на схеме присутствует подтяжка к +3.3В */
    GPIO_InitStruct.Pin = SD_PWR_EN_PIN; // например PA3
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;// подтяжка к VCC. SD карта не должна быть выбрана при старте
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    
    /*IS_CHARGING */
    GPIO_InitStruct.Pin = IS_CHARGING_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(IS_CHARGING_PORT, &GPIO_InitStruct);

    
    /* ADC BAT INPUT */
    GPIO_InitStruct.Pin = ADC_BAT_IN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ADC_BAT_IN_PORT, &GPIO_InitStruct);
    
    /* ADC BAT ENABLE */
    GPIO_InitStruct.Pin = ADC_BAT_EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ADC_BAT_IN_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(ADC_BAT_EN_PORT, ADC_BAT_EN_PIN, GPIO_PIN_SET); //High-Z состояние
}