#ifndef FURAA_HPP
#define FURAA_HPP

#include <ControllerBase.hpp>

class FuraA  {
   public:
    FuraA(ControllerBase* controllerBaseValue);

    void main();
   private:
    ControllerBase* controller; // Puntero al controlador base

    volatile uint32_t num_ms = 0;
    volatile uint32_t time_tick_ini = 0;
};

#endif  // FURAA_HPP
