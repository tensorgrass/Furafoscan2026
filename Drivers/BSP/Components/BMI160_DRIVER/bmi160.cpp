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
