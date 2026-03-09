#include "FuraC.hpp"

FuraC::FuraC(ControllerBase* controllerBaseValue) : controller(controllerBaseValue) {
  // Constructor de FuraC, inicializa el controlador y los objetos UART
  current_step = enum_step_fura::STEP_WAIT_BUTTON_START_PRESSED;
}

// prueba boton 5 segundos para arrancar y arrancar el motor
void FuraC::main() {
  switch (current_step) {
    case enum_step_fura::STEP_WAIT_BUTTON_START_PRESSED:
      if (controller->getButtonStart()->isButtonPressed()) {
        HAL_Delay(1);
        //volvemos a leer el valor del motor para descartar rebotes
        if (controller->getButtonStart()->isButtonPressed()) {
          if (step_wait_button_start_pressed__button_previous_state == 0) {
            //Se ha pulsado el boton, apagamos el led y pasamos al siguiente estado y empezamos a calcular el timempo del boton pulsado
            controller->getLedStart()->turn_off();
            step_wait_button_start_pressed__button_previous_state = 1;
            current_step = enum_step_fura::STEP_BUTTON_START_PRESSED;
            step_button_start_pressed__time_tick_ini = HAL_GetTick(); // Guarda el tiempo actual
            controller->getMotorLeft()->setSpeed(ESC_MID_STOP);
            controller->getMotorRight()->setSpeed(ESC_MID_STOP);
          }
          else {
            //Aun esta pulsado el boton del estado anterior, se tiene que esperar a que lo suelte
          }
        }
        else {
          //rebote detectado
        }
      }
      else {
        //esperar a que se pulse el boton, se deja parpadeando el led
        if (step_wait_button_start_pressed__button_previous_state == 1) {
          step_wait_button_start_pressed__button_previous_state = 0;
        }
        //Parpadea led, segun si es par o impar cada 100ms
        if ((HAL_GetTick() / 100) % 2 == 0) {
          controller->getLedStart()->turn_on();
        }
        else {
          controller->getLedStart()->turn_off();
        }
      }
      break;
    case enum_step_fura::STEP_BUTTON_START_PRESSED:
      if (controller->getButtonStart()->isButtonPressed()) {
        step_button_start_pressed__time_tick_current = HAL_GetTick() - step_button_start_pressed__time_tick_ini;
        if (step_button_start_pressed__time_tick_current < BUTTON_START_WAIT) {
          //Esperamos a que pasen los 3 segundos
        }
        else {
          if (controller->getLedStart()->get_status() == 0) {
            controller->getLedStart()->turn_on();
            current_step = enum_step_fura::STEP_MOTOR_START;
            step_motor_start__button_previous_state = 1;
            controller->getMotorLeft()->setSpeed(ESC_TEST_SPEED);
            controller->getMotorRight()->setSpeed(ESC_TEST_SPEED);
          }
          else {
            //Ya hemos llegado a los 5 segundos y no hace falta volver a encender el led
          }
        }
      }
      else {
        //Se ha dejado de pulsar el boton antes de tiempo, restauramos valores iniciales
        current_step = enum_step_fura::STEP_WAIT_BUTTON_START_PRESSED;
        step_wait_button_start_pressed__button_previous_state = 0;
        controller->getMotorLeft()->disable();
        controller->getMotorRight()->disable();
      }
      break;
    case enum_step_fura::STEP_MOTOR_START:
      if (controller->getButtonStart()->isButtonPressed()) {
        HAL_Delay(1);
        //volvemos a leer el valor del motor para descartar rebotes
        if (controller->getButtonStart()->isButtonPressed()) {
          //en el caso que se haya soltado, y se vuelva a pulsar, se tiene que parar todo y volver al estado inicial
          if (step_motor_start__button_previous_state == 0) {
            //Se ha pulsado el boton y se tiene que parar todo
            controller->getMotorLeft()->disable();
            controller->getMotorRight()->disable();
            current_step = enum_step_fura::STEP_WAIT_BUTTON_START_PRESSED;
            step_wait_button_start_pressed__button_previous_state = 1;
          }
          else {
            //aun está pulsado el boton del estado anterior, se tiene que esperar a que lo suelte
          }
        }
        else {
          //Detectado rebote
        }
      }
      else {
        //en el caso que se suelte el boton, se tiene que actualizar el estado de boton anterior
        if (step_motor_start__button_previous_state == 1) {
          step_motor_start__button_previous_state = 0;
        }
      }
      break;
    default:
      // Código para caso por defecto
      break;
  }


}
