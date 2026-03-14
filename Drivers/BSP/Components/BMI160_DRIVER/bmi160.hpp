#ifndef BMI160_H
#define BMI160_H

#include "stm32f4xx_hal.h"

// Dirección I2C del BMI160 (SDO a GND)
#define BMI160_ADDR         (0x68 << 1)

// Registros Principales
#define REG_CHIP_ID         0x00
#define REG_GYRO_DATA       0x0C
#define REG_ACCEL_DATA      0x12
#define REG_COMMAND         0x7E

// Comandos de modo normal
#define CMD_ACCEL_NORMAL    0x11
#define CMD_GYRO_NORMAL     0x15

// Estructura para datos procesados
typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} BMI160_RawData_t;

// Prototipos de funciones
HAL_StatusTypeDef BMI160_Init(I2C_HandleTypeDef *hi2c);
void BMI160_Read_All(I2C_HandleTypeDef *hi2c, BMI160_RawData_t *accel, BMI160_RawData_t *gyro);

#endif
