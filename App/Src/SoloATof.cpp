#include <SoloATof.hpp>

#define MIN_DITANCE 0
#define MAX_DITANCE 500

#define DEV_CENTER 0x57
#define DEV_LEFT 0x24
#define DEV_RIGHT 0x26

#include <TofVL53L4CD2.hpp>



uint16_t distance_center;
uint16_t distance_left;
uint16_t distance_right;

// prueba boton 3 segundos para arrancar
extern "C" void test_soloatof(void) {

	TofVL53L4CD2 distance_tof_center = TofVL53L4CD2(GPIOB, GPIO_PIN_3);
	TofVL53L4CD2 distance_tof_left = TofVL53L4CD2(GPIOB, GPIO_PIN_4);
	TofVL53L4CD2 distance_tof_right = TofVL53L4CD2(GPIOA, GPIO_PIN_15);
	distance_tof_center.init(DEV_CENTER);
	distance_tof_left.init(DEV_LEFT);
	distance_tof_right.init(DEV_RIGHT);

	while (1) {
		if (distance_tof_center.isReady()) {
			distance_center = distance_tof_center.getDistance(MIN_DITANCE, MAX_DITANCE);
		}
		if (distance_tof_left.isReady()) {
			distance_left = distance_tof_left.getDistance(MIN_DITANCE, MAX_DITANCE);
		}
		if (distance_tof_right.isReady()) {
			distance_right = distance_tof_right.getDistance(MIN_DITANCE, MAX_DITANCE);
		}
		HAL_Delay(1);
	}

}
