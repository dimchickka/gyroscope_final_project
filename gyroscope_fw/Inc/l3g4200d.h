#ifndef L3G4200D_H
#define L3G4200D_H

#include "common_defs.h"

//registors
#define L3G4200D_ADDRESS 0x68 << 1 //if SDO = LOW 
#define L3G4200D_WhO_AM_I 0x0F
#define L3G4200D_CTRL_REG1 0x20
#define L3G4200D_CTRL_REG4 0x23
#define L3G4200D_OUT_X_L 0x28

// Configuration values
#define L3G4200D_CTRL_REG1_CONFIG 0x7F // Normal mode, all axes enabled, 200Hz, 70Hz cutoff
#define L3G4200D_CTRL_REG4_CONFIG 0x20  // 2000 dps full scale


#define PI 3.14
#define HALF_OF_CIRCLE_IN_DEGREES 180




uint8_t Gyro_Init(I2C_HandleTypeDef *hi2c, UART_HandleTypeDef* huart);
void Gyro_Read_Velocity(I2C_HandleTypeDef *hi2c, GyroData_t* data);
void prepareStructure(GyroData_t* data);
void calculateAcceleration(GyroData_t* data, float DT);
void startGettingData(I2C_HandleTypeDef *hi2c, GyroData_t* data);
void Gyro_Configure(I2C_HandleTypeDef *hi2c);
uint32_t setTimeToRead(uint8_t *buff, uint32_t lba, UART_HandleTypeDef* huart);
#endif