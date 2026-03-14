#include <stm32f4xx_hal.h>
#include <SoloBGyro.hpp>
#include <GyroBMI160.hpp>

int16_t accel_x, accel_y, accel_z;
int16_t gyro_x, gyro_y, gyro_z;

extern "C" void test_solobgyro(I2C_HandleTypeDef *hi2c1) {
	GyroBMI160 gyro(hi2c1);
	while (1) {
		gyro.read_values();
		accel_x = gyro.get_accel_x();
		accel_y = gyro.get_accel_y();
		accel_z = gyro.get_accel_z();
		gyro_x = gyro.get_gyro_x();
		gyro_y = gyro.get_gyro_y();
		gyro_z = gyro.get_gyro_z();
		HAL_Delay(1);
	}
}
