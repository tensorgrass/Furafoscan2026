#include "TestH.hpp"
#include <MotorOneShot125.hpp>

TestH::TestH(ControllerBase* controllerBaseValue) : TestBase(controllerBaseValue) {
  // Constructor de TestH, inicializa el controlador y los objetos UART
}

void TestH::sendData() {
  int subgrupo = controller->getRxSubgroup() - '0';  // Convertir el carácter a entero
  switch (subgrupo) {
    case enum_subgroup::SUBGRUPO_0:
      // Código para SUBGRUPO_1
      break;
    case enum_subgroup::SUBGRUPO_1:
      sendMotorLeft();
      break;
    case enum_subgroup::SUBGRUPO_2:
      sendMotorRight();
      break;
    case enum_subgroup::SUBGRUPO_3:
      sendMotorEncoderLeft();
      break;
    case enum_subgroup::SUBGRUPO_4:
      sendMotorEncoderRight();
      break;
    // Agrega más casos según los valores de enum_subgroup
    default:
      // Código para caso por defecto
      break;
  }
}

void TestH::sendMotorLeft() {
  uint8_t motor_dir = 0;
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

      controller->commSendData();

      motor_value = ESC_MID;
      controller->getMotorOneshot125Left()->setSpeed(motor_value);
      uartSerial->bufferln(std::to_string(motor_value));

      HAL_Delay(3000); // Espera 100 milisegundos

      while(1) {
        if (motor_dir == 0) {
          motor_value += ESC_INC;
          if (motor_value < ESC_FORWARD && motor_value > ESC_BACKWARD) {
            motor_value = ESC_FORWARD;
          }
          if (motor_value >= ESC_FORWARD_MAX) {
             motor_dir = 1; // Cambia la dirección a reversa
          }
        }
        else if (motor_dir == 1) {
          motor_value -= ESC_INC;
          if (motor_value < ESC_FORWARD && motor_value > ESC_BACKWARD) {
            motor_value = ESC_BACKWARD;
          }
          if (motor_value <= ESC_BACKWARD_MIN) {
             motor_dir = 2; // Cambia la dirección a avance
          }
        }
        else {
          motor_value += ESC_INC;
          if (motor_value < ESC_FORWARD && motor_value > ESC_BACKWARD) {
//            motor_value = ESC_FORWARD;
            motor_value = ESC_MID;
            uartSerial->bufferln(std::to_string(motor_value));
            controller->getMotorOneshot125Left()->setSpeed(motor_value);
            break;
          }
        }
        controller->getMotorOneshot125Left()->setSpeed(motor_value);
        uartSerial->bufferln(std::to_string(motor_value));

        HAL_Delay(500); // Espera 100 milisegundos
      }

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

void TestH::sendMotorRight() {
  uint8_t motor_dir = 0;
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

      controller->commSendData();

      motor_value = ESC_MID;
      controller->getMotorOneshot125Right()->setSpeed(motor_value);
      uartSerial->bufferln(std::to_string(motor_value));

      HAL_Delay(3000); // Espera 100 milisegundos

      while(1) {
        if (motor_dir == 0) {
          motor_value += ESC_INC;
          if (motor_value < ESC_FORWARD && motor_value > ESC_BACKWARD) {
            motor_value = ESC_FORWARD;
          }
          if (motor_value >= ESC_FORWARD_MAX) {
             motor_dir = 1; // Cambia la dirección a reversa
          }
        }
        else if (motor_dir == 1) {
          motor_value -= ESC_INC;
          if (motor_value < ESC_FORWARD && motor_value > ESC_BACKWARD) {
            motor_value = ESC_BACKWARD;
          }
          if (motor_value <= ESC_BACKWARD_MIN) {
             motor_dir = 2; // Cambia la dirección a avance
          }
        }
        else {
          motor_value += ESC_INC;
          if (motor_value < ESC_FORWARD && motor_value > ESC_BACKWARD) {
//            motor_value = ESC_FORWARD;
            motor_value = ESC_MID;
            uartSerial->bufferln(std::to_string(motor_value));
            controller->getMotorOneshot125Right()->setSpeed(motor_value);
            break;
          }
        }
        controller->getMotorOneshot125Right()->setSpeed(motor_value);
        uartSerial->bufferln(std::to_string(motor_value));

        HAL_Delay(500); // Espera 100 milisegundos
      }

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

void TestH::sendMotorEncoderLeft() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      controller->setRxStep(enum_step::STEP_INIT);
      break;
    case enum_step::STEP_INIT:
      uartSerial->bufferln("Fura: STEP_INIT");
      controller->setRxStep(enum_step::STEP_WAIT_ACCELERATE);

      motor_value = ESC_MID;
      controller->getMotorOneshot125Left()->setSpeed(motor_value);
      uartSerial->bufferln(std::to_string(motor_value));

      HAL_Delay(3000); // Espera 100 milisegundos

      controller->setTxGroup(controller->getRxGroup());
      controller->setTxSubgroup(controller->getRxSubgroup());
      controller->setTxStep(enum_step::STEP_ACCELERATE); // Cambia el paso para evitar reentradas
      controller->commSendData();

      break;
    case enum_step::STEP_WAIT_ACCELERATE:
      timer11Delay->delay(1);
      break;
    case enum_step::STEP_ACCELERATE:
      uartSerial->buffer("Fura: STEP_ACCELERATE\r\n");
      controller->setRxStep(enum_step::STEP_WAIT_DECELERATE);

      while(1) {
        motor_value += ESC_INC;
        if (motor_value < ESC_FORWARD && motor_value > ESC_BACKWARD) {
          motor_value = ESC_FORWARD;
        }
        if (motor_value >= ESC_FORWARD_MAX) {
//           motor_dir = 1; // Cambia la dirección a reversa
          break;
        }

        controller->getMotorOneshot125Left()->setSpeed(motor_value);
        uartSerial->bufferln(std::to_string(motor_value));

        HAL_Delay(100); // Espera 100 milisegundos
      }

      controller->setTxStep(enum_step::STEP_DECELERATE); // Cambia el paso para evitar reentradas
      controller->commSendData();

      break;
    case enum_step::STEP_WAIT_DECELERATE:
      timer11Delay->delay(1);
      break;
    case enum_step::STEP_DECELERATE:
      uartSerial->buffer("Fura: STEP_DECELERATE\r\n");
      controller->setRxStep(enum_step::STEP_FINISH);

      while(1) {
        motor_value -= ESC_INC;
        if (motor_value < ESC_FORWARD && motor_value > ESC_BACKWARD) {
          motor_value = ESC_MID;
        }

        controller->getMotorOneshot125Left()->setSpeed(motor_value);
        uartSerial->bufferln(std::to_string(motor_value));

        if (motor_value < ESC_FORWARD && motor_value > ESC_BACKWARD) {
          uartSerial->bufferln("Fura: MOTOR STOP");
          break;
        }

        HAL_Delay(100); // Espera 100 milisegundos
      }

      controller->setTxStep(enum_step::STEP_FINISH); // Cambia el paso para evitar reentradas
      controller->commSendData();

      break;
    case enum_step::STEP_WAIT_FINISH:
      controller->setRxStep(enum_step::STEP_FINISH);
      break;
    case enum_step::STEP_FINISH:
      uartSerial->bufferln("Fura: STEP_FINISH");
      uartSerial->printBuffer();
      controller->endRxTransaction(); // Finaliza la transacción de envío
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestH::sendMotorEncoderRight() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      controller->setRxStep(enum_step::STEP_INIT);
      break;
    case enum_step::STEP_INIT:
      uartSerial->bufferln("Fura: STEP_INIT");
      controller->setRxStep(enum_step::STEP_WAIT_ACCELERATE);

      motor_value = ESC_MID;
      controller->getMotorOneshot125Right()->setSpeed(motor_value);
      uartSerial->bufferln(std::to_string(motor_value));

      HAL_Delay(3000); // Espera 100 milisegundos

      controller->setTxGroup(controller->getRxGroup());
      controller->setTxSubgroup(controller->getRxSubgroup());
      controller->setTxStep(enum_step::STEP_ACCELERATE); // Cambia el paso para evitar reentradas
      controller->commSendData();

      break;
    case enum_step::STEP_WAIT_ACCELERATE:
      timer11Delay->delay(1);
      break;
    case enum_step::STEP_ACCELERATE:
      uartSerial->buffer("Fura: STEP_ACCELERATE\r\n");
      controller->setRxStep(enum_step::STEP_WAIT_DECELERATE);

      while(1) {
        motor_value += ESC_INC;
        if (motor_value < ESC_FORWARD && motor_value > ESC_BACKWARD) {
          motor_value = ESC_FORWARD;
        }
        if (motor_value >= ESC_FORWARD_MAX) {
//           motor_dir = 1; // Cambia la dirección a reversa
          break;
        }

        controller->getMotorOneshot125Right()->setSpeed(motor_value);
        uartSerial->bufferln(std::to_string(motor_value));

        HAL_Delay(100); // Espera 100 milisegundos
      }

      controller->setTxStep(enum_step::STEP_DECELERATE); // Cambia el paso para evitar reentradas
      controller->commSendData();

      break;
    case enum_step::STEP_WAIT_DECELERATE:
      timer11Delay->delay(1);
      break;
    case enum_step::STEP_DECELERATE:
      uartSerial->buffer("Fura: STEP_DECELERATE\r\n");
      controller->setRxStep(enum_step::STEP_FINISH);

      while(1) {
        motor_value -= ESC_INC;
        if (motor_value < ESC_FORWARD && motor_value > ESC_BACKWARD) {
          motor_value = ESC_MID;
        }

        controller->getMotorOneshot125Right()->setSpeed(motor_value);
        uartSerial->bufferln(std::to_string(motor_value));

        if (motor_value < ESC_FORWARD && motor_value > ESC_BACKWARD) {
          uartSerial->bufferln("Fura: MOTOR STOP");
          break;
        }

        HAL_Delay(100); // Espera 100 milisegundos
      }

      controller->setTxStep(enum_step::STEP_FINISH); // Cambia el paso para evitar reentradas
      controller->commSendData();

      break;
    case enum_step::STEP_WAIT_FINISH:
      controller->setRxStep(enum_step::STEP_FINISH);
      break;
    case enum_step::STEP_FINISH:
      uartSerial->bufferln("Fura: STEP_FINISH");
      uartSerial->printBuffer();
      controller->endRxTransaction(); // Finaliza la transacción de envío
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestH::receiveData() {
}
