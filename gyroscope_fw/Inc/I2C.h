#ifndef I2C_H_
#define I2C_H_
#include "common_defs.h"

// Объявляем хэндлы I2C
extern I2C_HandleTypeDef hi2c1;
//extern I2C_HandleTypeDef hi2c2;

// Функция инициализации всех I2C
void I2C_Init_All(void);

#endif /* I2C_H_ */