#ifndef FURAE_HPP
#define FURAE_HPP

#define FURAFOSCAN1 "FURAFOSCA1"
//#define FURAFOSCAN2 "FURAFOSCA2"

#define FLASH_DATA_VERSION 3
#define FLASH_DATA_NUM_VALUES 7

//#define F_E_ACTIVE_SENSOR_TILTING
//#define F_E_ACTIVE_LED_SENSOR_TILTING

#define F_E_ACTIVE_TRACKER_SENSOR_LEFT
#define F_E_ACTIVE_TRACKER_SENSOR_RIGHT
#define F_E_ACTIVE_DISTANCE_SENSOR_LEFT
#define F_E_ACTIVE_DISTANCE_SENSOR_RIGHT
#define F_E_ACTIVE_DISTANCE_SENSOR_CENTER

//#define F_E_ACTIVE_LED_TRACKER_SENSOR_LEFT
//#define F_E_ACTIVE_LED_TRACKER_SENSOR_RIGHT
//#define F_E_ACTIVE_LED_DISTANCE_SENSOR_LEFT
//#define F_E_ACTIVE_LED_DISTANCE_SENSOR_RIGHT
//#define F_E_ACTIVE_LED_DISTANCE_SENSOR_CENTER

#define F_E_BUTTON_START_WAIT 1000//4890



#define F_E_TRACKER_LEFT_IN_DETECT_LINE 3000
#define F_E_TRACKER_LEFT_OUT_DETECT_LINE 3300
#define F_E_TRACKER_RIGHT_IN_DETECT_LINE 3000
#define F_E_TRACKER_RIGHT_OUT_DETECT_LINE 3300


#define F_E_TOF_DISTANCE_MIN 0
#define F_E_TOF_DISTANCE_MAX 400//450
#define F_E_TOF_DISTANCE_DETECT 0

#define F_E_GYRO_Y_UP_PRE 0
#define F_E_GYRO_Y_UP_POST 4500

#define F_E_IR_NEC_ON 33456510
#define F_E_IR_NEC_OK 33474615
#define F_E_IR_NEC_BACK 33481755
#define F_E_IR_NEC_UP 33430755
#define F_E_IR_NEC_DOWN 33424125
#define F_E_IR_NEC_LEFT 33483795
#define F_E_IR_NEC_RIGHT 33463395

#define F_E_IR_NEC_HOME 33476145
#define F_E_IR_NEC_MENU 33472830
#define F_E_IR_NEC_VOLUME_UP 33480990
#define F_E_IR_NEC_VOLUME_DOWN 33460590
#define F_E_IR_NEC_MOUSE 33428205

#define F_E_IR_RC5SUMO_COMMAND_PROGRAM 0xb
#define F_E_IR_RC5SUMO_COMMAND_START_STOP 0x7

#include <ControllerBase.hpp>

  // 1. Definimos el ENUM de los parámetros
  enum class enum_motor {

    F_M_ESC_MID_STOP,
    F_M_ESC_BASE_SPEED_RAMP_INI,
    F_M_ESC_BASE_SPEED_LEFT,
    F_M_ESC_BASE_SPEED_RIGHT,
    F_M_ESC_BASE_SLOW,
    F_M_ESC_BASE_FAST,
    F_M_ESC_TRAKER_SLOW_SPEED,//no utilizado
    F_M_ESC_TRACKER_SPEED,//no utilizado
    F_M_ESC_TRAKER_BACK,
    F_M_ESC_TRAKER_ROTATE_SLOW,
    F_M_ESC_TRAKER_ROTATE_FAST,
    F_M_ESC_TRACKER_BOTH_SPEED_FAST,
    F_M_ESC_TRACKER_BOTH_SPEED_SLOW,
    F_M_ESC_CHECK,

    F_M_SPEED_RAMP_TIME_INCREMENT_MS,
    F_M_SPEED_RAMP_TIME_INCREMENT_FROM_BACK_MS,
    F_M_SPEED_RAMP_VALUE_INCREMENT,
    F_M_SPEED_RAMP_VALUE_INCREMENT_FROM_BACK,
    F_M_SPEED_REFRESH_MS,
    F_M_SPEED_RAMP_CHECK,

    F_M_TRACKER_OUT_OF_LINE_MS,
    F_M_TRACKER_ROTATE_MS,
    F_M_TRACKER_OUT_OF_LINE_BOTH_MS,
    F_M_TRACKER_CHECK,

    F_M_DEFENSE_BACK_FAST,
    F_M_DEFENSE_BACK_SLOW,
    F_M_DEFENSE_SPIN_FRONT,
    F_M_DEFENSE_SPIN_BACK,
    F_M_DEFENSE_ATTACK,
    F_M_DEFENSE_LEFT_OUT_MS,
    F_M_DEFENSE_RIGHT_OUT_MS,
    F_M_DEFENSE_SPIN_OUT_MS,
    F_M_DEFENSE_ATTACK_OUT_MS,//no utilizado
    F_M_DEFENSE_CHECK,

    F_M_SENSOR_TILTING_BACK_FAST,
    F_M_SENSOR_TILTING_BACK_SLOW,
    F_M_SENSOR_TILTING_IN_MS,
    F_M_SENSOR_TILTING_OUT_MS,
    F_M_SENSOR_TILTING_CHECK,

    F_M_DISTANCE_SPEED_SLOW_LEFT,
    F_M_DISTANCE_SPEED_SLOW_RIGHT,
    F_M_DISTANCE_SPEED_LEFT,
    F_M_DISTANCE_SPEED_RIGHT,
    F_M_DISTANCE_BOTH_SPEED,
    F_M_DISTANCE_BOTH_SPEED_BOOST,
    F_M_DISTANCE_IN_MS,
    F_M_DISTANCE_OUT_MS,
    F_M_DISTANCE_BOTH_OUT_MS,
    F_M_DISTANCE_CENTER_BOOST_MS,
    F_M_DISTANCE_BOTH_BOOST_OUT_MS,
    F_M_DISTANCE_CHECK,


    COUNT // Truco para saber el tamaño automáticamente
  };

  // 2. Estructura que contiene el valor del ajuste
  struct ConfigValue {
    uint32_t value;
  };

  // 3. Definición de las tablas para cada tipo de chip/motor
  // Usamos arrays constantes para que vivan en la memoria Flash
  namespace Configs {
      constexpr std::array<ConfigValue, (size_t)enum_motor::COUNT> Furafoscan1 = {{
          [(size_t)enum_motor::F_M_ESC_MID_STOP] = { 1920 }, // 1830-2000
          [(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI] = { 2100 },
          [(size_t)enum_motor::F_M_ESC_BASE_SPEED_LEFT] = { 2300 }, //2110//TTT2200
          [(size_t)enum_motor::F_M_ESC_BASE_SPEED_RIGHT] = { 2300 }, //2110//TTT2200
          [(size_t)enum_motor::F_M_ESC_BASE_SLOW] = { 2300 }, //2250
          [(size_t)enum_motor::F_M_ESC_BASE_FAST] = { 2430 }, //2350
          [(size_t)enum_motor::F_M_ESC_TRAKER_SLOW_SPEED] = { 2050 },//No utilizado
          [(size_t)enum_motor::F_M_ESC_TRACKER_SPEED] = { 2450 },//No utilizado
          [(size_t)enum_motor::F_M_ESC_TRAKER_BACK] = { 1560 }, //1750//TTT1560
          [(size_t)enum_motor::F_M_ESC_TRAKER_ROTATE_SLOW] = { 1700 },
          [(size_t)enum_motor::F_M_ESC_TRAKER_ROTATE_FAST] = { 2400 },
          [(size_t)enum_motor::F_M_ESC_TRACKER_BOTH_SPEED_FAST] = { 1400 },
          [(size_t)enum_motor::F_M_ESC_TRACKER_BOTH_SPEED_SLOW] = { 1400 },
          [(size_t)enum_motor::F_M_ESC_CHECK] = { 99999 },

          [(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS] = { 1 },
          [(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_FROM_BACK_MS] = { 1 },
          [(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT] = { 2 },
          [(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT_FROM_BACK] = { 1 }, //No utilizado
          [(size_t)enum_motor::F_M_SPEED_REFRESH_MS] = { 10 },
          [(size_t)enum_motor::F_M_SPEED_RAMP_CHECK] = { 99999 },

          [(size_t)enum_motor::F_M_TRACKER_OUT_OF_LINE_MS] = { 70 },
          [(size_t)enum_motor::F_M_TRACKER_ROTATE_MS] = { 350 },
          [(size_t)enum_motor::F_M_TRACKER_OUT_OF_LINE_BOTH_MS] = { 100 },
          [(size_t)enum_motor::F_M_TRACKER_CHECK] = { 99999 },

          [(size_t)enum_motor::F_M_DEFENSE_BACK_FAST] = { 1400 },
          [(size_t)enum_motor::F_M_DEFENSE_BACK_SLOW] = { 1650 },
          [(size_t)enum_motor::F_M_DEFENSE_SPIN_FRONT] = { 2330 },
          [(size_t)enum_motor::F_M_DEFENSE_SPIN_BACK] = { 1400 },
          [(size_t)enum_motor::F_M_DEFENSE_ATTACK] = { 2450 },
          [(size_t)enum_motor::F_M_DEFENSE_LEFT_OUT_MS] = { 300 },
          [(size_t)enum_motor::F_M_DEFENSE_RIGHT_OUT_MS] = { 300 },
          [(size_t)enum_motor::F_M_DEFENSE_SPIN_OUT_MS] = { 550 },
          [(size_t)enum_motor::F_M_DEFENSE_ATTACK_OUT_MS] = { 650 },
          [(size_t)enum_motor::F_M_DEFENSE_CHECK] = { 99999 },

          [(size_t)enum_motor::F_M_SENSOR_TILTING_BACK_FAST] = { 1400 },
          [(size_t)enum_motor::F_M_SENSOR_TILTING_BACK_SLOW] = { 1650 },
          [(size_t)enum_motor::F_M_SENSOR_TILTING_IN_MS] = { 50 },
          [(size_t)enum_motor::F_M_SENSOR_TILTING_OUT_MS] = { 30 },
          [(size_t)enum_motor::F_M_SENSOR_TILTING_CHECK] = { 99999 },


          [(size_t)enum_motor::F_M_DISTANCE_SPEED_SLOW_LEFT] = { 2350 },
          [(size_t)enum_motor::F_M_DISTANCE_SPEED_SLOW_RIGHT] = { 2350 },
          [(size_t)enum_motor::F_M_DISTANCE_SPEED_LEFT] = { 2400 },
          [(size_t)enum_motor::F_M_DISTANCE_SPEED_RIGHT] = { 2000 },
          [(size_t)enum_motor::F_M_DISTANCE_BOTH_SPEED] = { 2350 },
          [(size_t)enum_motor::F_M_DISTANCE_BOTH_SPEED_BOOST] = { 2350 },
          [(size_t)enum_motor::F_M_DISTANCE_IN_MS] = { 1 },
          [(size_t)enum_motor::F_M_DISTANCE_OUT_MS] = { 20 },
          [(size_t)enum_motor::F_M_DISTANCE_BOTH_OUT_MS] = { 20 },
          [(size_t)enum_motor::F_M_DISTANCE_CENTER_BOOST_MS] = { 1 },
          [(size_t)enum_motor::F_M_DISTANCE_BOTH_BOOST_OUT_MS] = { 20 },
          [(size_t)enum_motor::F_M_DISTANCE_CHECK] = { 99999 },


      }};

      constexpr std::array<ConfigValue, (size_t)enum_motor::COUNT> Furafoscan2 = {{
          // 1000-2000
          [(size_t)enum_motor::F_M_ESC_MID_STOP] = { 1500 }, //FF2 1500 falta calcular
          [(size_t)enum_motor::F_M_ESC_BASE_SPEED_RAMP_INI] = { 1560 }, //FF2
          [(size_t)enum_motor::F_M_ESC_BASE_SPEED_LEFT] = { 1570 }, //FF2
          [(size_t)enum_motor::F_M_ESC_BASE_SPEED_RIGHT] = { 1570 }, //FF2
          [(size_t)enum_motor::F_M_ESC_BASE_SLOW] = { 1580 },//FF2
          [(size_t)enum_motor::F_M_ESC_BASE_FAST] = { 1660 },//FF2
          [(size_t)enum_motor::F_M_ESC_TRAKER_SLOW_SPEED] = { 1600 },//No utilizado
          [(size_t)enum_motor::F_M_ESC_TRACKER_SPEED] = { 1600 },//No utilizado
          [(size_t)enum_motor::F_M_ESC_TRAKER_BACK] = { 1450 }, //FF2//1750//TTT1560
          [(size_t)enum_motor::F_M_ESC_TRAKER_ROTATE_SLOW] = { 1450 },//FF2
          [(size_t)enum_motor::F_M_ESC_TRAKER_ROTATE_FAST] = { 1700 },//FF2
          [(size_t)enum_motor::F_M_ESC_TRACKER_BOTH_SPEED_FAST] = { 1600 },//FF2
          [(size_t)enum_motor::F_M_ESC_TRACKER_BOTH_SPEED_SLOW] = { 1400 },//FF2
          [(size_t)enum_motor::F_M_ESC_CHECK] = { 99999 },

          [(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_MS] = { 7 }, //FF2
          [(size_t)enum_motor::F_M_SPEED_RAMP_TIME_INCREMENT_FROM_BACK_MS] = { 7 }, //FF2
          [(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT] = { 8 }, //FF2
          [(size_t)enum_motor::F_M_SPEED_RAMP_VALUE_INCREMENT_FROM_BACK] = { 8 }, //FF2
          [(size_t)enum_motor::F_M_SPEED_REFRESH_MS] = { 10 },
          [(size_t)enum_motor::F_M_SPEED_RAMP_CHECK] = { 99999 },

          [(size_t)enum_motor::F_M_TRACKER_OUT_OF_LINE_MS] = { 100 }, //FF2
          [(size_t)enum_motor::F_M_TRACKER_ROTATE_MS] = { 300 },
          [(size_t)enum_motor::F_M_TRACKER_OUT_OF_LINE_BOTH_MS] = { 800 },
          [(size_t)enum_motor::F_M_TRACKER_CHECK] = { 99999 },

          [(size_t)enum_motor::F_M_DEFENSE_BACK_FAST] = { 1400 },
          [(size_t)enum_motor::F_M_DEFENSE_BACK_SLOW] = { 1650 },
          [(size_t)enum_motor::F_M_DEFENSE_SPIN_FRONT] = { 2330 },
          [(size_t)enum_motor::F_M_DEFENSE_SPIN_BACK] = { 1400 },
          [(size_t)enum_motor::F_M_DEFENSE_ATTACK] = { 2450 },
          [(size_t)enum_motor::F_M_DEFENSE_LEFT_OUT_MS] = { 600 },
          [(size_t)enum_motor::F_M_DEFENSE_RIGHT_OUT_MS] = { 650 },
          [(size_t)enum_motor::F_M_DEFENSE_SPIN_OUT_MS] = { 550 },
          [(size_t)enum_motor::F_M_DEFENSE_ATTACK_OUT_MS] = { 650 },
          [(size_t)enum_motor::F_M_DEFENSE_CHECK] = { 99999 },

          [(size_t)enum_motor::F_M_SENSOR_TILTING_BACK_FAST] = { 1400 },
          [(size_t)enum_motor::F_M_SENSOR_TILTING_BACK_SLOW] = { 1650 },
          [(size_t)enum_motor::F_M_SENSOR_TILTING_IN_MS] = { 1 },
          [(size_t)enum_motor::F_M_SENSOR_TILTING_OUT_MS] = { 1000 },
          [(size_t)enum_motor::F_M_SENSOR_TILTING_CHECK] = { 99999 },

          [(size_t)enum_motor::F_M_DISTANCE_SPEED_SLOW_LEFT] = { 2200 },
          [(size_t)enum_motor::F_M_DISTANCE_SPEED_SLOW_RIGHT] = { 2200 },
          [(size_t)enum_motor::F_M_DISTANCE_SPEED_LEFT] = { 2380 },
          [(size_t)enum_motor::F_M_DISTANCE_SPEED_RIGHT] = { 2380 },
          [(size_t)enum_motor::F_M_DISTANCE_BOTH_SPEED] = { 2380 },
          [(size_t)enum_motor::F_M_DISTANCE_BOTH_SPEED_BOOST] = { 2550 },
          [(size_t)enum_motor::F_M_DISTANCE_IN_MS] = { 1 },
          [(size_t)enum_motor::F_M_DISTANCE_OUT_MS] = { 60 },
          [(size_t)enum_motor::F_M_DISTANCE_BOTH_OUT_MS] = { 100 },
          [(size_t)enum_motor::F_M_DISTANCE_CENTER_BOOST_MS] = { 11 },
          [(size_t)enum_motor::F_M_DISTANCE_BOTH_BOOST_OUT_MS] = { 1000 },
          [(size_t)enum_motor::F_M_DISTANCE_CHECK] = { 99999 },
      }};
  }

  // Si el tamaño del enum cambia y no actualizas la lógica, esto falla en compilación
//  static_assert((size_t)enum_motor::COUNT == 14, "¡Ojo! Has añadido un parámetro al enum pero no has actualizado las tablas.");
  static_assert((uint32_t)Configs::Furafoscan1[(size_t)enum_motor::F_M_ESC_CHECK].value == 99999, "Error falta valor en array Furafosca1");
  static_assert((uint32_t)Configs::Furafoscan2[(size_t)enum_motor::F_M_ESC_CHECK].value == 99999, "Error falta valor en array Furafosca2");
  static_assert((uint32_t)Configs::Furafoscan1[(size_t)enum_motor::F_M_SPEED_RAMP_CHECK].value == 99999, "Error falta valor en array Furafosca1");
  static_assert((uint32_t)Configs::Furafoscan2[(size_t)enum_motor::F_M_SPEED_RAMP_CHECK].value == 99999, "Error falta valor en array Furafosca2");
  static_assert((uint32_t)Configs::Furafoscan1[(size_t)enum_motor::F_M_DEFENSE_CHECK].value == 99999, "Error falta valor en array Furafosca1");
  static_assert((uint32_t)Configs::Furafoscan2[(size_t)enum_motor::F_M_DEFENSE_CHECK].value == 99999, "Error falta valor en array Furafosca2");
  static_assert((uint32_t)Configs::Furafoscan1[(size_t)enum_motor::F_M_SENSOR_TILTING_CHECK].value == 99999, "Error falta valor en array Furafosca1");
  static_assert((uint32_t)Configs::Furafoscan2[(size_t)enum_motor::F_M_SENSOR_TILTING_CHECK].value == 99999, "Error falta valor en array Furafosca2");
  static_assert((uint32_t)Configs::Furafoscan1[(size_t)enum_motor::F_M_TRACKER_CHECK].value == 99999, "Error falta valor en array Furafosca1");
  static_assert((uint32_t)Configs::Furafoscan2[(size_t)enum_motor::F_M_TRACKER_CHECK].value == 99999, "Error falta valor en array Furafosca2");
  static_assert((uint32_t)Configs::Furafoscan1[(size_t)enum_motor::F_M_DISTANCE_CHECK].value == 99999, "Error falta valor en array Furafosca1");
  static_assert((uint32_t)Configs::Furafoscan2[(size_t)enum_motor::F_M_DISTANCE_CHECK].value == 99999, "Error falta valor en array Furafosca2");



class FuraE {
 public:
  FuraE(ControllerBase* controllerBaseValue);

  void main();

 private:
  ControllerBase* controller;  // Puntero al controlador base
  // Puntero a la tabla activa
  uint32_t fura_type = 1;
  const std::array<ConfigValue, (size_t)enum_motor::COUNT>* config_motor = nullptr;

  enum enum_type_fura {
    T_FURAFOSCAN1 = 1,
    T_FURAFOSCAN2 = 2,
    T_FURAXICLE = 3,
  };

  enum enum_step_fura {
    STEP_WAIT_BUTTON_START_PRESSED,
    STEP_WAIT_TRACKER_CALIBRATE_WHITE,
    STEP_INIT_TRACKER_CALIBRATE_WHITE,
    STEP_WAIT_TRACKER_CALIBRATE_BLACK,
    STEP_INIT_TRACKER_CALIBRATE_BLACK,
    STEP_TEST_ALL_SENSORS,
    STEP_WAIT_REMOTE_SUMO,
    STEP_BUTTON_START_PRESSED,
    STEP_DEFENSE_LEFT,
    STEP_DEFENSE_RIGHT,
    STEP_DEFENSE_SPIN,
    STEP_DEFENSE__STEP2_ATTACK,
    STEP_MOTOR_START,
    STEP_SENSOR_TILTING_DETECTED,
    STEP_TRACKER_LEFT_DETECTED,
    STEP_TRACKER_LEFT_DETECTED__STEP2_ROTATE,
    STEP_TRACKER_LEFT_DETECTED__STEP2_RIGHT_DETECTED,
    STEP_TRACKER_RIGHT_DETECTED,
    STEP_TRACKER_RIGHT_DETECTED__STEP2_ROTATE,
    STEP_TRACKER_RIGHT_DETECTED__STEP2_LEFT_DETECTED,
    STEP_DISTANCE_LEFT_DETECTED,
    STEP_DISTANCE_RIGHT_DETECTED,
    STEP_DISTANCE_BOTH_DETECTED,
    STEP_DISTANCE_CENTER_BOOST,
    STEP_END
  };

  enum enum_direction {
    DIR_NONE,
    DIR_CENTER,
    DIR_LEFT,
    DIR_RIGHT
  };

  enum_step_fura current_step, previous_step, step_after_button;
  bool current_step_init = true;
  void goNextStep(enum_step_fura next_step);

  uint32_t rc_num_error1 = 0;
  uint32_t rc_num_error2 = 0;
  uint32_t rc_address = 0;

  uint32_t speed_left_ini = 0;
  uint32_t speed_left_end = 0;
  uint32_t speed_right_ini = 0;
  uint32_t speed_right_end = 0;
  uint32_t speed_ramp__time_tick_ini = 0;
  uint32_t speed_ramp__time_tick_current = 0;
  uint32_t speed_refresh__time_tick_ini = 0;
  uint32_t speed_refresh__time_tick_current = 0;

  uint8_t direction_base;
  uint32_t esc_speed_base_left = (*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_LEFT].value;
  uint32_t esc_speed_base_right = (*config_motor)[(size_t)enum_motor::F_M_ESC_BASE_SPEED_RIGHT].value;

  uint32_t tracker_base_left_in_detect_line = F_E_TRACKER_LEFT_IN_DETECT_LINE;
  uint32_t tracker_base_right_in_detect_line = F_E_TRACKER_RIGHT_IN_DETECT_LINE;
  uint32_t tracker_base_left_out_detect_line = F_E_TRACKER_RIGHT_IN_DETECT_LINE;
  uint32_t tracker_base_right_out_detect_line = F_E_TRACKER_RIGHT_OUT_DETECT_LINE;
  uint32_t tracker_base_iteration = 0;
  uint32_t tracker_base_lap = 0;
  std::vector<uint16_t> tracker_base_left_values;
  std::vector<uint16_t> tracker_base_right_values;
  uint32_t tracker_base_time_tick_ini = 0;
  uint32_t tracker_base_time_tick_current = 0;

  uint32_t step_button_start_pressed__time_tick_ini = 0;
  uint32_t step_button_start_pressed__time_tick_current = 0;
  uint8_t button_stop__button_previous_state = 0;

  uint32_t time_out__time_tick_ini = 0;
  uint32_t time_out__time_tick_current = 0;

  int16_t sensor_gyro_in__y_value = 0;
  uint32_t sensor_gyro_in__time_tick_ini = 0;
  uint32_t sensor_gyro_in__time_tick_current = 0;

  int16_t sensor_gyro_out__y_value = 0;
  uint32_t sensor_gyro_out__time_tick_ini = 0;
  uint32_t sensor_gyro_out__time_tick_current = 0;

  uint32_t tracker_left_in__tracker_value = 0;
  uint32_t tracker_left_in__time_tick_ini = 0;
  uint32_t tracker_left_in__time_tick_current = 0;

  uint32_t tracker_left_out__tracker_value = 0;
  uint32_t tracker_left_out__time_tick_ini = 0;
  uint32_t tracker_left_out__time_tick_current = 0;

  uint32_t tracker_right_in__tracker_value = 0;
  uint32_t tracker_right_in__time_tick_ini = 0;
  uint32_t tracker_right_in__time_tick_current = 0;

  uint32_t tracker_right_out__tracker_value = 0;
  uint32_t tracker_right_out__time_tick_ini = 0;
  uint32_t tracker_right_out__time_tick_current = 0;

  uint32_t distance_left_in__distance_value = 0;
  uint32_t distance_left_in__time_tick_ini = 0;
  uint32_t distance_left_in__time_tick_current = 0;

  uint32_t distance_left_out__distance_value = 0;
  uint32_t distance_left_out__time_tick_ini = 0;
  uint32_t distance_left_out__time_tick_current = 0;

  uint32_t distance_right_in__distance_value = 0;
  uint32_t distance_right_in__time_tick_ini = 0;
  uint32_t distance_right_in__time_tick_current = 0;

  uint32_t distance_right_out__distance_value = 0;
  uint32_t distance_right_out__time_tick_ini = 0;
  uint32_t distance_right_out__time_tick_current = 0;

  uint32_t distance_center_in__distance_value = 0;
  uint32_t distance_center_in__time_tick_ini = 0;
  uint32_t distance_center_in__time_tick_current = 0;

  uint32_t distance_center_out__distance_value = 0;
  uint32_t distance_center_out__time_tick_ini = 0;
  uint32_t distance_center_out__time_tick_current = 0;

  void detectButtonStop();
  void detectTimeOut(enum_step_fura next_step, uint32_t time_out);
  void detectSensorGyroUp(enum_step_fura next_step);
  void detectSensorTiltingOut(enum_step_fura next_step, uint32_t time_out_of_line);
  void detectTrakerLeftIn(enum_step_fura next_step);
  void detectTrakerLeftOut(enum_step_fura next_step, uint32_t time_out_of_line);
  void detectTrakerRightIn(enum_step_fura next_step);
  void detectTrakerRightOut(enum_step_fura next_step, uint32_t time_out_of_line);
  void detectDistanceLeftIn(enum_step_fura next_step);
  void detectDistanceLeftOut(enum_step_fura next_step, uint32_t time_out_detect);
  void detectDistanceRightIn(enum_step_fura next_step);
  void detectDistanceRightOut(enum_step_fura next_step, uint32_t time_out_detect);
  void detectDistanceCenterIn(enum_step_fura next_step);
  void detectDistanceCenterOut(enum_step_fura next_step, uint32_t time_out_detect);

  void setMotorSpeed(uint32_t speed_left, uint32_t speed_right);
  void setMotorSpeedRamp(uint32_t speed_left_ini, uint32_t speed_left_end, uint32_t speed_right_ini, uint32_t speed_right_end);
  void rampMotorSpeed(uint32_t speed_ramp_time_increment_ms, uint32_t speed_ramp_value_increment);

  void setDirectionCenter();
  void setDirectionLeft();   // deprecated
  void setDirectionRight();  // deprecated

  void writeDataFlash(uint32_t tracker_base_left_in_detect_line_value,
                      uint32_t tracker_base_right_in_detect_line_value,
                      uint32_t tracker_base_left_out_detect_line_value,
                      uint32_t tracker_base_right_out_detect_line_value,
                      uint32_t rc_address_value,
                      uint32_t furafoscan_type_value);
};

#endif  // FURAE_HPP
