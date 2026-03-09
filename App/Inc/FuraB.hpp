#ifndef FURAB_HPP
#define FURAB_HPP

#define BUTTON_START_WAIT 3000

#include <ControllerBase.hpp>

class FuraB  {
   public:
    FuraB(ControllerBase* controllerBaseValue);

    void main();
   private:
    ControllerBase* controller; // Puntero al controlador base

    uint32_t num_ms = 0;
    uint32_t time_tick_ini = 0;
    uint32_t time_tick_current = 0;
    uint8_t button_previous_state = 0;
    uint8_t led_status = 0;
};

#endif  // FURAB_HPP
