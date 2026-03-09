#ifndef FURAC_HPP
#define FURAC_HPP

#define BUTTON_START_WAIT 3000

#define ESC_MID_STOP 1920//1830-2000
#define ESC_TEST_SPEED 2050

#include <ControllerBase.hpp>

class FuraC  {
   public:
    FuraC(ControllerBase* controllerBaseValue);

    void main();
   private:
    ControllerBase* controller; // Puntero al controlador base


    uint8_t step_wait_button_start_pressed__button_previous_state = 0;

    uint32_t step_button_start_pressed__time_tick_ini = 0;
    uint32_t step_button_start_pressed__time_tick_current = 0;

    uint8_t step_motor_start__button_previous_state = 0;



    enum enum_step_fura {
      STEP_WAIT_BUTTON_START_PRESSED,
      STEP_BUTTON_START_PRESSED,
      STEP_MOTOR_START,
      STEP_END
    };
    enum_step_fura current_step;
};

#endif  // FURAC_HPP
