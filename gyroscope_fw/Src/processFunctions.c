#include "processFunctions.h"

bool is_first_iteration = true;
extern uint32_t numberOfPoints;
static uint16_t appendLine(const char* line, uint16_t len, char* buffer);
static uint32_t initializedBuffer(char* buffer);
static uint16_t buf_pos = 0;            // текущая позиция в буфере (0..511)
static uint32_t current_sector = START_SECTOR;

uint16_t processCurrentData(GyroData_t* gyroData, char* buffer){
    if(is_first_iteration){
         initializedBuffer(buffer);
         is_first_iteration = false;
    }
    char line[32];
    int n = snprintf(line, sizeof(line), "t%dd%deod\n", timeFromTheStartOfMCU, (int)gyroData->x_current);
    if (n <= 0) return ERROR_EXECUTION;
    if (n >= (int)sizeof(line)) n = sizeof(line);

    // 2) Добавляем в секторный буфер или записываем на карту
    uint16_t res = appendLine(line, (uint16_t)n, buffer);
    return res;           
}


static uint16_t appendLine(const char* line, uint16_t len, char* buffer){
    uint32_t result = SUCCESS_EXECUTION;
    if (len + 1 > SECTOR_SIZE) return 1; // +1 под '\n'

    if ((uint32_t)buf_pos + len > SECTOR_SIZE){
        // добить хвост нулями (опционально, но правильно)
        for (uint16_t j = buf_pos; j < SECTOR_SIZE; j++) buffer[j] = 0;
        
        if (SD_Write_Block((uint8_t*) buffer, current_sector, &huart1)) return ERROR_EXECUTION;

        current_sector++;
        if(initializedBuffer(buffer)){result = ERROR_EXECUTION;}
    }

    // копируем строку
    for (uint16_t i = 0; i < len; i++)
        buffer[buf_pos + i] = (uint8_t)line[i];
    
    buf_pos += len;

    return result;
}

static uint32_t initializedBuffer(char* buffer) {
    uint32_t result = SUCCESS_EXECUTION;
    
    if(!is_first_iteration){
        memset(buffer, 0, SECTOR_SIZE); //обнуление всего сектора
    }

    int written = sprintf(buffer, "Index%lu", (unsigned long)indexOfIteration);

    // 3. Обновляем позицию указателя для дальнейшей записи
    // Теперь buf_pos указывает на первый свободный нулевой байт после числа
    if (written > 0) {
        buf_pos = (uint16_t)written;
    } else {
        result = ERROR_EXECUTION;
    }
    
    return result;
}

void checkIsDeviceCharging(){
    if(!HAL_GPIO_ReadPin(IS_CHARGING_PORT, IS_CHARGING_PIN)){
            if(!is_charging){
                is_charging = true;
            }
     }
    else{
        if(is_charging){
            is_charging = false;
        }
    }
}


void processAlgorithm(char* buffer, GyroData_t* data){
    //проверка: заряжается ли устройство?
     checkIsDeviceCharging();
    
    
    if(!isChargeOnBatteryEnough && !is_charging){
        //Если заряда на батареи не достаточно, то красный сетодиод будет гореть постоянно
       //blinkTheDiod(LED_BLUE_PORT, LED_BLUE_PIN);
       if(isBatteryConditionNeedsToBeChecked) checkTheBatteryCondition();
    }
    
    
    else if(isErrorFlag == ERROR_EXECUTION){
        blinkTheDiod(LED_RED_PORT, LED_RED_PIN);
        isErrorFlag = false; //После мигания светодиодом проверяем, а не работает ли уже
    }
    else{
        if(isBatteryConditionNeedsToBeChecked) checkTheBatteryCondition();
        if(is_first_iteration){
            checkTheBatteryCondition(); //После включения обязательно проверяем батарейку
            if(isChargeOnBatteryEnough) isErrorFlag = setTimeToReadAndIidexOfIteration(buffer,CONFIG_SECTOR, &huart1);
        }
        
        if(!isErrorFlag && isChargeOnBatteryEnough){
            //После вызова данной функции в структуре gyroData появляются current значения угловой скорости
            startGettingData(&hi2c1,data);
            if(processCurrentData(data, buffer) == SUCCESS_EXECUTION){
                HAL_Delay(STEP_TO_READ_MS - 1);
                numberOfPoints++;
                if(numberOfPoints == 430){
                    volatile uint32_t tam = 5;
                    tam++;
                }
                if(numberOfPoints == 580){
                    volatile uint32_t tam = 5;
                    tam++;
                }
            }
            else{
                isErrorFlag = ERROR_EXECUTION;
            }
        }
        else{
            if(isErrorFlag) isErrorFlag = ERROR_EXECUTION;
        }
     }
}
