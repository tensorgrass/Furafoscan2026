/*
 * maincpp.cpp
 *
 *  Created on: May 24, 2025
 *      Author: froilan
 */

// #include <main.h>
#include <ADCBase.hpp>
#include <ButtonPullup.hpp>
#include <ControllerBase.hpp>
#include <FlashMemory.hpp>
//#include <FuraA.hpp>
//#include <FuraB.hpp>
//#include <FuraC.hpp>
//#include <FuraD.hpp>
#include <FuraE.hpp>
#include <IRReceiver.hpp>
#include <LedBase.hpp>
#include <MotorOneShot125.hpp>
#include <TestA.hpp>
#include <TestBase.hpp>
#include <TestC.hpp>
#include <TestD.hpp>
#include <TestE.hpp>
#include <TestG.hpp>
#include <TestH.hpp>
#include <TestI.hpp>
#include <TestVoid.hpp>
#include <Timer11Delay.hpp>
#include <TrackerBase.hpp>
#include <UartComm.hpp>
#include <UartSerial.hpp>
#include <maincpp.hpp>
#include <message_types.hpp>
#include <TofVL53L4CD2.hpp>
#include <GyroBMI160.hpp>

ControllerBase controller;
TestBase *current_test = nullptr;
FuraE fura_run(&controller);

// Variable para contar los mensajes enviados/recibidos
volatile int message_counter = 0;

extern "C" void main_fura_mode(TIM_HandleTypeDef *htim2,
		                       TIM_HandleTypeDef *htim4,
                               ADC_HandleTypeDef *hadc1,
							   I2C_HandleTypeDef *hi2c1,
							   uint16_t *adc_values_value,
							   uint16_t num_adc_channels_value) {
  ADCBase adc(hadc1, adc_values_value, num_adc_channels_value);
  bool tracker_reverse = false;
  TrackerBase tracker_left(&adc, 0, tracker_reverse);  // PA1
  bool led_reverse = false;
  //LedBase led_tracker_left(GPIOA, GPIO_PIN_15, led_reverse);
  TrackerBase tracker_right(&adc, 1, tracker_reverse);  // PA2
  //LedBase led_tracker_right(GPIOB, GPIO_PIN_3, led_reverse);
  TofVL53L4CD2 distance_tof_left(GPIOB, GPIO_PIN_4);  // PB4
  //LedBase led_distance_left(GPIOA, GPIO_PIN_12, led_reverse);
  TofVL53L4CD2 distance_tof_right(GPIOA, GPIO_PIN_15);  // PA15
  //LedBase led_distance_right(GPIOB, GPIO_PIN_4, led_reverse);
  //TrackerBase distance_lateral_left(&adc, 4);  // PA4
  //LedBase led_distance_lateral_left(GPIOA, GPIO_PIN_11, led_reverse);
  //TrackerBase distance_lateral_right(&adc, 5);  // PA6
  //LedBase led_distance_lateral_right(GPIOB, GPIO_PIN_5, led_reverse);
  TofVL53L4CD2 distance_tof_center(GPIOB, GPIO_PIN_3);  // PA6
  //LedBase led_distance_center(GPIOA, GPIO_PIN_8, led_reverse);

  distance_tof_left.init(TOF_ADDR_LEFT);
  distance_tof_right.init(TOF_ADDR_RIGHT);
  distance_tof_center.init(TOF_ADDR_CENTER);

  MotorOneShot125 motor_left(htim4, TIM_CHANNEL_2, &(htim4->Instance->CCR2));
  MotorOneShot125 motor_right(htim4, TIM_CHANNEL_1, &(htim4->Instance->CCR1));

  bool reverse = true;
  ButtonPullup button_start(GPIOB, GPIO_PIN_13, reverse);
  IRReceiver ir_receiver(htim2, TIM_CHANNEL_3);
  led_reverse = true;
  LedBase led_start(GPIOC, GPIO_PIN_13, led_reverse);
  GyroBMI160 sensor_gyro(hi2c1);  // Desactivado temporalmente

  FlashMemory flash_memory;

  controller.init_furafoscan(&adc,
							 &tracker_left,
							 &tracker_right,
							 &distance_tof_left,
							 &distance_tof_right,
							 &distance_tof_center,
							 &motor_left, &motor_right,
							 &button_start, &ir_receiver, &led_start,
							 &sensor_gyro,
							 &flash_memory);
  while (1) {
    fura_run.main();
  }
}

void HAL_UART_RxCpltCallback_cpp(UART_HandleTypeDef *huart) {
  if (huart->Instance == USART1)  // Verifica que sea el UART correcto
  {
    // Llama al método de la clase UartSerial para manejar la recepción
    controller.commReset();  // Reinicia la recepción para el siguiente mensaje
    if (controller.getRxGroup() != '\0') {
      controller.endRxTransaction();
      delete current_test;
      current_test = new TestVoid(&controller);
    }
    controller.serialReceivedData(huart);
  } else if (huart->Instance == USART2) {
    // Hemos recibido un mensaje completo
    controller.commReceivedData(huart);
    if (controller.getRxGroup() != current_test->getGroup()) {
      switch (controller.getRxGroup()) {
        case '\0':
          current_test = new TestVoid(&controller);
          break;
        case 'A':
          current_test = new TestA(&controller);
          break;
        case 'C':
          current_test = new TestC(&controller);
          break;
        case 'D':
          current_test = new TestD(&controller);
          break;
        case 'E':
          current_test = new TestE(&controller);
          break;
        case 'G':
          current_test = new TestG(&controller);
          break;
        case 'H':
          current_test = new TestH(&controller);
          break;
        case 'I':
          current_test = new TestI(&controller);
          break;
        default:
          break;
      }
    }
  }
}

void HAL_GPIO_EXTI_Callback_cpp(uint16_t GPIO_Pin) {
  if (GPIO_Pin == GPIO_PIN_13) {
    controller.getButtonStart()->actualizaEstado();
  }
}

void HAL_TIM_IC_CaptureCallback_cpp(TIM_HandleTypeDef *htim) {
  if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3) {
    controller.getIRReceiver()->adquireData();
  }
}
