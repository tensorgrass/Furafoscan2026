#include "TestA.hpp"

TestA::TestA(ControllerBase* controllerBaseValue) : TestBase(controllerBaseValue) {
  // Constructor de TestA, inicializa el controlador y los objetos UART
}

void TestA::sendData() {
  int subgrupo = controller->getRxSubgroup() - '0';  // Convertir el carácter a entero
  switch (subgrupo) {
    case enum_subgroup::SUBGRUPO_0:
      // Código para SUBGRUPO_1
      break;
    case enum_subgroup::SUBGRUPO_1:
      sendOneMessage();
      break;
    case enum_subgroup::SUBGRUPO_2:
      sendMultipleMessages();
      break;
    case enum_subgroup::SUBGRUPO_3:
      // Código para SUBGRUPO_3
      break;
    // Agrega más casos según los valores de enum_subgroup
    default:
      // Código para caso por defecto
      break;
  }
}

void TestA::sendOneMessage() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      controller->setRxStep(enum_step::STEP_INIT);
      break;
    case enum_step::STEP_INIT:
      controller->setTxGroup(controller->getRxGroup());
      controller->setTxSubgroup(controller->getRxSubgroup());
      controller->setTxStep(enum_step::STEP_FINISH); // Cambia el paso para evitar reentradas
      uartSerial->buffer("Slave Eco: Step 2\r\n");
//        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

      controller->commSendData();
      controller->endRxTransaction(); // Finaliza la transacción de envío
      uartSerial->printBuffer();
      break;
    case enum_step::STEP_WAIT_FINISH:
      break;
    case enum_step::STEP_FINISH:
      break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestA::sendMultipleMessages() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      controller->setRxStep(enum_step::STEP_INIT);
      break;
    case enum_step::STEP_INIT:
      controller->setTxGroup(controller->getRxGroup());
      controller->setTxSubgroup(controller->getRxSubgroup());
      controller->setTxStep(enum_step::STEP_FINISH); // Cambia el paso para evitar reentradas
      uartSerial->buffer("Slave Eco: Step 2\r\n");
//        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

      controller->commSendData();
      controller->endRxTransaction(); // Finaliza la transacción de envío
//      uartSerial->printBuffer();
      break;
    case enum_step::STEP_WAIT_FINISH:
      break;
    case enum_step::STEP_FINISH:
      break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestA::receiveData() {
}
