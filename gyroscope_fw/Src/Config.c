#include "Config.h"
#include "main.h"

uint32_t timeFromTheStartOfMCU = 0;
uint32_t indexOfIteration = 131;
bool isErrorFlag = SUCCESS_EXECUTION;
bool isFirstStep = true;
bool is_charging = false;
bool isBatteryConditionNeedsToBeChecked = false;

void MCU_Init(void){
    HAL_Init();
    SystemClock_Config();
    GPIO_Init_All();
    ADC1_Init();
    UART_Init_All();
    I2C_Init_All();
    SPI_Init_All(); 
    MX_TIM2_Init();
    
    if(Gyro_Init(&hi2c1, &huart1) == ERROR){isErrorFlag = 1;}
    HAL_TIM_Base_Start_IT(&htim2); //Старт таймера
    
   HAL_Delay(STEP_TO_READ_MS);
   
    SD_PowerOn();
    if(sd_ini(&huart1) == ERROR) {isErrorFlag = 1;}
}


void SystemClock_Config(void){

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;


    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK){
        Error_Handler();
    }
}

void Error_Handler(void){
    __disable_irq();
    while (1){}
}
