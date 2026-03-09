#include <TestC.hpp>
#include <step_utils.hpp>
#include <MotorOneShot125.hpp>
TestC::TestC(ControllerBase* controllerBaseValue) : TestBase(controllerBaseValue) {
  // Constructor de TestC, inicializa el controlador y los objetos UART
}

void TestC::sendData() {
  int subgrupo = controller->getRxSubgroup() - '0';  // Convertir el carácter a entero
  switch (subgrupo) {
    case enum_subgroup::SUBGRUPO_0:
      // Código para SUBGRUPO_1
      break;
    case enum_subgroup::SUBGRUPO_1:
      sendPressButtonStart();
      break;
    case enum_subgroup::SUBGRUPO_2:
      sendPressButtonStart2();
      break;

    default:
      // Código para caso por defecto
      break;
  }
}

void TestC::sendPressButtonStart() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      step_wait_init(controller);
      break;
    case enum_step::STEP_INIT:
      step_init__ini(controller);
      step_init__end(controller, enum_step::STEP_BUTTON_START, enum_step::STEP_BUTTON_START);
      break;
    case enum_step::STEP_WAIT_BUTTON_START:
      step_wait(controller);
      first_cicle_button_start = 0;
      break;
    case enum_step::STEP_BUTTON_START:
      if (first_cicle_button_start == 0) {
        step_button_start__ini(controller);
        first_cicle_button_start = 1;
      }
      if (controller->getButtonStart()->getEstado() == 1) {
        step_button_start__end(controller, enum_step::STEP_FINISH, enum_step::STEP_FINISH);
      }
      break;
    case enum_step::STEP_WAIT_FINISH:
      step_wait_finish(controller);
      break;
    case enum_step::STEP_FINISH:
      step_finish__ini(controller);
      step_finish__end(controller);
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestC::sendPressButtonStart2() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      step_wait_init(controller);
      break;
    case enum_step::STEP_INIT:
      step_init__ini(controller);
      step_init__end(controller, enum_step::STEP_BUTTON_START, enum_step::STEP_BUTTON_START);
      break;
    case enum_step::STEP_WAIT_BUTTON_START:
      step_wait(controller);
      first_cicle_button_start = 0;
      previus_state_button = 0;
      num_ms_button = 0;
      break;
    case enum_step::STEP_BUTTON_START:
      if (first_cicle_button_start == 0) {
        step_button_start__ini(controller);
        first_cicle_button_start = 1;
      }

      if (controller->getButtonStart()->isButtonPressed()) {
        if (previus_state_button == 0) {
          previus_state_button = 1;
          num_ms_button = 0;
          ini_time = HAL_GetTick(); // Guarda el instante de inicio
          HAL_Delay(1);
        }
        else {
          num_ms_button = HAL_GetTick() - ini_time;
          controller->getUartSerial()->bufferln(std::to_string(num_ms_button));
          if (num_ms_button >= 3000) {
            step_button_start__end(controller, enum_step::STEP_FINISH, enum_step::STEP_FINISH);
          }
          else {
            if (num_ms_button < 3000 - 2) {
              HAL_Delay(1);
            }
            else {
              HAL_Delay(1);
            }
          }
        }

      }
      else {
        HAL_Delay(1);
      }

      break;
    case enum_step::STEP_WAIT_FINISH:
      step_wait_finish(controller);
      break;
    case enum_step::STEP_FINISH:
      step_finish__ini(controller);
      step_finish__end(controller);
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}



void TestC::receiveData() {
}
