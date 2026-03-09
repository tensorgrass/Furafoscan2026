#include "TestE.hpp"

TestE::TestE(ControllerBase* controllerBaseValue) : TestBase(controllerBaseValue) {
  // Constructor de TestE, inicializa el controlador y los objetos UART
}

void TestE::sendData() {
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

void TestE::sendTrackerLeft() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      controller->setRxStep(enum_step::STEP_INIT);
      break;
    case enum_step::STEP_INIT:
      uartSerial->buffer("Fura: Step STEP_INIT\r\n");
      controller->setRxStep(enum_step::STEP_WAIT_TRACKER_LEFT_START);

      controller->setTxGroup(controller->getRxGroup());
      controller->setTxSubgroup(controller->getRxSubgroup());
      controller->setTxStep(enum_step::STEP_TRACKER_LEFT_START); // Cambia el paso para evitar reentradas
      uartSerial->bufferln("sendTrackerLeft");
      controller->commSendData();
      break;
    case enum_step::STEP_WAIT_TRACKER_LEFT_START:
      HAL_Delay(1);
      break;
    case enum_step::STEP_TRACKER_LEFT_START:
      uartSerial->buffer("Fura: Step STEP_TRACKER_LEFT_START\r\n");
      controller->setRxStep(enum_step::STEP_WAIT_FINISH);
      uartSerial->bufferln("sendTrackerLeft");
      for (int i = 0; i < 500; i++) {
        uartSerial->bufferln(std::to_string(controller->getTrackerLeft()->getValue()));
        HAL_Delay(10);
      }

      controller->setTxStep(enum_step::STEP_TRACKER_LEFT_DETECTED); // Cambia el paso para evitar reentradas
      controller->commSendData();
      break;
    case enum_step::STEP_WAIT_FINISH:
      HAL_Delay(1);
      break;
    case enum_step::STEP_FINISH:
      uartSerial->buffer("Fura: Step STEP_FINISH\r\n");
      uartSerial->printBuffer();
      controller->endRxTransaction(); // Finaliza la transacción de envío
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestE::sendTrackerRight() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      controller->setRxStep(enum_step::STEP_INIT);
      break;
    case enum_step::STEP_INIT:
      uartSerial->buffer("Fura: Step STEP_INIT\r\n");
      controller->setRxStep(enum_step::STEP_WAIT_TRACKER_RIGHT_START);

      controller->setTxGroup(controller->getRxGroup());
      controller->setTxSubgroup(controller->getRxSubgroup());
      controller->setTxStep(enum_step::STEP_TRACKER_RIGHT_START); // Cambia el paso para evitar reentradas
      uartSerial->bufferln("sendTrackerRight");
      controller->commSendData();
      break;
    case enum_step::STEP_WAIT_TRACKER_RIGHT_START:
      HAL_Delay(1);
      break;
    case enum_step::STEP_TRACKER_RIGHT_START:
      uartSerial->buffer("Fura: Step STEP_TRACKER_RIGHT_START\r\n");
      controller->setRxStep(enum_step::STEP_WAIT_FINISH);
      uartSerial->bufferln("sendTrackerRight");
      for (int i = 0; i < 500; i++) {
        uartSerial->bufferln(std::to_string(controller->getTrackerRight()->getValue()));
        HAL_Delay(10);
      }

      controller->setTxStep(enum_step::STEP_TRACKER_RIGHT_DETECTED); // Cambia el paso para evitar reentradas
      controller->commSendData();
      break;
    case enum_step::STEP_WAIT_FINISH:
      HAL_Delay(1);
      break;
    case enum_step::STEP_FINISH:
      uartSerial->buffer("Fura: Step STEP_FINISH\r\n");
      uartSerial->printBuffer();
      controller->endRxTransaction(); // Finaliza la transacción de envío
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestE::sendTrackerMultipleLeft() {
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
      uartSerial->bufferln(std::to_string(controller->getTrackerLeft()->getValue()));
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

void TestE::sendTrackerMultipleRight() {
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
      uartSerial->bufferln(std::to_string(controller->getTrackerRight()->getValue()));
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

void TestE::receiveData() {
}
