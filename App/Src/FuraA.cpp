#include "FuraA.hpp"

FuraA::FuraA(ControllerBase* controllerBaseValue) : controller(controllerBaseValue) {
  // Constructor de FuraA, inicializa el controlador y los objetos UART
}

// prueba simple led intermitente
void FuraA::main() {
  controller->getLedStart()->turn_on();
  HAL_Delay(100);
  controller->getLedStart()->turn_off();
  HAL_Delay(100);
}
