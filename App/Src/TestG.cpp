#include "TestG.hpp"

TestG::TestG(ControllerBase* controllerBaseValue) : TestBase(controllerBaseValue) {
  // Constructor de TestG, inicializa el controlador y los objetos UART
}

void TestG::sendData() {
  int subgrupo = controller->getRxSubgroup() - '0';  // Convertir el carácter a entero
  switch (subgrupo) {
    case enum_subgroup::SUBGRUPO_0:
      // Código para SUBGRUPO_1
      break;
    case enum_subgroup::SUBGRUPO_1:
      sendTrackerLeft();
      break;
    case enum_subgroup::SUBGRUPO_2:
      sendTrackerRight();
      break;
    case enum_subgroup::SUBGRUPO_3:
      sendTrackerMultipleLeft();
      break;
    case enum_subgroup::SUBGRUPO_4:
      sendTrackerMultipleRight();
      break;
    // Agrega más casos según los valores de enum_subgroup
    default:
      // Código para caso por defecto
      break;
  }
}

void TestG::sendTrackerLeft() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      controller->setRxStep(enum_step::STEP_INIT);
      break;
    case enum_step::STEP_INIT:
      controller->setTxGroup(controller->getRxGroup());
      controller->setTxSubgroup(controller->getRxSubgroup());
      controller->setTxStep(enum_step::STEP_FINISH); // Cambia el paso para evitar reentradas
      controller->setRxStep(enum_step::STEP_WAIT_FINISH);
      uartSerial->bufferln("sendTrackerLeft");
      for (int i = 0; i < 5; i++) {
        uartSerial->bufferln(std::to_string(controller->getDistanceLeft()->getValue()));
      }

      controller->commSendData();

      break;
    case enum_step::STEP_WAIT_FINISH:
      controller->setRxStep(enum_step::STEP_FINISH);
      break;
    case enum_step::STEP_FINISH:
      uartSerial->printBuffer();
      controller->endRxTransaction(); // Finaliza la transacción de envío
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestG::sendTrackerRight() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      controller->setRxStep(enum_step::STEP_INIT);
      break;
    case enum_step::STEP_INIT:
      controller->setTxGroup(controller->getRxGroup());
      controller->setTxSubgroup(controller->getRxSubgroup());
      controller->setTxStep(enum_step::STEP_FINISH); // Cambia el paso para evitar reentradas
      controller->setRxStep(enum_step::STEP_WAIT_FINISH);
      uartSerial->bufferln("sendTrackerRight");
      for (int i = 0; i < 5; i++) {
        uartSerial->bufferln(std::to_string(controller->getDistanceRight()->getValue()));
      }
      controller->commSendData();
      break;
    case enum_step::STEP_WAIT_FINISH:
      controller->setRxStep(enum_step::STEP_FINISH);
      break;
    case enum_step::STEP_FINISH:
      uartSerial->printBuffer();
      controller->endRxTransaction(); // Finaliza la transacción de envío
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestG::sendTrackerMultipleLeft() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      controller->setRxStep(enum_step::STEP_INIT);
      break;
    case enum_step::STEP_INIT:
      controller->setTxGroup(controller->getRxGroup());
      controller->setTxSubgroup(controller->getRxSubgroup());
      controller->setTxStep(enum_step::STEP_FINISH); // Cambia el paso para evitar reentradas
      controller->setRxStep(enum_step::STEP_WAIT_FINISH);
      uartSerial->bufferln("sendTrackerLeft");
      uartSerial->bufferln(std::to_string(controller->getDistanceLeft()->getValue()));
      controller->commSendData();
      break;
    case enum_step::STEP_WAIT_FINISH:
      controller->setRxStep(enum_step::STEP_FINISH);
      break;
    case enum_step::STEP_FINISH:
      uartSerial->printBuffer();
      controller->endRxTransaction(); // Finaliza la transacción de envío
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestG::sendTrackerMultipleRight() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      controller->setRxStep(enum_step::STEP_INIT);
      break;
    case enum_step::STEP_INIT:
      controller->setTxGroup(controller->getRxGroup());
      controller->setTxSubgroup(controller->getRxSubgroup());
      controller->setTxStep(enum_step::STEP_FINISH); // Cambia el paso para evitar reentradas
      controller->setRxStep(enum_step::STEP_WAIT_FINISH);
      uartSerial->bufferln("sendTrackerLeft");
      uartSerial->bufferln(std::to_string(controller->getDistanceRight()->getValue()));
      controller->commSendData();
      break;
    case enum_step::STEP_WAIT_FINISH:
      controller->setRxStep(enum_step::STEP_FINISH);
      break;
    case enum_step::STEP_FINISH:
      uartSerial->printBuffer();
      controller->endRxTransaction(); // Finaliza la transacción de envío
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestG::receiveData() {
}
