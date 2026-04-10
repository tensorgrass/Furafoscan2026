#include <bmi160.hpp>

HAL_StatusTypeDef BMI160_Init(I2C_HandleTypeDef *hi2c) {



    uint8_t checkID = 0;
    uint8_t cmd;

    // 1. Verificar ID (debe ser 0xD1)
    HAL_I2C_Mem_Read(hi2c, BMI160_ADDR, REG_CHIP_ID, 1, &checkID, 1, 100);
    if (checkID != 0xD1) return HAL_ERROR;

    // 2. Encender Acelerómetro
    cmd = CMD_ACCEL_NORMAL;
    HAL_I2C_Mem_Write(hi2c, BMI160_ADDR, REG_COMMAND, 1, &cmd, 1, 100);
    HAL_Delay(10); // Tiempo de arranque

    // 3. Encender Giroscopio
    cmd = CMD_GYRO_NORMAL;
    HAL_I2C_Mem_Write(hi2c, BMI160_ADDR, REG_COMMAND, 1, &cmd, 1, 100);
    HAL_Delay(80); // Tiempo de estabilización

    ///////////////////////////////////////////////
// FAST
//  uint8_t data;
//
//      // 1. Reset del dispositivo (Opcional pero recomendado para limpiar estados previos)
//      data = 0xB6;
//      HAL_I2C_Mem_Write(hi2c, BMI160_ADDR, REG_COMMAND, 1, &data, 1, 100);
//      HAL_Delay(10);
//
//      // 2. Encender Acelerómetro y Giroscopio en modo Normal
//      data = 0x11; // Accel Normal Mode
//      HAL_I2C_Mem_Write(hi2c, BMI160_ADDR, REG_COMMAND, 1, &data, 1, 100);
//      HAL_Delay(5);
//
//      data = 0x15; // Gyro Normal Mode
//      HAL_I2C_Mem_Write(hi2c, BMI160_ADDR, REG_COMMAND, 1, &data, 1, 100);
//      HAL_Delay(80); // El Giroscopio tarda más en estabilizarse
//
//      // 3. Configurar High Performance Mode (Desactiva el ahorro de energía que frena el ODR)
//      // Bit 0 = 0 (pwr_save_mode deshabilitado)
//      data = 0x00;
//      HAL_I2C_Mem_Write(hi2c, BMI160_ADDR, REG_CONF, 1, &data, 1, 100);
//
//      // 4. CONFIGURACIÓN DE VELOCIDAD (ODR)
//      // Acelerómetro: 1600 Hz (0x0C) y BWP en modo normal (0x20) -> total 0x2C
//      data = 0x2C;
//      HAL_I2C_Mem_Write(hi2c, BMI160_ADDR, REG_ACC_CONF, 1, &data, 1, 100);
//
//      // Giroscopio: 1600 Hz (0x0C) y BWP en modo normal (0x20) -> total 0x2C
//      data = 0x2C;
//      HAL_I2C_Mem_Write(hi2c, BMI160_ADDR, REG_GYR_CONF, 1, &data, 1, 100);
//
//      // 5. Configurar Rangos (Opcional, pero define la precisión)
//      data = 0x03; // +/- 2G
//      HAL_I2C_Mem_Write(hi2c, BMI160_ADDR, REG_ACC_RANGE, 1, &data, 1, 100);
//
//      data = 0x00; // +/- 2000 º/s
//      HAL_I2C_Mem_Write(hi2c, BMI160_ADDR, REG_GYR_RANGE, 1, &data, 1, 100);

    return HAL_OK;
}

void BMI160_Read_All(I2C_HandleTypeDef *hi2c, BMI160_RawData_t *accel, BMI160_RawData_t *gyro) {
    uint8_t raw_buffer[12]; // 6 bytes gyro + 6 bytes accel

    // Leemos desde el primer registro de datos (Gyro 0x0C) hasta el final de Accel
    HAL_I2C_Mem_Read(hi2c, BMI160_ADDR, REG_GYRO_DATA, 1, raw_buffer, 12, 100);

    // Mapeo de Giroscopio
    gyro->x = (int16_t)((raw_buffer[1] << 8) | raw_buffer[0]);
    gyro->y = (int16_t)((raw_buffer[3] << 8) | raw_buffer[2]);
    gyro->z = (int16_t)((raw_buffer[5] << 8) | raw_buffer[4]);

    // Mapeo de Acelerómetro
    accel->x = (int16_t)((raw_buffer[7] << 8) | raw_buffer[6]);
    accel->y = (int16_t)((raw_buffer[9] << 8) | raw_buffer[8]);
    accel->z = (int16_t)((raw_buffer[11] << 8) | raw_buffer[10]);
}
