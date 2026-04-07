//Библиотека для работы с SD картой
#include "SD.h"
sd_info_ptr sdinfo;
static uint8_t SD_cmd (uint8_t cmd, uint32_t arg, UART_HandleTypeDef* huart);
char str1[60]={0};

void SD_PowerOn(void){
  Timer2 = 0;
  //ждём 20 милисекунд, для того, чтобы напряжение стабилизировалось
  while(Timer2<2);
}


//-----------------------------------------------

uint8_t SD_Read_Block (uint8_t *buff, uint32_t lba, UART_HandleTypeDef* huart){

  uint8_t result;

  uint16_t cnt;
   
  result=SD_cmd (CMD17, lba, huart); //CMD17 даташит стр 50 и 96
  if (result!=0x00) return 5; //Выйти, если результат не 0x00  
    SPI_Release();

  cnt=0;
  do{ //Ждем начала блока
    result=SPI_ReceiveByte();
    cnt++;
  } while ( (result!=0xFE)&&(cnt<0xFFFF) );
  //0xFE = Data Start Token
  //В SPI-режиме каждый блок данных 512 байт начинается с этого байта

  if (cnt>=0xFFFF) return 5;
  for (cnt=0;cnt<512;cnt++) buff[cnt]=SPI_ReceiveByte(); //получаем байты блока из шины в буфер
  SPI_Release(); //Пропускаем контрольную сумму
  SPI_Release();

  return 0;

}

//-----------------------------------------------

//-----------------------------------------------

uint8_t SD_Write_Block (const uint8_t *buff, uint32_t lba, UART_HandleTypeDef* huart){
  //LBA (Logical Block Address) - номер блока. LBA указывает с какой позиции (блока) начинать чтение/запись.
  uint8_t result;
  uint16_t cnt;
    
  result=SD_cmd(CMD24,lba, huart); //CMD24 даташит стр 51 и 97-98
  if (result!=0x00) return 6; //Выйти, если результат не 0x00
  SPI_Release();
  SPI_SendByte (0xFE); //Начало буфера
  for (cnt=0;cnt<512;cnt++) SPI_SendByte(buff[cnt]); //Данные
  SPI_Release(); //Пропустим котрольную сумму
  SPI_Release();
  result=SPI_ReceiveByte();
  if ((result&0x05)!=0x05) return 6; //Выйти, если результат не 0x05 (Даташит стр 111)
  cnt=0;

  do { //Ждем окончания состояния BUSY
    result=SPI_ReceiveByte();
    cnt++;
  } while ( (result!=0xFF)&&(cnt<0xFFFF) );
  
  if (cnt>=0xFFFF) return 6;
  
    static const char msg_3[] = "good, we have witten the block\r\n";
    HAL_UART_Transmit(huart, (uint8_t*)msg_3, strlen(msg_3), I2C_TIMEOUT_MS);

  return 0;
}

//-----------------------------------------------

uint8_t sd_ini(UART_HandleTypeDef* huart){
    //Задача инициализации — определить тип нашей карты, которых, 
    // как мы знаем бывает несколько, 
    // а также перевести карту в обычный режим из режима IDLE.
    uint8_t i;
    //uint8_t cmd;
    uint8_t ocr[4]; //Operating Conditions Register (регистр условий работы)
    //Проще говоря: это 32-битное слово, в котором карта сообщает, 
    //при каких условиях она может работать и в каком состоянии она сейчас.
    
    int16_t tmr;
    //uint32_t temp;
    sdinfo.type = 0;
    static const char msg[] = "We have been here\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), I2C_TIMEOUT_MS);
    //По даташиту SD карты, чтобы она перешла в SPI режим, необходимо подать
    //не менее 74 тактов SCK при CS = 1 (карта не выбрана)

    SS_SD_DESELECT(); //Поднимаем CS = 1

    for(i=0;i<10;i++){ //80 импульсов (не менее 74) Даташит стр 91
       SPI_Release();
    }
    static const char msg_1[] = "We're here\r\n";
    HAL_UART_Transmit(huart, (uint8_t*)msg_1, strlen(msg_1), I2C_TIMEOUT_MS);
    //Возвращаем высокую скорость для SPI
    //SPI1_SetPrescaler(); //72 МГц/16 = 4.5 МГц
    
    if (SD_cmd(CMD0, 0, huart) == 1){ // Enter Idle state
        //MD0: успех = 1
        //Остальные команды: успех = 0
        static const char msg_2[] = "SD card in an Idle state, SUCCESS!!!\r\n";
        HAL_UART_Transmit(huart, (uint8_t*)msg_2, strlen(msg_2), I2C_TIMEOUT_MS);
        SPI_Release();
        
        if (SD_cmd(CMD8, 0x1AA, huart) == 1){ // SDv2
            //SD_cmd уже возвращает первый байт
            
            //4 байта дочитываются в цикле
            for (i = 0; i < 4; i++) ocr[i] = SPI_ReceiveByte();
            sprintf(str1,"OCR: 0x%02X 0x%02X 0x%02X 0x%02X\r\n",ocr[0],ocr[1],ocr[2],ocr[3]);
            HAL_UART_Transmit(&huart1,(uint8_t*)str1,strlen(str1),0x1000);
            
            // Get trailing return value of R7 resp
            if (ocr[2] == 0x01 && ocr[3] == 0xAA){ // The card can work at vdd range of 2.7-3.6V
                //Теперь нужно проверить High Capacity ли карта или нет
                //Т.к. если High Capacity, то у неё другой способ адресации данных,
                //а именно блочная адресация (по 512 байт)
                for (tmr = 12000; tmr && SD_cmd(ACMD41, 1UL << 30, huart); tmr--);
                //SD_cmd(...) == 0 -> Карта вышла из Idle

                
                if (tmr && SD_cmd(CMD58, 0, huart) == 0) { // Если таймаут tmr не истёк и 
                    //SD_cmd вернула ноль = корректный ответ
                    for (i = 0; i < 4; i++) ocr[i] = SPI_ReceiveByte();
                    //И наша задача посмотреть CCS bit in the OCR
                        
                    sprintf(str1,"OCR: 0x%02X 0x%02X 0x%02X 0x%02X\r\n",ocr[0],ocr[1],ocr[2],ocr[3]);
                    HAL_UART_Transmit(&huart1,(uint8_t*)str1,strlen(str1),0x1000);

                    sdinfo.type = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2; // SDv2 (HC or SC)
                        
                    sprintf(str1,"Type SD: 0x%02X\r\n",sdinfo.type);
                    HAL_UART_Transmit(&huart1,(uint8_t*)str1,strlen(str1),0x1000);
                }
            }
        }

        else{ //SDv1 or MMCv3 Это старые типы карт.
            //Для простоты готовое устройство просто не будет поддерживать эти типы
        }

    }
    else{
        static const char msg_3[] = "Unfortunatly, there's a problem with SD card. Code:111555\r\n";
        HAL_UART_Transmit(huart, (uint8_t*)msg_3, strlen(msg_3), I2C_TIMEOUT_MS);
        return 1;
    }

  return 0;
}

uint8_t SPIx_WriteRead(uint8_t Byte){
  uint8_t receivedbyte = 0;
  
  if(HAL_SPI_TransmitReceive(&hspi1,(uint8_t*) &Byte,(uint8_t*) &receivedbyte,1,0x1000)!=HAL_OK){
     blinkTheDiod(LED_GREEN_PORT, LED_GREEN_PIN);
  }
  return receivedbyte;

}

//-----------------------------------------------
void SPI_SendByte(uint8_t bt){
  SPIx_WriteRead(bt);
}


uint8_t SPI_ReceiveByte(void){
  uint8_t bt = SPIx_WriteRead(0xFF);
  return bt;
}

void SPI_Release(void){
    SPIx_WriteRead(0xFF);
}

//Функция передачи команды
//-----------------------------------------------
uint8_t SD_cmd(uint8_t cmd, uint32_t arg, UART_HandleTypeDef* huart)
{
    uint8_t n, res;
    (void)huart; // сейчас не используем

    // ACMD<n> = CMD55 + CMD<n>
    if (cmd & 0x80)
    {
        cmd &= 0x7F;
        res = SD_cmd(CMD55, 0, huart);
        if (res > 1) return res;
    }

    // Select the card (как в уроке)
    SS_SD_DESELECT();
    SPI_ReceiveByte();     // 8 тактов при CS=1

    SS_SD_SELECT();
    SPI_ReceiveByte();     // 8 тактов при CS=0

    SPI_ReceiveByte();     // ВАЖНО: третий прогон 0xFF перед командой (у тебя его не было)

    // Send a command packet (6 bytes)
    SPI_SendByte(cmd);
    SPI_SendByte((uint8_t)(arg >> 24));
    SPI_SendByte((uint8_t)(arg >> 16));
    SPI_SendByte((uint8_t)(arg >> 8));
    SPI_SendByte((uint8_t)arg);

    // CRC + Stop bit
    n = 0x01;
    if (cmd == CMD0) n = 0x95;
    if (cmd == CMD8) n = 0x87;
    SPI_SendByte(n);

    // Receive R1 response
    n = 10;
    do {
        res = SPI_ReceiveByte();
    } while ((res & 0x80) && --n);

    return res;
}
//-----------------------------------------------

uint32_t setTimeToReadAndIidexOfIteration(char* buff, uint32_t lba, UART_HandleTypeDef* huart) {
    uint32_t result = SUCCESS_EXECUTION;

    // Читаем блок. SD_Read_Block возвращает 0 при успехе
    if (SD_Read_Block((uint8_t*) buff, lba, huart) == 0) {
        //Ищем маркер "time"
        char* timePtr = strstr(buff, "time");
        if (timePtr != NULL) {
            // Сдвигаем указатель на длину слова "time" (4 символа), чтобы он указывал на цифры
            STEP_TO_READ_MS = strtoul(timePtr + 4, NULL, 10);
        } else {
            result = ERROR_EXECUTION; // Маркер не найден
        }
        //Ищем маркер "Index"
        char* indexPtr = strstr(buff, "Index");
        if (indexPtr != NULL) {
            // Сдвигаем на длину слова "Index" (5 символов)
            indexOfIteration = strtoul(indexPtr + 5, NULL, 10);
        } else {
            result = ERROR_EXECUTION; // Маркер не найден
        }
    } 
    else{
        result = ERROR_EXECUTION;
    }

    return result;
}
