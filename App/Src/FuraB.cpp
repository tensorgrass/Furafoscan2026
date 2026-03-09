#include "FuraB.hpp"

FuraB::FuraB(ControllerBase* controllerBaseValue) : controller(controllerBaseValue) {
  // Constructor de FuraB, inicializa el controlador y los objetos UART
}

// prueba boton 3 segundos para arrancar
void FuraB::main() {
  if (controller->getButtonStart()->isButtonPressed()) {
    if (button_previous_state == 0) {
      controller->getLedStart()->turn_off();
      time_tick_ini = HAL_GetTick(); // Guarda el tiempo actual
      button_previous_state = 1;
    }
    else {
      time_tick_current = HAL_GetTick() - time_tick_ini;
      if (time_tick_current < BUTTON_START_WAIT) {
        //Esperamos a que pasen los 3 segundos
      }
      else {
        if (controller->getLedStart()->get_status() == 0) {
          controller->getLedStart()->turn_on();
        }
        else {
          //Ya hemos llegado a los 3 segundo y no hace falta volver a encender el led
        }
      }
    }
  }
  else {
    if (button_previous_state == 1) {
      button_previous_state = 0;
    }

    if ((HAL_GetTick() / 100) % 2 == 0) {
      controller->getLedStart()->turn_on();
    }
    else {
      controller->getLedStart()->turn_off();
    }
  }
}
