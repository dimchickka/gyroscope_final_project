#include "adc.h"

extern ADC_HandleTypeDef hadc1;

//static void initializeADCOnPin(void){
//    ADC_ChannelConfTypeDef sConfig = {0};
//    sConfig.Channel = ADC_CHANNEL_2;      // Используем PA2
//    sConfig.Rank = 1;                  // Первое (и единственное) в очереди
//    // Для измерения батареи через делитель лучше брать макс. время выборки
//    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5; 
//    
//    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
//        // Ошибка инициализации
//        Error_Handler();
//    }

//    // 1. Настраиваем канал 17 (VrefInt)
//    sConfig.Channel = ADC_CHANNEL_VREFINT; // В HAL для F1 это и есть 17 канал
//    sConfig.Rank = ADC_REGULAR_RANK_1;      // Если измеряешь по очереди, укажи нужный ранг
//    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5; // ВАЖНО: Vref требует долгой выборки!
//    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
//    
//}

void ADC1_Init(void) {
    __HAL_RCC_ADC1_CLK_ENABLE();
    // 1. Настройка базовых параметров АЦП
    hadc1.Instance = ADC1;
    // ОБЯЗАТЕЛЬНО: Обнуляем все поля, чтобы не было "мусора"
    memset(&hadc1.Init, 0, sizeof(hadc1.Init));
    
    hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;         // Опрашиваем только один канал
    hadc1.Init.ContinuousConvMode = DISABLE;           // Одиночное измерение (по запросу)
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;  // Запуск программно, а не по таймеру
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;        // Выравнивание данных (стандарт)
    hadc1.Init.NbrOfConversion = 1;                    // Сколько каналов в очереди

    if (HAL_ADC_Init(&hadc1) != HAL_OK) {
        isErrorFlag = true;
    }
    //initializeADCOnPin();
}

//float Get_Vbat(void) {
//    uint32_t raw_bat = 0;
//    uint32_t raw_vref = 0;
//    ADC_ChannelConfTypeDef sConfig = {0};

//    // --- ШАГ 1: Настраиваем hadc1 на канал PA1 (обычно это ADC_CHANNEL_1) ---
//    sConfig.Channel = ADC_CHANNEL_1; 
//    sConfig.Rank = ADC_REGULAR_RANK_1;
//    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5; // Для резисторов 10-50кОм хватит
//    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

//    HAL_ADC_Start(&hadc1);
//    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
//        raw_bat = HAL_ADC_GetValue(&hadc1);
//    }
//    HAL_ADC_Stop(&hadc1);

//    // --- ШАГ 2: Перенастраиваем ТУ ЖЕ hadc1 на канал VrefInt ---
//    sConfig.Channel = ADC_CHANNEL_VREFINT; // Внутренний эталон
//    sConfig.Rank = ADC_REGULAR_RANK_1;
//    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5; // Для Vref лучше ставить максимум
//    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

//    HAL_ADC_Start(&hadc1);
//    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
//        raw_vref = HAL_ADC_GetValue(&hadc1);
//    }
//    HAL_ADC_Stop(&hadc1);

//    return raw_bat
//}






//uint32_t Get_Battery_Raw(void) {
//    uint32_t rawValue = 0;

//    // 1. Включаем делитель напряжения (пин EN)
//    // Устанавливаем RESET (0), чтобы открыть транзистор (если P-канал)
//    HAL_GPIO_WritePin(ADC_BAT_EN_PORT, ADC_BAT_EN_PIN, GPIO_PIN_RESET); 
//    
//    // Даем время (2-5 мс), чтобы напряжение на делителе «устаканилось»
//    // и зарядило входную емкость АЦП через ваши резисторы.
//    HAL_Delay(5);

//    // 2. Запускаем преобразование АЦП
//    HAL_ADC_Start(&hadc1);

//    // 3. Ждем окончания измерения (таймаут 10мс)
//    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
//        // Читаем результат из регистра данных
//        rawValue = HAL_ADC_GetValue(&hadc1);
//    }

//    // 4. Останавливаем АЦП
//    HAL_ADC_Stop(&hadc1);

//    // 5. Выключаем делитель (переводим пин в High-Z для экономии)
//    // Устанавливаем SET (1), в режиме Open Drain это разрыв цепи.
//    HAL_GPIO_WritePin(ADC_BAT_EN_PORT, ADC_BAT_EN_PIN, GPIO_PIN_SET);
//    
//    
//    return rawValue; 
//}

//float Get_Battery_Voltage(void) {
//    uint32_t raw = Get_Battery_Raw();
//    
//    // Vref = 3.3V, ADC resolution = 12 bit (4095)
//    // Коэффициент делителя = (R1 + R2) / R2. Если резисторы равны, то это 2.0
//    //float voltage = ( (float)raw * 3.3f / 4095.0f ) * 2.0f;
//    float voltage = ( (float)raw_vbat * V_actual_vdd / 4095.0f ) * 2.0f;
//    return voltage;
//}

// Вспомогательная функция для смены канала и замера


uint32_t ADC_Read_Channel(uint32_t channel) {
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    // Для VrefInt и делителей с высокими резисторами всегда ставим максимум
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5; 
    
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
        return 0;
    }

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    uint32_t val = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    return val;
}

float Get_Battery_Voltage(void) {
    // 1. Включаем делитель (открываем транзистор)
    HAL_GPIO_WritePin(ADC_BAT_EN_PORT, ADC_BAT_EN_PIN, GPIO_PIN_RESET);
    
    // Даем время емкостям зарядиться (5 мс достаточно для резисторов до 100кОм)
    HAL_Delay(5);

    // 2. Делаем замеры
    // Сначала читаем нашу батарейку на PA1 (Channel 2)
    uint32_t raw_bat = ADC_Read_Channel(ADC_CHANNEL_2);
    
    // Затем читаем внутренний эталон (Channel 17)
    uint32_t raw_vref = ADC_Read_Channel(ADC_CHANNEL_VREFINT);

    // 3. Выключаем делитель для экономии энергии
    HAL_GPIO_WritePin(ADC_BAT_EN_PORT, ADC_BAT_EN_PIN, GPIO_PIN_SET);

    // Защита от деления на ноль, если АЦП вдруг не сработал
    if (raw_vref == 0) return 0.0f;

    // 4. РАСЧЕТЫ
    // Типичное значение VrefInt для STM32F103 составляет 1.21 Вольта
    const float VREFINT_VOLTAGE = 1.21f; 

    // Вычисляем реальное напряжение питания МК (Vdda)
    // Формула: Vdda = (VrefInt_типовое * 4095) / raw_vref
    float vdda_actual = (VREFINT_VOLTAGE * 4095.0f) / (float)raw_vref;

    // Вычисляем напряжение на пине PA1
    // Формула: Vpin = (raw_bat * vdda_actual) / 4095
    float v_pin = ((float)raw_bat * vdda_actual) / 4095.0f;

    // Итоговое напряжение батареи (умножаем на коэффициент делителя 2.0)
    return v_pin * 2.0f;
}


void checkTheBatteryCondition(void){
    float vBat = Get_Battery_Voltage();
    
    if(is_charging){
        if (vBat < 3.1f){
            isChargeOnBatteryEnough = false;
            blinkTheDiod(LED_BLUE_PORT, LED_BLUE_PIN);
        }
        else if(vBat < 4.15f){
            isChargeOnBatteryEnough = true;
            blinkTheDiod(LED_BLUE_PORT, LED_BLUE_PIN);
        }
     
        else{
            isChargeOnBatteryEnough = true;
            blinkTheDiod(LED_GREEN_PORT, LED_GREEN_PIN);
        }
    }
    else if(!is_charging && vBat <= 3.1f){
        HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, 1);
        isChargeOnBatteryEnough = false;
    }
    else if(!is_charging && vBat > 3.1f){
        HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PIN, 0);
        isChargeOnBatteryEnough = true;
    }
}

