#include "FuraE.hpp"

FuraE::FuraE(ControllerBase* controllerBaseValue) : controller(controllerBaseValue) {
  // Constructor de FuraE, inicializa el controlador y los objetos UART
  current_step = enum_step_fura::STEP_WAIT_BUTTON_START_PRESSED;
  current_step_init = true;
  direction_base = enum_direction::DIR_NONE;

  uint32_t rx_data[FLASH_DATA_NUM_VALUES];
  controller->getFlashMemory()->read(rx_data, FLASH_DATA_NUM_VALUES);
  if (rx_data[0] == FLASH_DATA_VERSION) {
    tracker_base_left_in_detect_line = rx_data[1];
    tracker_base_right_in_detect_line = rx_data[2];
    tracker_base_left_out_detect_line = rx_data[3];
    tracker_base_right_out_detect_line = rx_data[4];
    rc_address = rx_data[5];
    fura_type = rx_data[6];
  }

//  fura_type = enum_type_fura::T_FURAFOSCAN1;
//  fura_type = enum_type_fura::T_FURAFOSCAN2;

  switch ((enum_type_fura)fura_type) {
    case enum_type_fura::T_FURAFOSCAN1:
      config_motor = &Configs::Furafoscan1;
      break;
    case enum_type_fura::T_FURAFOSCAN2:
      config_motor = &Configs::Furafoscan2;
      break;
    case enum_type_fura::T_FURAXICLE:
      break;
  }

}

// prueba boton 5 segundos para arrancar y arrancar el motor
void FuraE::main() {
  switch (current_step) {
    case enum_step_fura::STEP_WAIT_BUTTON_START_PRESSED:
      if (current_step_init) {
        controller->getIRReceiver()->setNecType();
        current_step_init = false;
      }
      if (controller->getButtonStart()->isButtonPressed()) {
        HAL_Delay(1);
        // volvemos a leer el valor del motor para descartar rebotes
        if (controller->getButtonStart()->isButtonPressed()) {
          if (button_stop__button_previous_state == 0) {
            // Se ha pulsado el boton, apagamos el led y pasamos al siguiente
            // estado y empezamos a calcular el timempo del boton pulsado
            controller->getLedStart()->turn_on();
            button_stop__button_previous_state = 1;
            setDirectionCenter();
            step_after_button = enum_step_fura::STEP_MOTOR_START;
            goNextStep(enum_step_fura::STEP_BUTTON_START_PRESSED);
          } else {
            // Aun esta pulsado el boton del estado anterior, se tiene que
            // esperar a que lo suelte
          }
        } else {
          // rebote detectado
        }
      } else if (controller->getIRReceiver()->isDataReady()) {
        uint32_t ir_receiver_value = controller->getIRReceiver()->getValueNec();
        switch (ir_receiver_value) {
          case F_E_IR_NEC_OK:
            controller->getLedStart()->turn_on();
            button_stop__button_previous_state = 0;
            setDirectionCenter();
            step_after_button = enum_step_fura::STEP_MOTOR_START;
//            goNextStep(enum_step_fura::STEP_BUTTON_START_PRESSED);
            goNextStep(enum_step_fura::STEP_WAIT_REMOTE_SUMO);
            break;
          case F_E_IR_NEC_UP:
            controller->getLedStart()->turn_on();
            button_stop__button_previous_state = 0;
            setDirectionCenter();
            step_after_button = enum_step_fura::STEP_MOTOR_START;
            goNextStep(enum_step_fura::STEP_WAIT_REMOTE_SUMO);
            break;

          case F_E_IR_NEC_LEFT:
            controller->getLedStart()->turn_on();
            button_stop__button_previous_state = 0;
            setDirectionLeft();
            step_after_button = enum_step_fura::STEP_MOTOR_START;
//            step_after_button = enum_step_fura::STEP_DEFENSE_LEFT;
            goNextStep(enum_step_fura::STEP_WAIT_REMOTE_SUMO);
            break;

          case F_E_IR_NEC_RIGHT:
            controller->getLedStart()->turn_on();
            button_stop__button_previous_state = 0;
            setDirectionRight();
            step_after_button = enum_step_fura::STEP_MOTOR_START;
//            step_after_button = enum_step_fura::STEP_DEFENSE_RIGHT;
            goNextStep(enum_step_fura::STEP_WAIT_REMOTE_SUMO);
            break;

          case F_E_IR_NEC_DOWN:
            controller->getLedStart()->turn_on();
            button_stop__button_previous_state = 0;
            step_after_button = enum_step_fura::STEP_DEFENSE_SPIN;
            goNextStep(enum_step_fura::STEP_WAIT_REMOTE_SUMO);
            break;

          case F_E_IR_NEC_MOUSE:  // configurar los limites de los tracker en la pista real
            button_stop__button_previous_state = 0;
            controller->getLedStart()->turn_on();
            tracker_base_lap = 0;
            tracker_base_iteration = 0;
            tracker_base_left_values.clear();
            tracker_base_right_values.clear();
            goNextStep(enum_step_fura::STEP_INIT_TRACKER_CALIBRATE_WHITE);
            break;

          case F_E_IR_NEC_MENU:  // Testear los sensores de traking y distancia, sin que funcionen los motores
            button_stop__button_previous_state = 0;
            controller->getLedStart()->turn_on();
            goNextStep(enum_step_fura::STEP_TEST_ALL_SENSORS);
            break;

          default:
            break;
        }

      } else {
        // esperar a que se pulse el boton, se deja parpadeando el led
        if (button_stop__button_previous_state == 1) {
          button_stop__button_previous_state = 0;
        }
        // Parpadea led, segun si es par o impar cada 100ms
        if ((HAL_GetTick() / 500) % 2 == 0) {
          controller->getLedStart()->turn_on();
        } else {
          controller->getLedStart()->turn_off();
        }
      }
      break;

    case enum_step_fura::STEP_WAIT_TRACKER_CALIBRATE_WHITE:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;
      }

      if ((HAL_GetTick() / 100) % 2 == 0) {
        controller->getLedStart()->turn_on();
      } else {
        controller->getLedStart()->turn_off();
      }
      if (controller->getIRReceiver()->isDataReady()) {
        uint32_t ir_receiver_value = controller->getIRReceiver()->getValueNec();
        if (ir_receiver_value == F_E_IR_NEC_MOUSE) {
          controller->getLedStart()->turn_on();
          goNextStep(enum_step_fura::STEP_INIT_TRACKER_CALIBRATE_WHITE);
        }
      }
      break;

    case enum_step_fura::STEP_INIT_TRACKER_CALIBRATE_WHITE:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;

        tracker_base_time_tick_ini = HAL_GetTick();
      }

      tracker_base_time_tick_current = HAL_GetTick();
      tracker_base_time_tick_current = HAL_GetTick() - tracker_base_time_tick_ini;
      if (tracker_base_time_tick_current < 3000) {
        tracker_base_left_values.push_back(controller->getTrackerLeft()->getValue());
        tracker_base_right_values.push_back(controller->getTrackerRight()->getValue());
        tracker_base_iteration++;
        HAL_Delay(1);
      } else if (tracker_base_lap < 2) {
        tracker_base_lap++;
        goNextStep(enum_step_fura::STEP_WAIT_TRACKER_CALIBRATE_WHITE);
      } else {
        uint32_t sum_values_left = 0;
        uint32_t sum_values_right = 0;
        for (uint32_t i = 250; i < tracker_base_iteration; i++) {
          sum_values_left += tracker_base_left_values[i];
          sum_values_right += tracker_base_right_values[i];
        }
        tracker_base_left_in_detect_line = sum_values_left / tracker_base_iteration;
        tracker_base_right_in_detect_line = sum_values_right / tracker_base_iteration;
        tracker_base_lap = 0;
        tracker_base_iteration = 0;
        tracker_base_left_values.clear();
        tracker_base_right_values.clear();
        goNextStep(enum_step_fura::STEP_WAIT_TRACKER_CALIBRATE_BLACK);
      }
      break;

    case enum_step_fura::STEP_WAIT_TRACKER_CALIBRATE_BLACK:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;
      }

      if ((HAL_GetTick() / 100) % 2 == 0) {
        controller->getLedStart()->turn_on();
      } else {
        controller->getLedStart()->turn_off();
      }
      if (controller->getIRReceiver()->isDataReady()) {
        uint32_t ir_receiver_value = controller->getIRReceiver()->getValueNec();
        if (ir_receiver_value == F_E_IR_NEC_MOUSE) {
          controller->getLedStart()->turn_on();
          goNextStep(enum_step_fura::STEP_INIT_TRACKER_CALIBRATE_BLACK);
        }
      }
      break;

    case enum_step_fura::STEP_INIT_TRACKER_CALIBRATE_BLACK:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;

        tracker_base_time_tick_ini = HAL_GetTick();
      }

      tracker_base_time_tick_current = HAL_GetTick();
      tracker_base_time_tick_current = HAL_GetTick() - tracker_base_time_tick_ini;
      if (tracker_base_time_tick_current < 3000) {
        tracker_base_left_values.push_back(controller->getTrackerLeft()->getValue());
        tracker_base_right_values.push_back(controller->getTrackerRight()->getValue());
        tracker_base_iteration++;
        HAL_Delay(1);
      } else if (tracker_base_lap < 2) {
        tracker_base_lap++;
        goNextStep(enum_step_fura::STEP_WAIT_TRACKER_CALIBRATE_BLACK);
      } else {
        uint32_t sum_values_left = 0;
        uint32_t sum_values_right = 0;
        for (uint32_t i = 250; i < tracker_base_iteration; i++) {
          sum_values_left += tracker_base_left_values[i];
          sum_values_right += tracker_base_right_values[i];
        }
        uint32_t mid_values_left = sum_values_left / tracker_base_iteration;
        uint32_t mid_values_right = sum_values_right / tracker_base_iteration;
        if (mid_values_left < (tracker_base_left_in_detect_line + 500) || mid_values_right < (tracker_base_right_in_detect_line + 500)) {
          tracker_base_left_in_detect_line = F_E_TRACKER_LEFT_IN_DETECT_LINE;
          tracker_base_right_in_detect_line = F_E_TRACKER_RIGHT_IN_DETECT_LINE;
          tracker_base_left_out_detect_line = F_E_TRACKER_LEFT_OUT_DETECT_LINE;
          tracker_base_right_out_detect_line = F_E_TRACKER_RIGHT_OUT_DETECT_LINE;
        } else {
          tracker_base_left_in_detect_line = (((mid_values_left - tracker_base_left_in_detect_line) / 4) * 3) + tracker_base_left_in_detect_line;
          tracker_base_right_in_detect_line = (((mid_values_right - tracker_base_right_in_detect_line) / 4) * 3) + tracker_base_right_in_detect_line;
          tracker_base_left_out_detect_line = tracker_base_left_in_detect_line + 300;
          tracker_base_right_out_detect_line = tracker_base_right_in_detect_line + 300;
        }

        writeDataFlash(tracker_base_left_in_detect_line,
                       tracker_base_right_in_detect_line,
                       tracker_base_left_out_detect_line,
                       tracker_base_right_out_detect_line,
                       rc_address,
                       fura_type);

        tracker_base_left_values.clear();
        tracker_base_right_values.clear();
        goNextStep(enum_step_fura::STEP_WAIT_BUTTON_START_PRESSED);
      }
      break;

    case enum_step_fura::STEP_TEST_ALL_SENSORS:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;

        tracker_left_in__time_tick_ini = 0;
        tracker_right_in__time_tick_ini = 0;
        distance_left_in__time_tick_ini = 0;
        distance_right_in__time_tick_ini = 0;
        distance_center_in__time_tick_ini = 0;
      }

      if ((HAL_GetTick() / 100) % 2 == 0) {
        controller->getLedStart()->turn_on();
      } else {
        controller->getLedStart()->turn_off();
      }

      previous_step = current_step;
      detectButtonStop();
      if (current_step != previous_step) {
        controller->getLedTrackerLeft()->turn_off();
        controller->getLedTrackerRight()->turn_off();
        controller->getLedDistanceLeft()->turn_off();
        controller->getLedDistanceRight()->turn_off();
        controller->getLedDistanceCenter()->turn_off();
        break;
      }

      detectTrakerLeftIn(enum_step_fura::STEP_TEST_ALL_SENSORS);
      detectTrakerRightIn(enum_step_fura::STEP_TEST_ALL_SENSORS);
      detectSensorGyroUp(enum_step_fura::STEP_TEST_ALL_SENSORS);
      detectDistanceCenterIn(enum_step_fura::STEP_TEST_ALL_SENSORS);
      detectDistanceLeftIn(enum_step_fura::STEP_TEST_ALL_SENSORS);
      detectDistanceRightIn(enum_step_fura::STEP_TEST_ALL_SENSORS);
      break;
    case enum_step_fura:: STEP_WAIT_REMOTE_SUMO:
      if (current_step_init) {
        controller->getIRReceiver()->setRC5SumoType();
        previous_step = current_step;
        current_step_init = false;

        setMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_ESC_MID_STOP].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_MID_STOP].value);
      }
      if (controller->getIRReceiver()->isDataReady()) {
        IRReceiver::ValueRC5Sumo value = controller->getIRReceiver()->getValueRC5Sumo();
        uint32_t command = value.command;
        uint32_t address = value.address;
        switch (command) {
          case F_E_IR_RC5SUMO_COMMAND_START_STOP: //start / stop
            if (address == rc_address+1) { //start
              controller->getLedStart()->turn_on();
              button_stop__button_previous_state = 0;
              goNextStep(step_after_button);
              step_after_button = enum_step_fura::STEP_WAIT_BUTTON_START_PRESSED;

            }
            else if (address == rc_address) { //stop
              rc_num_error2=rc_num_error2; //No hace nada
            }
            else {
              rc_num_error2++;
            }
            break;
          case F_E_IR_RC5SUMO_COMMAND_PROGRAM: // program
            rc_address = address;
            writeDataFlash(tracker_base_left_in_detect_line,
                           tracker_base_right_in_detect_line,
                           tracker_base_left_out_detect_line,
                           tracker_base_right_out_detect_line,
                           rc_address,
                           fura_type);
            break;
          default:
            rc_num_error1++;
        }
      }
      else {
        if ((HAL_GetTick() / 100) % 2 == 0) {
          controller->getLedStart()->turn_on();
        } else {
          controller->getLedStart()->turn_off();
        }
      }

      break;
    case enum_step_fura::STEP_BUTTON_START_PRESSED:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;

        setMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_ESC_MID_STOP].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_MID_STOP].value);
      }

      HAL_Delay(F_E_BUTTON_START_WAIT);
      controller->getLedStart()->turn_off();
      button_stop__button_previous_state = 0;
      goNextStep(step_after_button);
      step_after_button = enum_step_fura::STEP_WAIT_BUTTON_START_PRESSED;
      break;

    case enum_step_fura::STEP_DEFENSE_LEFT:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;

        setMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_DEFENSE_BACK_SLOW].value, (*config_motor)[(size_t)enum_motor::F_M_DEFENSE_BACK_FAST].value);
        sensor_gyro_in__time_tick_ini = 0;
        tracker_left_in__time_tick_ini = 0;
        tracker_right_in__time_tick_ini = 0;
        distance_left_in__time_tick_ini = 0;
        distance_right_in__time_tick_ini = 0;
        distance_center_in__time_tick_ini = 0;

        time_out__time_tick_ini = HAL_GetTick();

      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorGyroUp(enum_step_fura::STEP_SENSOR_TILTING_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerLeftIn(enum_step_fura::STEP_TRACKER_LEFT_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerRightIn(enum_step_fura::STEP_TRACKER_RIGHT_DETECTED);
      if (current_step != previous_step) break;

      detectTimeOut(enum_step_fura::STEP_DEFENSE__STEP2_ATTACK, (*config_motor)[(size_t)enum_motor::F_M_DEFENSE_LEFT_OUT_MS].value);
      if (current_step != previous_step) break;
      break;

    case enum_step_fura::STEP_DEFENSE_RIGHT:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;

        setMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_DEFENSE_BACK_FAST].value, (*config_motor)[(size_t)enum_motor::F_M_DEFENSE_BACK_SLOW].value);
        sensor_gyro_in__time_tick_ini = 0;
        tracker_left_in__time_tick_ini = 0;
        tracker_right_in__time_tick_ini = 0;
        distance_left_in__time_tick_ini = 0;
        distance_right_in__time_tick_ini = 0;
        distance_center_in__time_tick_ini = 0;

        time_out__time_tick_ini = HAL_GetTick();
      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorGyroUp(enum_step_fura::STEP_SENSOR_TILTING_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerLeftIn(enum_step_fura::STEP_TRACKER_LEFT_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerRightIn(enum_step_fura::STEP_TRACKER_RIGHT_DETECTED);
      if (current_step != previous_step) break;

      detectTimeOut(enum_step_fura::STEP_DEFENSE__STEP2_ATTACK, (*config_motor)[(size_t)enum_motor::F_M_DEFENSE_RIGHT_OUT_MS].value);
      if (current_step != previous_step) break;
      break;

    case enum_step_fura::STEP_DEFENSE_SPIN:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;

        setMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_DEFENSE_SPIN_FRONT].value, (*config_motor)[(size_t)enum_motor::F_M_DEFENSE_SPIN_BACK].value);
        tracker_left_in__time_tick_ini = 0;
        tracker_right_in__time_tick_ini = 0;
        distance_left_in__time_tick_ini = 0;
        distance_right_in__time_tick_ini = 0;
        distance_center_in__time_tick_ini = 0;

        time_out__time_tick_ini = HAL_GetTick();
      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorGyroUp(enum_step_fura::STEP_SENSOR_TILTING_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerLeftIn(enum_step_fura::STEP_TRACKER_LEFT_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerRightIn(enum_step_fura::STEP_TRACKER_RIGHT_DETECTED);
      if (current_step != previous_step) break;

      detectTimeOut(enum_step_fura::STEP_DEFENSE__STEP2_ATTACK, (*config_motor)[(size_t)enum_motor::F_M_DEFENSE_SPIN_OUT_MS].value);
      if (current_step != previous_step) break;
      break;

    case enum_step_fura::STEP_DEFENSE__STEP2_ATTACK:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;

        setMotorSpeedRamp((*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI].value, (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_BOTH_SPEED_BOOST].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI].value, (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_BOTH_SPEED_BOOST].value);
        distance_center_out__time_tick_ini = HAL_GetTick();
      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_FROM_BACK_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT_FROM_BACK].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorGyroUp(enum_step_fura::STEP_SENSOR_TILTING_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerLeftIn(enum_step_fura::STEP_TRACKER_LEFT_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerRightIn(enum_step_fura::STEP_TRACKER_RIGHT_DETECTED);
      if (current_step != previous_step) break;

      detectDistanceCenterOut(enum_step_fura::STEP_DISTANCE_BOTH_DETECTED, (*config_motor)[(size_t)enum_motor::F_M_DEFENSE_ATTACK_OUT_MS].value);
      if (current_step != previous_step) break;
      break;

    case enum_step_fura::STEP_MOTOR_START:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;
        setMotorSpeedRamp((*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI].value, esc_speed_base_left, (*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI].value, esc_speed_base_right);
        sensor_gyro_in__time_tick_ini = 0;
        tracker_left_in__time_tick_ini = 0;
        tracker_right_in__time_tick_ini = 0;
        distance_left_in__time_tick_ini = 0;
        distance_right_in__time_tick_ini = 0;
        distance_center_in__time_tick_ini = 0;
        time_out__time_tick_ini = 0;
      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorGyroUp(enum_step_fura::STEP_SENSOR_TILTING_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerLeftIn(enum_step_fura::STEP_TRACKER_LEFT_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerRightIn(enum_step_fura::STEP_TRACKER_RIGHT_DETECTED);
      if (current_step != previous_step) break;

      detectDistanceCenterIn(enum_step_fura::STEP_DISTANCE_CENTER_BOOST);
      if (current_step != previous_step) break;
      detectDistanceLeftIn(enum_step_fura::STEP_DISTANCE_LEFT_DETECTED);
      if (current_step != previous_step) break;
      detectDistanceRightIn(enum_step_fura::STEP_DISTANCE_RIGHT_DETECTED);
      if (current_step != previous_step) break;
      break;

    case enum_step_fura::STEP_SENSOR_TILTING_DETECTED:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;
        setMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SENSOR_TILTING_BACK_FAST].value, (*config_motor)[(size_t)enum_motor::F_M_SENSOR_TILTING_BACK_SLOW].value);
        sensor_gyro_out__time_tick_ini = HAL_GetTick();
      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorTiltingOut(enum_step_fura::STEP_MOTOR_START, (*config_motor)[(size_t)enum_motor::F_M_SENSOR_TILTING_OUT_MS].value);
      if (current_step != previous_step) break;
      break;

    case enum_step_fura::STEP_TRACKER_LEFT_DETECTED:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;
        setMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_ESC_TRAKER_BACK].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_TRAKER_BACK].value);
        tracker_left_out__time_tick_ini = HAL_GetTick();
      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorGyroUp(enum_step_fura::STEP_SENSOR_TILTING_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerLeftOut(enum_step_fura::STEP_TRACKER_LEFT_DETECTED__STEP2_ROTATE, (*config_motor)[(size_t)enum_motor::F_M_TRACKER_OUT_OF_LINE_MS].value);
      if (current_step != previous_step) break;
      detectTrakerRightIn(enum_step_fura::STEP_TRACKER_LEFT_DETECTED__STEP2_RIGHT_DETECTED);
      if (current_step != previous_step) break;
      break;

    case enum_step_fura::STEP_TRACKER_LEFT_DETECTED__STEP2_ROTATE:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;
        setMotorSpeedRamp((*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_TRAKER_ROTATE_FAST].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_TRAKER_ROTATE_SLOW].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_TRAKER_ROTATE_SLOW].value);
        time_out__time_tick_ini = HAL_GetTick();
      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorGyroUp(enum_step_fura::STEP_SENSOR_TILTING_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerLeftIn(enum_step_fura::STEP_TRACKER_LEFT_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerRightIn(enum_step_fura::STEP_TRACKER_RIGHT_DETECTED);
      if (current_step != previous_step) break;
      detectTimeOut(STEP_MOTOR_START, (*config_motor)[(size_t)enum_motor::F_M_TRACKER_ROTATE_MS].value);
      if (current_step != previous_step) break;
      break;

    case enum_step_fura::STEP_TRACKER_LEFT_DETECTED__STEP2_RIGHT_DETECTED:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;
        setMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_ESC_TRACKER_BOTH_SPEED_FAST].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_TRACKER_BOTH_SPEED_SLOW].value);
        tracker_left_out__time_tick_ini = HAL_GetTick();
        tracker_right_out__time_tick_ini = HAL_GetTick();
      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorGyroUp(enum_step_fura::STEP_SENSOR_TILTING_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerRightOut(enum_step_fura::STEP_TRACKER_RIGHT_DETECTED__STEP2_ROTATE, (*config_motor)[(size_t)enum_motor::F_M_TRACKER_OUT_OF_LINE_BOTH_MS].value);
      if (current_step != previous_step) break;
      detectTrakerLeftOut(enum_step_fura::STEP_TRACKER_LEFT_DETECTED__STEP2_ROTATE, (*config_motor)[(size_t)enum_motor::F_M_TRACKER_OUT_OF_LINE_BOTH_MS].value);
      if (current_step != previous_step) break;
      break;

    case enum_step_fura::STEP_TRACKER_RIGHT_DETECTED:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;
        setMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_ESC_TRAKER_BACK].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_TRAKER_BACK].value);
        tracker_right_out__time_tick_ini = HAL_GetTick();
      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorGyroUp(enum_step_fura::STEP_SENSOR_TILTING_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerRightOut(enum_step_fura::STEP_TRACKER_RIGHT_DETECTED__STEP2_ROTATE, (*config_motor)[(size_t)enum_motor::F_M_TRACKER_OUT_OF_LINE_MS].value);
      if (current_step != previous_step) break;
      detectTrakerLeftIn(enum_step_fura::STEP_TRACKER_RIGHT_DETECTED__STEP2_LEFT_DETECTED);
      if (current_step != previous_step) break;
      break;

    case enum_step_fura::STEP_TRACKER_RIGHT_DETECTED__STEP2_ROTATE:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;
        setMotorSpeedRamp((*config_motor)[(size_t)enum_motor::F_M_ESC_TRAKER_ROTATE_SLOW].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_TRAKER_ROTATE_SLOW].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_TRAKER_ROTATE_FAST].value);
        tracker_right_out__time_tick_ini = HAL_GetTick();
        time_out__time_tick_ini = HAL_GetTick();
      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorGyroUp(enum_step_fura::STEP_SENSOR_TILTING_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerRightIn(enum_step_fura::STEP_TRACKER_RIGHT_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerLeftIn(enum_step_fura::STEP_TRACKER_LEFT_DETECTED);
      if (current_step != previous_step) break;
      detectTimeOut(STEP_MOTOR_START, (*config_motor)[(size_t)enum_motor::F_M_TRACKER_ROTATE_MS].value);
      if (current_step != previous_step) break;
      break;

    case enum_step_fura::STEP_TRACKER_RIGHT_DETECTED__STEP2_LEFT_DETECTED:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;
        setMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_ESC_TRACKER_BOTH_SPEED_SLOW].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_TRACKER_BOTH_SPEED_FAST].value);
        tracker_left_out__time_tick_ini = HAL_GetTick();
        tracker_right_out__time_tick_ini = HAL_GetTick();
      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorGyroUp(enum_step_fura::STEP_SENSOR_TILTING_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerLeftOut(enum_step_fura::STEP_TRACKER_LEFT_DETECTED__STEP2_ROTATE, (*config_motor)[(size_t)enum_motor::F_M_TRACKER_OUT_OF_LINE_BOTH_MS].value);
      if (current_step != previous_step) break;
      detectTrakerRightOut(enum_step_fura::STEP_TRACKER_RIGHT_DETECTED__STEP2_ROTATE, (*config_motor)[(size_t)enum_motor::F_M_TRACKER_OUT_OF_LINE_BOTH_MS].value);
      if (current_step != previous_step) break;
      break;

    case enum_step_fura::STEP_DISTANCE_LEFT_DETECTED:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;
        setMotorSpeedRamp((*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI].value, (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_SPEED_SLOW_LEFT].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI].value, (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_SPEED_RIGHT].value);
        distance_left_out__time_tick_ini = HAL_GetTick();
      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorGyroUp(enum_step_fura::STEP_SENSOR_TILTING_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerLeftIn(enum_step_fura::STEP_TRACKER_LEFT_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerRightIn(enum_step_fura::STEP_TRACKER_RIGHT_DETECTED);
      if (current_step != previous_step) break;

      detectDistanceCenterIn(enum_step_fura::STEP_DISTANCE_CENTER_BOOST);
      if (current_step != previous_step) break;
      detectDistanceLeftOut(enum_step_fura::STEP_MOTOR_START, (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_OUT_MS].value);
      if (current_step != previous_step) {
        detectDistanceRightIn(enum_step_fura::STEP_DISTANCE_RIGHT_DETECTED);
        break;
      }
      detectDistanceRightIn(enum_step_fura::STEP_DISTANCE_BOTH_DETECTED);
      if (current_step != previous_step) break;
      break;

    case enum_step_fura::STEP_DISTANCE_RIGHT_DETECTED:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;
        setMotorSpeedRamp((*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI].value, (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_SPEED_LEFT].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI].value, (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_SPEED_SLOW_RIGHT].value);
        distance_right_out__time_tick_ini = HAL_GetTick();
      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorGyroUp(enum_step_fura::STEP_SENSOR_TILTING_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerLeftIn(enum_step_fura::STEP_TRACKER_LEFT_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerRightIn(enum_step_fura::STEP_TRACKER_RIGHT_DETECTED);
      if (current_step != previous_step) break;

      detectDistanceCenterIn(enum_step_fura::STEP_DISTANCE_CENTER_BOOST);
      if (current_step != previous_step) break;
      detectDistanceRightOut(enum_step_fura::STEP_MOTOR_START, (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_OUT_MS].value);
      if (current_step != previous_step) {
        detectDistanceLeftIn(enum_step_fura::STEP_DISTANCE_LEFT_DETECTED);
        break;
      }
      detectDistanceLeftIn(enum_step_fura::STEP_DISTANCE_BOTH_DETECTED);
      if (current_step != previous_step) break;

      break;

    case enum_step_fura::STEP_DISTANCE_BOTH_DETECTED:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;
        setMotorSpeedRamp((*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI].value, (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_BOTH_SPEED].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI].value, (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_BOTH_SPEED].value);
        distance_left_out__time_tick_ini = HAL_GetTick();
        distance_right_out__time_tick_ini = HAL_GetTick();
      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorGyroUp(enum_step_fura::STEP_SENSOR_TILTING_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerLeftIn(enum_step_fura::STEP_TRACKER_LEFT_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerRightIn(enum_step_fura::STEP_TRACKER_RIGHT_DETECTED);
      if (current_step != previous_step) break;

      detectDistanceCenterIn(enum_step_fura::STEP_DISTANCE_CENTER_BOOST);
      if (current_step != previous_step) break;
      detectDistanceLeftOut(enum_step_fura::STEP_DISTANCE_RIGHT_DETECTED, (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_BOTH_OUT_MS].value);
      if (current_step != previous_step) break;
      detectDistanceRightOut(enum_step_fura::STEP_DISTANCE_LEFT_DETECTED, (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_BOTH_OUT_MS].value);
      if (current_step != previous_step) break;
      break;

    case enum_step_fura::STEP_DISTANCE_CENTER_BOOST:
      if (current_step_init) {
        previous_step = current_step;
        current_step_init = false;
        setMotorSpeedRamp((*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI].value, (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_BOTH_SPEED_BOOST].value, (*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI].value, (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_BOTH_SPEED_BOOST].value);
        distance_center_out__time_tick_ini = HAL_GetTick();
      }

      rampMotorSpeed((*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS].value, (*config_motor)[(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT].value);
      detectButtonStop();
      if (current_step != previous_step) break;
      detectSensorGyroUp(enum_step_fura::STEP_SENSOR_TILTING_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerLeftIn(enum_step_fura::STEP_TRACKER_LEFT_DETECTED);
      if (current_step != previous_step) break;
      detectTrakerRightIn(enum_step_fura::STEP_TRACKER_RIGHT_DETECTED);
      if (current_step != previous_step) break;

      detectDistanceCenterOut(enum_step_fura::STEP_DISTANCE_BOTH_DETECTED, (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_BOTH_BOOST_OUT_MS].value);
      if (current_step != previous_step) break;
      break;

    default:
      // Código para caso por defecto
      break;
  }
}

void FuraE::goNextStep(enum_step_fura next_step) {
  current_step = next_step;
  current_step_init = true;
}

void FuraE::detectButtonStop() {
  if (controller->getButtonStart()->isButtonPressed()) {
    HAL_Delay(1);
    // volvemos a leer el valor del motor para descartar rebotes
    if (controller->getButtonStart()->isButtonPressed()) {
      // en el caso que se haya soltado, y se vuelva a pulsar, se tiene que
      // parar todo y volver al estado inicial
      if (button_stop__button_previous_state == 0) {
        // Se ha pulsado el boton y se tiene que parar todo
        switch ((enum_type_fura)fura_type) {
          case enum_type_fura::T_FURAFOSCAN1:
            controller->getMotorOneshot125Left()->disable();
            controller->getMotorOneshot125Right()->disable();
            break;
          case enum_type_fura::T_FURAFOSCAN2:
            controller->getMotorPWMLeft()->disable();
            controller->getMotorPWMRight()->disable();
            break;
          case enum_type_fura::T_FURAXICLE:
            controller->getMotorOneshot125Left()->disable();
            controller->getMotorOneshot125Right()->disable();
            break;
        }
        button_stop__button_previous_state = 1;
        goNextStep(enum_step_fura::STEP_WAIT_BUTTON_START_PRESSED);
        return;
      } else {
        // aun está pulsado el boton del estado anterior, se tiene que esperar a
        // que lo suelte
      }
    } else {
      // Detectado rebote
    }
  } else if (controller->getIRReceiver()->isDataReady()) {
    IRReceiver::ValueRC5Sumo value = controller->getIRReceiver()->getValueRC5Sumo();
    uint32_t command = value.command;
    uint32_t address = value.address;
    if (command == F_E_IR_RC5SUMO_COMMAND_START_STOP && address == rc_address) { //stop
      switch ((enum_type_fura)fura_type) {
        case enum_type_fura::T_FURAFOSCAN1:
          controller->getMotorOneshot125Left()->disable();
          controller->getMotorOneshot125Right()->disable();
          break;
        case enum_type_fura::T_FURAFOSCAN2:
          controller->getMotorPWMLeft()->disable();
          controller->getMotorPWMRight()->disable();
          break;
        case enum_type_fura::T_FURAXICLE:
          controller->getMotorOneshot125Left()->disable();
          controller->getMotorOneshot125Right()->disable();
          break;
      }
      button_stop__button_previous_state = 1;
      goNextStep(enum_step_fura::STEP_WAIT_BUTTON_START_PRESSED);
      return;
    } else {
      // aun está pulsado el boton del estado anterior, se tiene que esperar a
      // que lo suelte
    }
  } else {
    // en el caso que se suelte el boton, se tiene que actualizar el estado de
    // boton anterior
    if (button_stop__button_previous_state == 1) {
      button_stop__button_previous_state = 0;
    }
  }
}

void FuraE::detectTimeOut(enum_step_fura next_step, uint32_t time_out) {
  if (time_out__time_tick_ini == 0) {
    // Se ha dejado de detectar al oponente
    time_out__time_tick_ini = HAL_GetTick();
  } else {
    // Determinar si hay rebote o se sigue sin detectar al oponente
    time_out__time_tick_current = HAL_GetTick() - time_out__time_tick_ini;
    if (time_out__time_tick_current < time_out) {
      // Esperando tiempo prudencial para dejar de buscar al oponente
    } else {
      // Pasa al siguiente estado
      time_out__time_tick_ini = 0;
      goNextStep(next_step);
      return;
    }
  }
}

void FuraE::detectSensorGyroUp(enum_step_fura next_step) {
#ifdef F_E_ACTIVE_SENSOR_TILTING
  controller->getSensorGyro()->read_values();
  sensor_gyro_in__y_value = controller->getSensorGyro()->get_accel_y();
  if (!(sensor_gyro_in__y_value > F_E_GYRO_Y_UP_PRE && sensor_gyro_in__y_value < F_E_GYRO_Y_UP_POST)){
    // Se ha empezado a detectar al oponente
    #ifdef F_E_ACTIVE_LED_SENSOR_TILTING
    if (controller->getLedDistanceCenter()->get_status() == 0) controller->getLedDistanceCenter()->turn_on();
    #endif
    if (sensor_gyro_in__time_tick_ini == 0) {
      // Empezamos a contar el tiempo del sensor de distancia esta activo
      sensor_gyro_in__time_tick_ini = HAL_GetTick();
    } else {
      // Esperamos a que termine 1ms para saber que el tracker se ha detectado y no hay rebotes
      sensor_gyro_in__time_tick_current = HAL_GetTick() - sensor_gyro_in__time_tick_ini;
      if (sensor_gyro_in__time_tick_current >= (*config_motor)[(size_t)enum_motor::F_M_SENSOR_TILTING_IN_MS].value) {
        // Oponente detectado descartando rebotes
        sensor_gyro_in__time_tick_ini = 0;
        goNextStep(next_step);
        return;
      } else {
        // Esperamos a que pase 1ms para descartar rebotes
      }
    }
  } else {
    #ifdef F_E_ACTIVE_LED_SENSOR_TILTING
    if (controller->getLedDistanceCenter()->get_status() == 1) controller->getLedDistanceCenter()->turn_off();
    #endif
    if (sensor_gyro_in__time_tick_ini != 0) {
      // Detectado rebote
      sensor_gyro_in__time_tick_ini = 0;
    } else {
      // En espera para detectar al oponente
    }
  }
#endif
}

void FuraE::detectSensorTiltingOut(enum_step_fura next_step, uint32_t time_out_detect) {
#ifdef F_E_ACTIVE_DISTANCE_SENSOR_CENTER
  controller->getSensorGyro()->read_values();
  sensor_gyro_in__y_value = controller->getSensorGyro()->get_accel_y();
  if (sensor_gyro_in__y_value > F_E_GYRO_Y_UP_PRE && sensor_gyro_in__y_value < F_E_GYRO_Y_UP_POST){
    #ifdef F_E_ACTIVE_LED_SENSOR_TILTING
    if (controller->getLedDistanceCenter()->get_status() == 1) controller->getLedDistanceCenter()->turn_off();
    #endif
    if (sensor_gyro_out__time_tick_ini == 0) {
      // Se ha dejado de detectar al oponente
      sensor_gyro_out__time_tick_ini = HAL_GetTick();
    } else {
      // Determinar si hay rebote o se sigue sin detectar al oponente
      sensor_gyro_out__time_tick_current = HAL_GetTick() - sensor_gyro_out__time_tick_ini;
      if (sensor_gyro_out__time_tick_current < time_out_detect) {
        // Esperando tiempo prudencial para dejar de buscar al oponente
      } else {
        // Pasa al siguiente estado
        sensor_gyro_out__time_tick_ini = 0;
        goNextStep(next_step);
        return;
      }
    }
  } else {
// Aun estamos detectando al oponente
    #ifdef F_E_ACTIVE_LED_SENSOR_TILTING
    if (controller->getLedDistanceCenter()->get_status() == 0) controller->getLedDistanceCenter()->turn_on();
    #endif
    if (sensor_gyro_out__time_tick_ini == 0) {
      // Espera a dejar de detectar al oponente
    } else {
      // Rebote detectado
      sensor_gyro_out__time_tick_ini = 0;
    }
  }
#endif
}

void FuraE::detectTrakerLeftIn(enum_step_fura next_step) {
#ifdef F_E_ACTIVE_TRACKER_SENSOR_LEFT
  // Detectamos si el tracker izquierdo toca la linea exterior
  tracker_left_in__tracker_value = controller->getTrackerLeft()->getValue();
  if (tracker_left_in__tracker_value < tracker_base_left_in_detect_line) {
    // se ha detectado el tracker left
    #ifdef F_E_ACTIVE_LED_TRACKER_SENSOR_LEFT
    if (!controller->getLedTrackerLeft()->get_status()) controller->getLedTrackerLeft()->turn_on();
    #endif
    if (tracker_left_in__time_tick_ini == 0) {
      // empezamos a contar el tiempo que el tracker izquierdo esta activo
      tracker_left_in__time_tick_ini = HAL_GetTick();
    } else {
      // esperamos a que termine 1ms para saber que el tracker se ha detectado
      // correctamente
      tracker_left_in__time_tick_current =
          HAL_GetTick() - tracker_left_in__time_tick_ini;
      if (tracker_left_in__time_tick_current >= 1) {
        // tracker detectado descartando rebotes
        tracker_left_in__time_tick_ini = 0;
        goNextStep(next_step);
        return;
      } else {
        // esperamos a que pase 1ms para descartar rebotes
      }
    }
  } else {
    #ifdef F_E_ACTIVE_LED_TRACKER_SENSOR_LEFT
    if (controller->getLedTrackerLeft()->get_status()) controller->getLedTrackerLeft()->turn_off();
    #endif
    if (tracker_left_in__time_tick_ini != 0) {
      // Detectado rebote
      tracker_left_in__time_tick_ini = 0;
    } else {
      // En espera para detectar thacker left
    }
  }
#endif
}

void FuraE::detectTrakerLeftOut(enum_step_fura next_step, uint32_t time_out_of_line) {
#ifdef F_E_ACTIVE_TRACKER_SENSOR_LEFT
  // Detectamos que el tracker izquierdo deja de detectar la linea exterior
  tracker_left_out__tracker_value = controller->getTrackerLeft()->getValue();
  if (tracker_left_out__tracker_value > tracker_base_left_out_detect_line) {
    #ifdef F_E_ACTIVE_LED_TRACKER_SENSOR_LEFT
    if (controller->getLedTrackerLeft()->get_status()) controller->getLedTrackerLeft()->turn_off();
    #endif
    if (tracker_left_out__time_tick_ini == 0) {
      // detectado primera fuera de linea
      tracker_left_out__time_tick_ini = HAL_GetTick();
    } else {
      // determinar si hay rebote o se ha salido de la linea
      tracker_left_out__time_tick_current = HAL_GetTick() - tracker_left_out__time_tick_ini;
      if (tracker_left_out__time_tick_current < time_out_of_line) {
        // esperando tiempo prudencial para salir de la linea, para que no
        // haya reentradas en la linea
      } else {
        // vuelve al estado anterior
        tracker_left_out__time_tick_ini = 0;
        goNextStep(next_step);
        return;
      }
    }
  } else {
    // aun esta sobre la linea, esperar a salir
    #ifdef F_E_ACTIVE_LED_TRACKER_SENSOR_LEFT
    if (!controller->getLedTrackerLeft()->get_status()) controller->getLedTrackerLeft()->turn_on();
    #endif
    if (tracker_left_out__time_tick_ini == 0) {
      // espera para salir de la linea
    } else {
      // rebote detectado
      tracker_left_out__time_tick_ini = 0;
    }
  }
  // Deteccion de cambio de velocidad
#endif
}


void FuraE::detectTrakerRightIn(enum_step_fura next_step) {
#ifdef F_E_ACTIVE_TRACKER_SENSOR_RIGHT
  // Detectamos si el tracker derecho toca la linea exterior
  tracker_right_in__tracker_value = controller->getTrackerRight()->getValue();
  if (tracker_right_in__tracker_value < tracker_base_right_in_detect_line) {
// se ha detectado el tracker right
    #ifdef F_E_ACTIVE_LED_TRACKER_SENSOR_RIGHT
    if (!controller->getLedTrackerRight()->get_status()) controller->getLedTrackerRight()->turn_on();
    #endif
    if (tracker_right_in__time_tick_ini == 0) {
      // empezamos a contar el tiempo que el tracker izquierdo esta activo
      tracker_right_in__time_tick_ini = HAL_GetTick();
    } else {
      // esperamos a que termine 1ms para saber que el tracker se ha detectado
      // correctamente
      tracker_right_in__time_tick_current = HAL_GetTick() - tracker_right_in__time_tick_ini;
      if (tracker_right_in__time_tick_current >= 1) {
        // tracker detectado descartando rebotes
        tracker_right_in__time_tick_ini = 0;
        goNextStep(next_step);
        return;
      } else {
        // esperamos a que pase 1ms para descartar rebotes
      }
    }
  } else {
    #ifdef F_E_ACTIVE_LED_TRACKER_SENSOR_RIGHT
    if (controller->getLedTrackerRight()->get_status()) controller->getLedTrackerRight()->turn_off();
    #endif
    if (tracker_right_in__time_tick_ini != 0) {
      // Detectado rebote
      tracker_right_in__time_tick_ini = 0;
    } else {
      // En espera para detectar thacker right
    }
  }
#endif
}

void FuraE::detectTrakerRightOut(enum_step_fura next_step, uint32_t time_out_of_line) {
#ifdef F_E_ACTIVE_TRACKER_SENSOR_RIGHT
  // Movimiento del servo
  tracker_right_out__tracker_value = controller->getTrackerRight()->getValue();
  if (tracker_right_out__tracker_value > tracker_base_right_out_detect_line) {
    #ifdef F_E_ACTIVE_LED_TRACKER_SENSOR_RIGHT
    if (controller->getLedTrackerRight()->get_status()) controller->getLedTrackerRight()->turn_off();
    #endif
    if (tracker_right_out__time_tick_ini == 0) {
      // detectado primera fuera de linea
      tracker_right_out__time_tick_ini = HAL_GetTick();
    } else {
      // determinar si hay rebote o se ha salido de la linea
      tracker_right_out__time_tick_current = HAL_GetTick() - tracker_right_out__time_tick_ini;
      if (tracker_right_out__time_tick_current < time_out_of_line) {
        // esperando tiempo prudencial para salir de la linea, para que no
        // haya reentradas en la linea
      } else {
        // vuelve al estado anterior
        tracker_right_out__time_tick_ini = 0;
        goNextStep(next_step);
        return;
      }
    }
  } else {
// aun esta sobre la linea, esperar a salir
    #ifdef F_E_ACTIVE_LED_TRACKER_SENSOR_RIGHT
    if (!controller->getLedTrackerRight()->get_status()) controller->getLedTrackerRight()->turn_on();
    #endif
    if (tracker_right_out__time_tick_ini == 0) {
      // espera para salir de la linea
    } else {
      // rebote detectado
      tracker_right_out__time_tick_ini = 0;
    }
  }
  // Deteccion de cambio de velocidad
#endif
}



void FuraE::detectDistanceLeftIn(enum_step_fura next_step) {
#ifdef F_E_ACTIVE_DISTANCE_SENSOR_LEFT
  distance_left_in__distance_value = controller->getDistanceTofLeft()->getDistance(F_E_TOF_DISTANCE_MIN, F_E_TOF_DISTANCE_MAX);
  if (distance_left_in__distance_value < F_E_TOF_DISTANCE_MAX) {
// Se ha empezado a detectar al oponente
    #ifdef F_E_ACTIVE_LED_DISTANCE_SENSOR_LEFT
    if (controller->getLedDistanceLeft()->get_status() == 0) controller->getLedDistanceLeft()->turn_on();
    #endif
    if (distance_left_in__time_tick_ini == 0) {
      // Empezamos a contar el tiempo del sensor de distancia esta activo
      distance_left_in__time_tick_ini = HAL_GetTick();
    } else {
      // Esperamos a que termine 1ms para saber que el tracker se ha detectado y no hay rebotes
      distance_left_in__time_tick_current = HAL_GetTick() - distance_left_in__time_tick_ini;
      if (distance_left_in__time_tick_current >= (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_IN_MS].value) {
        // Oponente detectado descartando rebotes
        distance_left_in__time_tick_ini = 0;
        goNextStep(next_step);
        return;
      } else {
        // Esperamos a que pase 1ms para descartar rebotes
      }
    }
  } else {
    #ifdef F_E_ACTIVE_LED_DISTANCE_SENSOR_LEFT
    if (controller->getLedDistanceLeft()->get_status() == 1) controller->getLedDistanceLeft()->turn_off();
    #endif
    if (distance_left_in__time_tick_ini != 0) {
      // Detectado rebote
      distance_left_in__time_tick_ini = 0;
    } else {
      // En espera para detectar al oponente
    }
  }
#endif
}

void FuraE::detectDistanceLeftOut(enum_step_fura next_step, uint32_t time_out_detect) {
#ifdef F_E_ACTIVE_DISTANCE_SENSOR_LEFT
  distance_left_out__distance_value = controller->getDistanceTofLeft()->getDistance(F_E_TOF_DISTANCE_MIN, F_E_TOF_DISTANCE_MAX);
  if (distance_left_out__distance_value > F_E_TOF_DISTANCE_MAX) {
// Se ha dejado de detectar al oponente
    #ifdef F_E_ACTIVE_LED_DISTANCE_SENSOR_LEFT
    if (controller->getLedDistanceLeft()->get_status() == 1) controller->getLedDistanceLeft()->turn_off();
    #endif
    if (distance_left_out__time_tick_ini == 0) {
      // Se ha dejado de detectar al oponente por primera vez, tiene que esperar un tiempo prudencial para que no sea un falso positivo
      distance_left_out__time_tick_ini = HAL_GetTick();
    } else {
      // Determinar si hay rebote o se sigue sin detectar al oponente
      distance_left_out__time_tick_current = HAL_GetTick() - distance_left_out__time_tick_ini;
      if (distance_left_out__time_tick_current < time_out_detect) {
        // Esperando tiempo prudencial para dejar de buscar al oponente
      } else {
        // Pasa al siguiente estado
        distance_left_out__time_tick_ini = 0;
        goNextStep(next_step);
        return;
      }
    }
  } else {
    // Aun estamos detectando al oponente
    #ifdef F_E_ACTIVE_LED_DISTANCE_SENSOR_LEFT
    if (controller->getLedDistanceLeft()->get_status() == 0) controller->getLedDistanceLeft()->turn_on();
    #endif
    if (distance_left_out__time_tick_ini == 0) {
      // Espera a dejar de detectar al oponente
    } else {
      // Rebote detectado
      distance_left_out__time_tick_ini = 0;
    }
  }
#endif
}

void FuraE::detectDistanceRightIn(enum_step_fura next_step) {
#ifdef F_E_ACTIVE_DISTANCE_SENSOR_RIGHT
  distance_right_in__distance_value = controller->getDistanceTofRight()->getDistance(F_E_TOF_DISTANCE_MIN, F_E_TOF_DISTANCE_MAX);
  if (distance_right_in__distance_value < F_E_TOF_DISTANCE_MAX) {
// Se ha empezado a detectar al oponente
    #ifdef F_E_ACTIVE_LED_DISTANCE_SENSOR_RIGHT
    if (controller->getLedDistanceRight()->get_status() == 0) controller->getLedDistanceRight()->turn_on();
    #endif
    if (distance_right_in__time_tick_ini == 0) {
      // Empezamos a contar el tiempo del sensor de distancia esta activo
      distance_right_in__time_tick_ini = HAL_GetTick();
    } else {
      // Esperamos a que termine 1ms para saber que el tracker se ha detectado y no hay rebotes
      distance_right_in__time_tick_current = HAL_GetTick() - distance_right_in__time_tick_ini;
      if (distance_right_in__time_tick_current >= (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_IN_MS].value) {
        // Oponente detectado descartando rebotes
        distance_right_in__time_tick_ini = 0;
        goNextStep(next_step);
        return;
      } else {
        // Esperamos a que pase 1ms para descartar rebotes
      }
    }
  } else {
    #ifdef F_E_ACTIVE_LED_DISTANCE_SENSOR_RIGHT
      if (controller->getLedDistanceRight()->get_status() == 1) controller->getLedDistanceRight()->turn_off();
    #endif
    if (distance_right_in__time_tick_ini != 0) {
      // Detectado rebote
      distance_right_in__time_tick_ini = 0;
    } else {
      // En espera para detectar al oponente
    }
  }
#endif
}

void FuraE::detectDistanceRightOut(enum_step_fura next_step, uint32_t time_out_detect) {
#ifdef F_E_ACTIVE_DISTANCE_SENSOR_RIGHT
  // Detectamos que el tracker izquierdo deja de detectar la linea exterior
  distance_right_out__distance_value = controller->getDistanceTofRight()->getDistance(F_E_TOF_DISTANCE_MIN, F_E_TOF_DISTANCE_MAX);
  if (distance_right_out__distance_value > F_E_TOF_DISTANCE_MAX) {
    // Se ha dejado de detectar al oponente
    #ifdef F_E_ACTIVE_LED_DISTANCE_SENSOR_RIGHT
      if (controller->getLedDistanceRight()->get_status() == 1) controller->getLedDistanceRight()->turn_off();
    #endif
    if (distance_right_out__time_tick_ini == 0) {
      // Se ha dejado de detectar al oponente por primera vez, tiene que esperar un tiempo prudencial para que no sea un falso positivo
      distance_right_out__time_tick_ini = HAL_GetTick();
    } else {
      // Determinar si hay rebote o se sigue sin detectar al oponente
      distance_right_out__time_tick_current = HAL_GetTick() - distance_right_out__time_tick_ini;
      if (distance_right_out__time_tick_current < time_out_detect) {
        // Esperando tiempo prudencial para dejar de buscar al oponente
      } else {
        // Pasa al siguiente estado
        distance_right_out__time_tick_ini = 0;
        goNextStep(next_step);
        return;
      }
    }
  } else {
    // Aun estamos detectando al oponente
    #ifdef F_E_ACTIVE_LED_DISTANCE_SENSOR_RIGHT
      if (controller->getLedDistanceRight()->get_status() == 0) controller->getLedDistanceRight()->turn_on();
    #endif
    if (distance_right_out__time_tick_ini == 0) {
      // Espera a dejar de detectar al oponente
    } else {
      // Rebote detectado
      distance_right_out__time_tick_ini = 0;
    }
  }
#endif
}

void FuraE::detectDistanceCenterIn(enum_step_fura next_step) {
#ifdef F_E_ACTIVE_DISTANCE_SENSOR_CENTER
  distance_center_in__distance_value = controller->getDistanceTofCenter()->getDistance(F_E_TOF_DISTANCE_MIN, F_E_TOF_DISTANCE_MAX);
  if (distance_center_in__distance_value < F_E_TOF_DISTANCE_MAX) {
// Se ha empezado a detectar al oponente
    #ifdef F_E_ACTIVE_LED_DISTANCE_SENSOR_CENTER
      if (controller->getLedDistanceCenter()->get_status() == 0) controller->getLedDistanceCenter()->turn_on();
    #endif
    if (distance_center_in__time_tick_ini == 0) {
      // Empezamos a contar el tiempo del sensor de distancia esta activo
      distance_center_in__time_tick_ini = HAL_GetTick();
    } else {
      // Esperamos a que termine 1ms para saber que el tracker se ha detectado y no hay rebotes
      distance_center_in__time_tick_current = HAL_GetTick() - distance_center_in__time_tick_ini;
      if (distance_center_in__time_tick_current >= (*config_motor)[(size_t)enum_motor::F_M_DISTANCE_CENTER_BOOST_MS].value) {
        // Oponente detectado descartando rebotes
        distance_center_in__time_tick_ini = 0;
        goNextStep(next_step);
        return;
      } else {
        // Esperamos a que pase 1ms para descartar rebotes
      }
    }
  } else {
    #ifdef F_E_ACTIVE_LED_DISTANCE_SENSOR_CENTER
    if (controller->getLedDistanceCenter()->get_status() == 1) controller->getLedDistanceCenter()->turn_off();
    #endif
    if (distance_center_in__time_tick_ini != 0) {
      // Detectado rebote
      distance_center_in__time_tick_ini = 0;
    } else {
      // En espera para detectar al oponente
    }
  }
#endif
}

void FuraE::detectDistanceCenterOut(enum_step_fura next_step, uint32_t time_out_detect) {
#ifdef F_E_ACTIVE_DISTANCE_SENSOR_CENTER
  distance_center_out__distance_value = controller->getDistanceTofCenter()->getDistance(F_E_TOF_DISTANCE_MIN, F_E_TOF_DISTANCE_MAX);
  if (distance_center_out__distance_value > F_E_TOF_DISTANCE_MAX) {
    #ifdef F_E_ACTIVE_LED_DISTANCE_SENSOR_CENTER
    if (controller->getLedDistanceCenter()->get_status() == 1) controller->getLedDistanceCenter()->turn_off();
    #endif
    if (distance_center_out__time_tick_ini == 0) {
      // Se ha dejado de detectar al oponente
      distance_center_out__time_tick_ini = HAL_GetTick();
    } else {
      // Determinar si hay rebote o se sigue sin detectar al oponente
      distance_center_out__time_tick_current = HAL_GetTick() - distance_center_out__time_tick_ini;
      if (distance_center_out__time_tick_current < time_out_detect) {
        // Esperando tiempo prudencial para dejar de buscar al oponente
      } else {
        // Pasa al siguiente estado
        distance_center_out__time_tick_ini = 0;
        goNextStep(next_step);
        return;
      }
    }
  } else {
    // Aun estamos detectando al oponente
    #ifdef F_E_ACTIVE_LED_DISTANCE_SENSOR_CENTER
    if (controller->getLedDistanceCenter()->get_status() == 0) controller->getLedDistanceCenter()->turn_on();
    #endif
    if (distance_center_out__time_tick_ini == 0) {
      // Espera a dejar de detectar al oponente
    } else {
      // Rebote detectado
      distance_center_out__time_tick_ini = 0;
    }
  }
#endif
}

void FuraE::setMotorSpeed(uint32_t speed_left_value, uint32_t speed_right_value) {
  switch ((enum_type_fura)fura_type) {
    case enum_type_fura::T_FURAFOSCAN1:
      controller->getMotorOneshot125Left()->setSpeed(speed_left_value);
      controller->getMotorOneshot125Right()->setSpeed(speed_right_value);
      break;
    case enum_type_fura::T_FURAFOSCAN2:
      controller->getMotorPWMLeft()->setSpeed(speed_left_value);
      controller->getMotorPWMRight()->setSpeed(speed_right_value);
      break;
    case enum_type_fura::T_FURAXICLE:
      controller->getMotorOneshot125Left()->setSpeed(speed_left_value);
      controller->getMotorOneshot125Right()->setSpeed(speed_right_value);
      break;
  }
  speed_left_ini = speed_left_value;
  speed_left_end = speed_left_value;
  speed_right_ini = speed_right_value;
  speed_right_end = speed_right_value;
  speed_ramp__time_tick_ini = 0;
  speed_refresh__time_tick_ini = 0;
}

void FuraE::setMotorSpeedRamp(uint32_t speed_left_ini_value, uint32_t speed_left_end_value, uint32_t speed_right_ini_value, uint32_t speed_right_end_value) {
  if (speed_left_ini_value <= speed_left_ini) {
    if (speed_left_end_value <= speed_left_ini) {
      speed_left_ini = speed_left_end_value;
    } else {
      // Seguimos incrementando la velocidad a partir de la velocidad anterior
    }
  } else {
    speed_left_ini = speed_left_ini_value;
  }
  speed_left_end = speed_left_end_value;

  if (speed_right_ini_value <= speed_right_ini) {
    if (speed_right_end_value < speed_right_ini) {
      speed_right_ini = speed_right_end_value;
    } else {
      // Seguimos incrementando la velocidad a partir de la velocidad anterior
    }
  } else {
    speed_right_ini = speed_right_ini_value;
  }
  speed_right_end = speed_right_end_value;

  switch ((enum_type_fura)fura_type) {
    case enum_type_fura::T_FURAFOSCAN1:
      controller->getMotorOneshot125Left()->setSpeed(speed_left_ini);
      controller->getMotorOneshot125Right()->setSpeed(speed_right_ini);
      break;
    case enum_type_fura::T_FURAFOSCAN2:
      controller->getMotorPWMLeft()->setSpeed(speed_left_ini);
      controller->getMotorPWMRight()->setSpeed(speed_right_ini);
      break;
    case enum_type_fura::T_FURAXICLE:
      controller->getMotorOneshot125Left()->setSpeed(speed_left_ini);
      controller->getMotorOneshot125Right()->setSpeed(speed_right_ini);
      break;
  }
  speed_ramp__time_tick_ini = 0;
  speed_refresh__time_tick_ini = 0;
}

void FuraE::rampMotorSpeed(uint32_t speed_ramp_time_increment_ms, uint32_t speed_ramp_value_increment) {
  if (speed_left_ini != speed_left_end || speed_right_ini != speed_right_end) {
    if (speed_ramp__time_tick_ini == 0) {
      // Iniciamos el contador de tiempo
      speed_ramp__time_tick_ini = HAL_GetTick();
    } else {
      // Esperamos a que termine 1ms para saber que el tracker se ha detectado y no hay rebotes
      speed_ramp__time_tick_current = HAL_GetTick() - speed_ramp__time_tick_ini;
      if (speed_ramp__time_tick_current >= speed_ramp_time_increment_ms) {
        if (speed_left_ini != speed_left_end) {
          speed_left_ini += speed_ramp_value_increment;
          if (speed_left_ini > speed_left_end) {
            speed_left_ini = speed_left_end;
          }
          switch ((enum_type_fura)fura_type) {
            case enum_type_fura::T_FURAFOSCAN1:
              controller->getMotorOneshot125Left()->setSpeed(speed_left_ini);
              break;
            case enum_type_fura::T_FURAFOSCAN2:
              controller->getMotorPWMLeft()->setSpeed(speed_left_ini);
              break;
            case enum_type_fura::T_FURAXICLE:
              controller->getMotorOneshot125Left()->setSpeed(speed_left_ini);
              break;
          }
        }
        if (speed_right_ini != speed_right_end) {
          speed_right_ini += speed_ramp_value_increment;
          if (speed_right_ini > speed_right_end) {
            speed_right_ini = speed_right_end;
          }
          switch ((enum_type_fura)fura_type) {
            case enum_type_fura::T_FURAFOSCAN1:
              controller->getMotorOneshot125Right()->setSpeed(speed_right_ini);
              break;
            case enum_type_fura::T_FURAFOSCAN2:
              controller->getMotorPWMRight()->setSpeed(speed_right_ini);
              break;
            case enum_type_fura::T_FURAXICLE:
              controller->getMotorOneshot125Right()->setSpeed(speed_right_ini);
              break;
          }
        }
        speed_ramp__time_tick_ini = HAL_GetTick();
        speed_refresh__time_tick_ini = 0;
      } else {
        // Esperamos a que pase el tiempo
      }
    }
  }
  else {
    if (speed_ramp__time_tick_ini == 0) {
      // Iniciamos el contador de tiempo
      speed_refresh__time_tick_ini = HAL_GetTick();
    } else {
      speed_refresh__time_tick_current = HAL_GetTick() - speed_refresh__time_tick_ini;
      if (speed_refresh__time_tick_current >= (*config_motor)[(size_t)enum_motor::F_M_SPEED_REFRESH_MS].value) {
        switch ((enum_type_fura)fura_type) {
          case enum_type_fura::T_FURAFOSCAN1:
            controller->getMotorOneshot125Left()->setSpeed(speed_left_ini);
            controller->getMotorOneshot125Right()->setSpeed(speed_right_ini);
            break;
          case enum_type_fura::T_FURAFOSCAN2:
            controller->getMotorPWMLeft()->setSpeed(speed_left_ini);
            controller->getMotorPWMRight()->setSpeed(speed_right_ini);
            break;
          case enum_type_fura::T_FURAXICLE:
            controller->getMotorOneshot125Left()->setSpeed(speed_left_ini);
            controller->getMotorOneshot125Right()->setSpeed(speed_right_ini);
            break;
        }
        speed_refresh__time_tick_ini = 0;
      }
    }
  }
}

void FuraE::setDirectionCenter() {
  if (direction_base != enum_direction::DIR_CENTER) {
    direction_base = enum_direction::DIR_CENTER;
    esc_speed_base_left = (*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_LEFT].value;
    esc_speed_base_right = (*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RIGHT].value;
  }
}
void FuraE::setDirectionLeft() {
  if (direction_base != enum_direction::DIR_LEFT) {
    direction_base = enum_direction::DIR_LEFT;
    esc_speed_base_left = (*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_FAST].value;
    esc_speed_base_right = (*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SLOW].value;
  }
}
void FuraE::setDirectionRight() {
  if (direction_base != enum_direction::DIR_RIGHT) {
    direction_base = enum_direction::DIR_RIGHT;
    esc_speed_base_left = (*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SLOW].value;
    esc_speed_base_right = (*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_FAST].value;
  }
}

void FuraE::writeDataFlash(uint32_t tracker_base_left_in_detect_line_value,
                           uint32_t tracker_base_right_in_detect_line_value,
                           uint32_t tracker_base_left_out_detect_line_value,
                           uint32_t tracker_base_right_out_detect_line_value,
                           uint32_t rc_address_value,
                           uint32_t fura_type_value) {
  uint32_t rx_data[FLASH_DATA_NUM_VALUES];
  rx_data[0] = FLASH_DATA_VERSION;
  rx_data[1] = tracker_base_left_in_detect_line_value;
  rx_data[2] = tracker_base_right_in_detect_line_value;
  rx_data[3] = tracker_base_left_out_detect_line_value;
  rx_data[4] = tracker_base_right_out_detect_line_value;
  rx_data[5] = rc_address_value;
  rx_data[6] = fura_type_value;
  controller->getFlashMemory()->write(rx_data, FLASH_DATA_NUM_VALUES);
}
