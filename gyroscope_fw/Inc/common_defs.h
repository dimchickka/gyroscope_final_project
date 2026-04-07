#ifndef COMMON_DEFS_H
#define COMMON_DEFS_H

#include <stdint.h> 
#include <string.h> 
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32f1xx_hal.h"

//-----------------------//CONFIGURATION DATA//------------

extern uint32_t timeFromTheStartOfMCU;
extern uint32_t indexOfIteration;

//-----------------------//FLAGS//------------

extern bool isErrorFlag;
extern bool isFirstStep;
extern bool is_charging;
extern bool is_first_iteration;

extern bool isChargeOnBatteryEnough;
extern bool isBatteryConditionNeedsToBeChecked;
//-----------------------//RESULT OF OPERATIONS//------------

#define SUCCESS_EXECUTION 0
#define ERROR_EXECUTION 1

//-----------------------//ADDRESSES OF SD CARD//------------

#define CONFIG_SECTOR 0x1000U  //4096 сектор
#define START_SECTOR 0x1001U   //4097 сектор
//—читаем, что подключена SD карта размером 30√б
//—читаем, что доступно 62 млн. секторов
#define END_SECTOR 0x03B1E6C0U
#define SECTOR_SIZE 512U

//-----------------------//GPIO PINS//------------
//*****LED for indication*****
#define LED_RED_PORT   GPIOB
#define LED_RED_PIN    GPIO_PIN_1

#define LED_GREEN_PORT    GPIOB
#define LED_GREEN_PIN     GPIO_PIN_2

#define LED_BLUE_PORT  GPIOB
#define LED_BLUE_PIN   GPIO_PIN_3

//*****System pins*****
#define SD_PWR_EN_PORT  GPIOA      
#define SD_PWR_EN_PIN   GPIO_PIN_3

#define ADC_BAT_EN_PORT GPIOA
#define ADC_BAT_EN_PIN  GPIO_PIN_12

#define ADC_BAT_IN_PORT  GPIOA
#define ADC_BAT_IN_PIN   GPIO_PIN_2

#define IS_CHARGING_PORT  GPIOB
#define IS_CHARGING_PIN   GPIO_PIN_4

//*****USART1*****
#define USART1_TX_PORT      GPIOA
#define USART1_TX_PIN       GPIO_PIN_9
#define USART1_RX_PORT      GPIOA
#define USART1_RX_PIN       GPIO_PIN_10

//*****I2C*****
#define I2C1_PORT    GPIOB
#define I2C1_SCL_PIN GPIO_PIN_6
#define I2C1_SDA_PIN GPIO_PIN_7

//*****SPI1*****
#define SPI1_PORT    GPIOA
#define SPI1_SCK_PIN GPIO_PIN_5
#define SPI1_MOSI_PIN GPIO_PIN_7
#define SPI1_MISO_PIN GPIO_PIN_6
#define SPI1_CS_PIN GPIO_PIN_4

//-----------------------//Gyroscope//------------

extern uint32_t STEP_TO_READ_MS;
#define I2C_TIMEOUT_MS 10  // мс
#define UART_TIMEOUT_MKS 10  // мкс
#define STEP_TO_READ_S 0.01f // с

typedef struct {
    int16_t x_prev, y_prev, z_prev;
    int16_t x_current, y_current, z_current;
    float ax, ay, az;   // угловое ускорение
} GyroData_t;


//-----------------------//ERROR HANDLER//------------

extern void Error_Handler(void);


#endif