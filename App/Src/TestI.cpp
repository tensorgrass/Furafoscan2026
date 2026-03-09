#include <TestI.hpp>
#include <step_utils.hpp>
#include <MotorOneShot125.hpp>
TestI::TestI(ControllerBase* controllerBaseValue) : TestBase(controllerBaseValue) {
  // Constructor de TestI, inicializa el controlador y los objetos UART
}

void TestI::sendData() {
  int subgrupo = controller->getRxSubgroup() - '0';  // Convertir el carácter a entero
  switch (subgrupo) {
    case enum_subgroup::SUBGRUPO_0:
      // Código para SUBGRUPO_1
      break;
    case enum_subgroup::SUBGRUPO_1:
      sendMotorEncoder();
      break;
    case enum_subgroup::SUBGRUPO_2:
      sendMotorEncoderTracker();
      break;
    case enum_subgroup::SUBGRUPO_3:
      sendMotorEncoderTrackerDistance();
      break;
    case enum_subgroup::SUBGRUPO_4:
      sendMotorEncoderTrackerDistanceBySpeed();
      break;
    case enum_subgroup::SUBGRUPO_5:
      sendMotorEncoderTrackerLeft();
      break;
    case enum_subgroup::SUBGRUPO_6:
      sendMotorEncoderTrackerRight();
      break;
    case enum_subgroup::SUBGRUPO_7:
      sendMotorEncoderDistanceLeft();
      break;
    case enum_subgroup::SUBGRUPO_8:
      sendMotorEncoderDistanceRight();
      break;
      // Agrega más casos según los valores de enum_subgroup
    default:
      // Código para caso por defecto
      break;
  }
}

void TestI::sendMotorEncoder() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      step_wait_init(controller);
      break;
    case enum_step::STEP_INIT:
      step_init__ini(controller);
      step_init__motor(controller, &motor_value);
      step_init__end(controller, enum_step::STEP_WAIT_ACCELERATE, enum_step::STEP_ACCELERATE);
      break;
    case enum_step::STEP_WAIT_ACCELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_ACCELERATE:
      step_accelerate__ini(controller);
      step_accelerate__motor(controller, &motor_value, ESC_FORWARD_MAX, 100);
      step_accelerate__end(controller, enum_step::STEP_WAIT_DECELERATE, enum_step::STEP_DECELERATE);
      break;
    case enum_step::STEP_WAIT_DECELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_DECELERATE:
      step_decelerate__ini(controller);
      step_decelerate__motor(controller, &motor_value, ESC_MID, 100);
      step_decelerate__end(controller, enum_step::STEP_FINISH, enum_step::STEP_FINISH);
      break;
    case enum_step::STEP_WAIT_FINISH:
      step_wait_finish(controller);
      break;
    case enum_step::STEP_FINISH:
      step_finish__ini(controller);
      step_finish__motor(controller);
      step_finish__end(controller);
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestI::sendMotorEncoderTracker() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      step_wait_init(controller);
      break;
    case enum_step::STEP_INIT:
      step_init__ini(controller);
      step_init__motor(controller, &motor_value);
      step_init__end(controller, enum_step::STEP_WAIT_ACCELERATE, enum_step::STEP_ACCELERATE);
      break;
    case enum_step::STEP_WAIT_ACCELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_ACCELERATE:
      step_accelerate__ini(controller);
      step_accelerate__motor(controller, &motor_value, ESC_FORWARD_MAX, 100);
      step_accelerate__end(controller, enum_step::STEP_WAIT_TRACKER_BOTH_START, enum_step::STEP_TRACKER_BOTH_START);
      break;

    case enum_step::STEP_WAIT_TRACKER_BOTH_START:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_START:
      step_tracker_both_start__ini(controller);
      first_cicle_trakcer_detected = 0;
      step_tracker_both_start__end(controller, enum_step::STEP_TRACKER_BOTH_DETECTED, enum_step::STEP_TRACKER_BOTH_DETECTED);
      break;
    case enum_step::STEP_WAIT_TRACKER_BOTH_DETECTED:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_DETECTED:
      if (first_cicle_trakcer_detected == 0) {
        step_tracker_both_detected__ini(controller);
        first_cicle_trakcer_detected = 1;
      }
      tracker_left_value = controller->getTrackerLeft()->getValue();
      controller->getUartSerial()->buffer(std::to_string(tracker_left_value));
      controller->getUartSerial()->buffer(" ");

      tracker_right_value = controller->getTrackerRight()->getValue();
      controller->getUartSerial()->bufferln(std::to_string(tracker_right_value));


      if (tracker_left_value > 3000 || tracker_right_value > 3000) {
        HAL_Delay(1);
      }
      else {
        step_tracker_both_detected__end(controller, enum_step::STEP_WAIT_TRACKER_BOTH_RETURN, enum_step::STEP_TRACKER_BOTH_RETURN);
      }
      break;
    case enum_step::STEP_WAIT_TRACKER_BOTH_RETURN:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_RETURN:
      step_tracker_both_return__ini(controller);
      HAL_Delay(1);
      step_tracker_both_return__end(controller, enum_step::STEP_WAIT_DECELERATE, enum_step::STEP_DECELERATE);
      break;

    case enum_step::STEP_WAIT_DECELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_DECELERATE:
      step_decelerate__ini(controller);
      step_decelerate__motor(controller, &motor_value, ESC_MID, 100);
      step_decelerate__end(controller, enum_step::STEP_FINISH, enum_step::STEP_FINISH);
      break;
    case enum_step::STEP_WAIT_FINISH:
      step_wait_finish(controller);
      break;
    case enum_step::STEP_FINISH:
      step_finish__ini(controller);
      step_finish__motor(controller);
      step_finish__end(controller);
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestI::sendMotorEncoderTrackerDistance() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      step_wait_init(controller);
      break;
    case enum_step::STEP_INIT:
      step_init__ini(controller);
      step_init__motor(controller, &motor_value);
      step_init__end(controller, enum_step::STEP_WAIT_ACCELERATE, enum_step::STEP_ACCELERATE);
      break;
    case enum_step::STEP_WAIT_ACCELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_ACCELERATE:
      step_accelerate__ini(controller);
      step_accelerate__motor(controller, &motor_value, ESC_FORWARD_MAX, 100);
      step_accelerate__end(controller, enum_step::STEP_WAIT_TRACKER_BOTH_START, enum_step::STEP_TRACKER_BOTH_START);
      break;

    case enum_step::STEP_WAIT_TRACKER_BOTH_START:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_START:
      step_tracker_both_start__ini(controller);
      first_cicle_trakcer_detected = 0;
      step_tracker_both_start__end(controller, enum_step::STEP_TRACKER_BOTH_DETECTED, enum_step::STEP_TRACKER_BOTH_DETECTED);
      break;
    case enum_step::STEP_WAIT_TRACKER_BOTH_DETECTED:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_DETECTED:
      if (first_cicle_trakcer_detected == 0) {
        step_tracker_both_detected__ini(controller);
        first_cicle_trakcer_detected = 1;
      }
      tracker_left_value = controller->getTrackerLeft()->getValue();
      controller->getUartSerial()->buffer(std::to_string(tracker_left_value));
      controller->getUartSerial()->buffer(" ");

      tracker_right_value = controller->getTrackerRight()->getValue();
      controller->getUartSerial()->buffer(std::to_string(tracker_right_value));
      controller->getUartSerial()->buffer(" ");

      distance_left_value = controller->getDistanceLeft()->getValue();
      controller->getUartSerial()->buffer(std::to_string(distance_left_value));
      controller->getUartSerial()->buffer(" ");

      distance_right_value = controller->getDistanceRight()->getValue();
      controller->getUartSerial()->bufferln(std::to_string(distance_right_value));



      if (tracker_left_value > 3000 || tracker_right_value > 3000) {
        HAL_Delay(1);
      }
      else {
        step_tracker_both_detected__end(controller, enum_step::STEP_WAIT_TRACKER_BOTH_RETURN, enum_step::STEP_TRACKER_BOTH_RETURN);
      }
      break;
    case enum_step::STEP_WAIT_TRACKER_BOTH_RETURN:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_RETURN:
      step_tracker_both_return__ini(controller);
      HAL_Delay(1);
      step_tracker_both_return__end(controller, enum_step::STEP_WAIT_DECELERATE, enum_step::STEP_DECELERATE);
      break;

    case enum_step::STEP_WAIT_DECELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_DECELERATE:
      step_decelerate__ini(controller);
      step_decelerate__motor(controller, &motor_value, ESC_MID, 100);
      step_decelerate__end(controller, enum_step::STEP_FINISH, enum_step::STEP_FINISH);
      break;
    case enum_step::STEP_WAIT_FINISH:
      step_wait_finish(controller);
      break;
    case enum_step::STEP_FINISH:
      step_finish__ini(controller);
      step_finish__motor(controller);
      step_finish__end(controller);
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestI::sendMotorEncoderTrackerDistanceBySpeed() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      step_wait_init(controller);
      break;
    case enum_step::STEP_INIT:
      step_init__ini(controller);
      step_init__motor(controller, &motor_value);
      num_test_repetition = 0;
      step_init__end(controller, enum_step::STEP_WAIT_ACCELERATE, enum_step::STEP_ACCELERATE);
      break;
    case enum_step::STEP_WAIT_ACCELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_ACCELERATE:
      step_accelerate__ini(controller);
      step_accelerate__motor(controller, &motor_value, ESC_FORWARD_MAX, 100);
      step_accelerate__end(controller, enum_step::STEP_WAIT_TRACKER_BOTH_START, enum_step::STEP_TRACKER_BOTH_START);
      break;

    case enum_step::STEP_WAIT_TRACKER_BOTH_START:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_START:
      step_tracker_both_start__ini(controller);
      first_cicle_trakcer_detected = 0;
      distance_left_count=0;
      distance_right_count=0;
      tracker_left_count=0;
      tracker_right_count=0;
      step_tracker_both_start__end(controller, enum_step::STEP_TRACKER_BOTH_DETECTED, enum_step::STEP_TRACKER_BOTH_DETECTED);
      break;
    case enum_step::STEP_WAIT_TRACKER_BOTH_DETECTED:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_DETECTED:
      if (first_cicle_trakcer_detected == 0) {
        step_tracker_both_detected__ini(controller);
        controller->getUartSerial()->buffer("------------------TEST REPETITION: ");
        controller->getUartSerial()->buffer(std::to_string(num_test_repetition));
        num_test_repetition++;
        first_cicle_trakcer_detected = 1;
      }


      tracker_left_value = controller->getTrackerLeft()->getValue();
      controller->getUartSerial()->buffer(std::to_string(tracker_left_value));
      controller->getUartSerial()->buffer("(");
      if (tracker_left_value < 3000) {
        if (tracker_left_count == 0) {
          controller->setTxStep(enum_step::STEP_TRACKER_LEFT_DETECTED); // Cambia el paso para evitar reentradas
          controller->commSendData();
        }
        tracker_left_count++;
      }
      else {
        if (tracker_left_count != 0) {
          controller->setTxStep(enum_step::STEP_TRACKER_LEFT_DETECTED); // Cambia el paso para evitar reentradas
          controller->commSendData();
        }
        tracker_left_count=0;
      }
      controller->getUartSerial()->buffer(std::to_string(tracker_left_count));
      controller->getUartSerial()->buffer(")");

      tracker_right_value = controller->getTrackerRight()->getValue();
      controller->getUartSerial()->buffer(std::to_string(tracker_right_value));
      controller->getUartSerial()->buffer("(");
      if (tracker_right_value < 3000) {
        if (tracker_right_count == 0) {
          controller->setTxStep(enum_step::STEP_TRACKER_RIGHT_DETECTED); // Cambia el paso para evitar reentradas
          controller->commSendData();
        }
        tracker_right_count++;
      }
      else {
        if (tracker_right_count != 0) {
          controller->setTxStep(enum_step::STEP_TRACKER_RIGHT_DETECTED); // Cambia el paso para evitar reentradas
          controller->commSendData();
        }
        tracker_right_count=0;
      }
      controller->getUartSerial()->buffer(std::to_string(tracker_right_count));
      controller->getUartSerial()->buffer(") ");

      distance_left_value = controller->getDistanceLeft()->getValue();
      controller->getUartSerial()->buffer(std::to_string(distance_left_value));
      controller->getUartSerial()->buffer("(");
      if (distance_left_value > 1000) {
        distance_left_count++;
      }
      else {
        distance_left_count=0;
      }
      controller->getUartSerial()->buffer(std::to_string(distance_left_count));
      controller->getUartSerial()->buffer(")");

      distance_right_value = controller->getDistanceRight()->getValue();
      controller->getUartSerial()->buffer(std::to_string(distance_right_value));
      controller->getUartSerial()->buffer("(");
      if (distance_right_value > 1000) {
        distance_right_count++;
      }
      else {
        distance_right_count=0;
      }
      controller->getUartSerial()->buffer(std::to_string(distance_right_count));
      controller->getUartSerial()->bufferln(")");


//      if (tracker_left_value > 3000 || tracker_right_value > 3000) {
        HAL_Delay(1);
//      }
//      else {
//        step_tracker_both_detected__end(controller, enum_step::STEP_WAIT_TRACKER_BOTH_RETURN, enum_step::STEP_TRACKER_BOTH_RETURN);
//      }
//      break;
    case enum_step::STEP_WAIT_TRACKER_BOTH_RETURN:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_RETURN:
      step_tracker_both_return__ini(controller);
      HAL_Delay(1);
      step_tracker_both_return__end(controller, enum_step::STEP_WAIT_DECELERATE, enum_step::STEP_DECELERATE);
      break;

    case enum_step::STEP_WAIT_DECELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_DECELERATE:
      step_decelerate__ini(controller);
      step_decelerate__motor(controller, &motor_value, ESC_MID, 100);
      step_decelerate__end(controller, enum_step::STEP_FINISH, enum_step::STEP_FINISH);
      break;
    case enum_step::STEP_WAIT_FINISH:
      step_wait_finish(controller);
      break;
    case enum_step::STEP_FINISH:
      step_finish__ini(controller);
      step_finish__motor(controller);
      step_finish__end(controller);
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestI::sendMotorEncoderTrackerLeft() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      step_wait_init(controller);
      break;
    case enum_step::STEP_INIT:
      step_init__ini(controller);
      step_init__motor(controller, &motor_value);
      step_init__end(controller, enum_step::STEP_WAIT_ACCELERATE, enum_step::STEP_ACCELERATE);
      break;
    case enum_step::STEP_WAIT_ACCELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_ACCELERATE:
      step_accelerate__ini(controller);
      step_accelerate__motor(controller, &motor_value, ESC_FORWARD_MAX, 100);
      motor_left_value = motor_value;
      motor_right_value = motor_value;
      motor_left_before = motor_value;
      motor_right_before = motor_value;
      step_accelerate__end(controller, enum_step::STEP_WAIT_TRACKER_BOTH_START, enum_step::STEP_TRACKER_BOTH_START);
      break;

    case enum_step::STEP_WAIT_TRACKER_BOTH_START:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_START:
      step_tracker_both_start__ini(controller);
      first_cicle_trakcer_detected = 0;
      step_tracker_both_start__end(controller, enum_step::STEP_TRACKER_BOTH_DETECTED, enum_step::STEP_TRACKER_BOTH_DETECTED);
      break;
    case enum_step::STEP_WAIT_TRACKER_BOTH_DETECTED:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_DETECTED:
      if (first_cicle_trakcer_detected == 0) {
        step_tracker_both_detected__ini(controller);
        first_cicle_trakcer_detected = 1;
      }
      tracker_left_value = controller->getTrackerLeft()->getValue();
      controller->getUartSerial()->buffer(std::to_string(tracker_left_value));
      controller->getUartSerial()->buffer(" ");

      tracker_right_value = controller->getTrackerRight()->getValue();
      controller->getUartSerial()->buffer(std::to_string(tracker_right_value));
      controller->getUartSerial()->buffer(" ");

      distance_left_value = controller->getDistanceLeft()->getValue();
      controller->getUartSerial()->buffer(std::to_string(distance_left_value));
      controller->getUartSerial()->buffer(" ");

      distance_right_value = controller->getDistanceRight()->getValue();
      controller->getUartSerial()->buffer(std::to_string(distance_right_value));
      controller->getUartSerial()->buffer(" ");





      if (tracker_left_value < 3000) {
        if (motor_left_value == ESC_FORWARD_MAX) {
          motor_left_value = ESC_FORWARD_MAX + 100;
        }
        else {
          motor_left_value = motor_left_value + 1;
          if (motor_left_value > ESC_FORWARD_MAX + 400) {
            motor_left_value = motor_left_before;
          }
        }
      } else {
        motor_left_value = ESC_FORWARD_MAX;
      }

      if (motor_left_value != motor_left_before) {
        controller->getMotorLeft()->setSpeed(motor_left_value);
        motor_left_before = motor_left_value;
      }
      controller->getUartSerial()->buffer(std::to_string(motor_left_value));
      controller->getUartSerial()->buffer(" ");

      controller->getUartSerial()->bufferln(std::to_string(motor_right_value));




//      if (tracker_left_value > 3000 || tracker_right_value > 3000) {
        HAL_Delay(1);
//      }
//      else {
//        step_tracker_both_detected__end(controller, enum_step::STEP_WAIT_TRACKER_BOTH_RETURN, enum_step::STEP_TRACKER_BOTH_RETURN);
//      }
      break;
    case enum_step::STEP_WAIT_TRACKER_BOTH_RETURN:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_RETURN:
      step_tracker_both_return__ini(controller);
      HAL_Delay(1);
      step_tracker_both_return__end(controller, enum_step::STEP_WAIT_DECELERATE, enum_step::STEP_DECELERATE);
      break;

    case enum_step::STEP_WAIT_DECELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_DECELERATE:
      step_decelerate__ini(controller);
      step_decelerate_separate__motor(controller, &motor_left_value, &motor_right_value, ESC_MID, 100);
      step_decelerate__end(controller, enum_step::STEP_FINISH, enum_step::STEP_FINISH);
      break;
    case enum_step::STEP_WAIT_FINISH:
      step_wait_finish(controller);
      break;
    case enum_step::STEP_FINISH:
      step_finish__ini(controller);
      step_finish__motor(controller);
      step_finish__end(controller);
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestI::sendMotorEncoderTrackerRight() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      step_wait_init(controller);
      break;
    case enum_step::STEP_INIT:
      step_init__ini(controller);
      step_init__motor(controller, &motor_value);
      step_init__end(controller, enum_step::STEP_WAIT_ACCELERATE, enum_step::STEP_ACCELERATE);
      break;
    case enum_step::STEP_WAIT_ACCELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_ACCELERATE:
      step_accelerate__ini(controller);
      step_accelerate__motor(controller, &motor_value, ESC_FORWARD_MAX, 100);
      motor_left_value = motor_value;
      motor_right_value = motor_value;
      motor_left_before = motor_value;
      motor_right_before = motor_value;
      step_accelerate__end(controller, enum_step::STEP_WAIT_TRACKER_BOTH_START, enum_step::STEP_TRACKER_BOTH_START);
      break;

    case enum_step::STEP_WAIT_TRACKER_BOTH_START:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_START:
      step_tracker_both_start__ini(controller);
      first_cicle_trakcer_detected = 0;
      step_tracker_both_start__end(controller, enum_step::STEP_TRACKER_BOTH_DETECTED, enum_step::STEP_TRACKER_BOTH_DETECTED);
      break;
    case enum_step::STEP_WAIT_TRACKER_BOTH_DETECTED:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_DETECTED:
      if (first_cicle_trakcer_detected == 0) {
        step_tracker_both_detected__ini(controller);
        first_cicle_trakcer_detected = 1;
      }
      tracker_left_value = controller->getTrackerLeft()->getValue();
      controller->getUartSerial()->buffer(std::to_string(tracker_left_value));
      controller->getUartSerial()->buffer(" ");

      tracker_right_value = controller->getTrackerRight()->getValue();
      controller->getUartSerial()->buffer(std::to_string(tracker_right_value));
      controller->getUartSerial()->buffer(" ");

      distance_left_value = controller->getDistanceLeft()->getValue();
      controller->getUartSerial()->buffer(std::to_string(distance_left_value));
      controller->getUartSerial()->buffer(" ");

      distance_right_value = controller->getDistanceRight()->getValue();
      controller->getUartSerial()->buffer(std::to_string(distance_right_value));
      controller->getUartSerial()->buffer(" ");





      if (tracker_right_value < 3000) {
        if (motor_right_value == ESC_FORWARD_MAX) {
          motor_right_value = ESC_FORWARD_MAX + 100;
        }
        else {
          motor_right_value = motor_right_value + 1;
          if (motor_right_value > ESC_FORWARD_MAX + 400) {
            motor_right_value = motor_right_before;
          }
        }
      } else {
          motor_right_value = ESC_FORWARD_MAX;
      }

      if (motor_right_value != motor_right_before) {
        controller->getMotorRight()->setSpeed(motor_right_value);
        motor_right_before = motor_right_value;
      }
      controller->getUartSerial()->buffer(std::to_string(motor_left_value));
      controller->getUartSerial()->buffer(" ");

      controller->getUartSerial()->bufferln(std::to_string(motor_right_value));



//      if (tracker_left_value > 3000 || tracker_right_value > 3000) {
        HAL_Delay(1);
//      }
//      else {
//        step_tracker_both_detected__end(controller, enum_step::STEP_WAIT_TRACKER_BOTH_RETURN, enum_step::STEP_TRACKER_BOTH_RETURN);
//      }
      break;
    case enum_step::STEP_WAIT_TRACKER_BOTH_RETURN:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_RETURN:
      step_tracker_both_return__ini(controller);
      HAL_Delay(1);
      step_tracker_both_return__end(controller, enum_step::STEP_WAIT_DECELERATE, enum_step::STEP_DECELERATE);
      break;

    case enum_step::STEP_WAIT_DECELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_DECELERATE:
      step_decelerate__ini(controller);
      step_decelerate_separate__motor(controller, &motor_left_value, &motor_right_value, ESC_MID, 100);
      step_decelerate__end(controller, enum_step::STEP_FINISH, enum_step::STEP_FINISH);
      break;
    case enum_step::STEP_WAIT_FINISH:
      step_wait_finish(controller);
      break;
    case enum_step::STEP_FINISH:
      step_finish__ini(controller);
      step_finish__motor(controller);
      step_finish__end(controller);
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestI::sendMotorEncoderDistanceLeft() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      step_wait_init(controller);
      break;
    case enum_step::STEP_INIT:
      step_init__ini(controller);
      step_init__motor(controller, &motor_value);
      step_init__end(controller, enum_step::STEP_WAIT_ACCELERATE, enum_step::STEP_ACCELERATE);
      break;
    case enum_step::STEP_WAIT_ACCELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_ACCELERATE:
      step_accelerate__ini(controller);
      step_accelerate__motor(controller, &motor_value, ESC_FORWARD_MAX, 100);
      motor_left_value = motor_value;
      motor_right_value = motor_value;
      motor_left_before = motor_value;
      motor_right_before = motor_value;
      step_accelerate__end(controller, enum_step::STEP_WAIT_TRACKER_BOTH_START, enum_step::STEP_TRACKER_BOTH_START);
      break;

    case enum_step::STEP_WAIT_TRACKER_BOTH_START:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_START:
      step_tracker_both_start__ini(controller);
      first_cicle_trakcer_detected = 0;
      step_tracker_both_start__end(controller, enum_step::STEP_TRACKER_BOTH_DETECTED, enum_step::STEP_TRACKER_BOTH_DETECTED);
      break;
    case enum_step::STEP_WAIT_TRACKER_BOTH_DETECTED:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_DETECTED:
      if (first_cicle_trakcer_detected == 0) {
        step_tracker_both_detected__ini(controller);
        first_cicle_trakcer_detected = 1;
      }
      tracker_left_value = controller->getTrackerLeft()->getValue();
      controller->getUartSerial()->buffer(std::to_string(tracker_left_value));
      controller->getUartSerial()->buffer(" ");

      tracker_right_value = controller->getTrackerRight()->getValue();
      controller->getUartSerial()->buffer(std::to_string(tracker_right_value));
      controller->getUartSerial()->buffer(" ");

      distance_left_value = controller->getDistanceLeft()->getValue();
      controller->getUartSerial()->buffer(std::to_string(distance_left_value));
      controller->getUartSerial()->buffer(" ");

      distance_right_value = controller->getDistanceRight()->getValue();
      controller->getUartSerial()->buffer(std::to_string(distance_right_value));
      controller->getUartSerial()->buffer(" ");


      if (distance_left_value > 900) {
          if (motor_right_value == ESC_FORWARD_MAX) {
            motor_right_value = ESC_FORWARD_MAX + 100;
          }
          else {
            motor_right_value = motor_right_value + 1;
            if (motor_right_value > ESC_FORWARD_MAX + 300) {
              motor_right_value = motor_right_before;
            }
          }
      } else {
          motor_right_value = ESC_FORWARD_MAX;
      }

      if (motor_right_value != motor_right_before) {
        controller->getMotorRight()->setSpeed(motor_right_value);
        motor_right_before = motor_right_value;
      }
      controller->getUartSerial()->buffer(std::to_string(motor_left_value));
      controller->getUartSerial()->buffer(" ");

      controller->getUartSerial()->bufferln(std::to_string(motor_right_value));



//      if (tracker_left_value > 3000 || tracker_right_value > 3000) {
        HAL_Delay(1);
//      }
//      else {
//        step_tracker_both_detected__end(controller, enum_step::STEP_WAIT_TRACKER_BOTH_RETURN, enum_step::STEP_TRACKER_BOTH_RETURN);
//      }
      break;
    case enum_step::STEP_WAIT_TRACKER_BOTH_RETURN:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_RETURN:
      step_tracker_both_return__ini(controller);
      HAL_Delay(1);
      step_tracker_both_return__end(controller, enum_step::STEP_WAIT_DECELERATE, enum_step::STEP_DECELERATE);
      break;

    case enum_step::STEP_WAIT_DECELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_DECELERATE:
      step_decelerate__ini(controller);
      step_decelerate_separate__motor(controller, &motor_left_value, &motor_right_value, ESC_MID, 100);
      step_decelerate__end(controller, enum_step::STEP_FINISH, enum_step::STEP_FINISH);
      break;
    case enum_step::STEP_WAIT_FINISH:
      step_wait_finish(controller);
      break;
    case enum_step::STEP_FINISH:
      step_finish__ini(controller);
      step_finish__motor(controller);
      step_finish__end(controller);
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}

void TestI::sendMotorEncoderDistanceRight() {
  switch (controller->getRxStep()) {
    case enum_step::STEP_WAIT_INIT:
      step_wait_init(controller);
      break;
    case enum_step::STEP_INIT:
      step_init__ini(controller);
      step_init__motor(controller, &motor_value);
      step_init__end(controller, enum_step::STEP_WAIT_ACCELERATE, enum_step::STEP_ACCELERATE);
      break;
    case enum_step::STEP_WAIT_ACCELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_ACCELERATE:
      step_accelerate__ini(controller);
      step_accelerate__motor(controller, &motor_value, ESC_FORWARD_MAX, 100);
      motor_left_value = motor_value;
      motor_right_value = motor_value;
      motor_left_before = motor_value;
      motor_right_before = motor_value;
      step_accelerate__end(controller, enum_step::STEP_WAIT_TRACKER_BOTH_START, enum_step::STEP_TRACKER_BOTH_START);
      break;

    case enum_step::STEP_WAIT_TRACKER_BOTH_START:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_START:
      step_tracker_both_start__ini(controller);
      first_cicle_trakcer_detected = 0;
      step_tracker_both_start__end(controller, enum_step::STEP_TRACKER_BOTH_DETECTED, enum_step::STEP_TRACKER_BOTH_DETECTED);
      break;
    case enum_step::STEP_WAIT_TRACKER_BOTH_DETECTED:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_DETECTED:
      if (first_cicle_trakcer_detected == 0) {
        step_tracker_both_detected__ini(controller);
        first_cicle_trakcer_detected = 1;
      }


      tracker_left_value = controller->getTrackerLeft()->getValue();
      controller->getUartSerial()->buffer(std::to_string(tracker_left_value));
      controller->getUartSerial()->buffer(" ");

      tracker_right_value = controller->getTrackerRight()->getValue();
      controller->getUartSerial()->buffer(std::to_string(tracker_right_value));
      controller->getUartSerial()->buffer(" ");

      distance_left_value = controller->getDistanceLeft()->getValue();
      controller->getUartSerial()->buffer(std::to_string(distance_left_value));
      controller->getUartSerial()->buffer(" ");

      distance_right_value = controller->getDistanceRight()->getValue();
      controller->getUartSerial()->buffer(std::to_string(distance_right_value));
      controller->getUartSerial()->buffer(" ");





      if (distance_right_value > 900) {
          if (motor_left_value == ESC_FORWARD_MAX) {
            motor_left_value = ESC_FORWARD_MAX + 100;
          }
          else {
            motor_left_value = motor_left_value + 1;
            if (motor_left_value > ESC_FORWARD_MAX + 400) {
              motor_left_value = motor_left_before;
            }
          }
      } else {
          motor_left_value = ESC_FORWARD_MAX;
      }

      if (motor_left_value != motor_left_before) {
        controller->getMotorLeft()->setSpeed(motor_left_value);
        motor_left_before = motor_left_value;
      }
      controller->getUartSerial()->buffer(std::to_string(motor_left_value));
      controller->getUartSerial()->buffer(" ");

      controller->getUartSerial()->bufferln(std::to_string(motor_right_value));







//      if (tracker_left_value > 3000 || tracker_right_value > 3000) {
        HAL_Delay(1);
//      }
//      else {
//        step_tracker_both_detected__end(controller, enum_step::STEP_WAIT_TRACKER_BOTH_RETURN, enum_step::STEP_TRACKER_BOTH_RETURN);
//      }
      break;
    case enum_step::STEP_WAIT_TRACKER_BOTH_RETURN:
      step_wait(controller);
      break;
    case enum_step::STEP_TRACKER_BOTH_RETURN:
      step_tracker_both_return__ini(controller);
      HAL_Delay(1);
      step_tracker_both_return__end(controller, enum_step::STEP_WAIT_DECELERATE, enum_step::STEP_DECELERATE);
      break;

    case enum_step::STEP_WAIT_DECELERATE:
      step_wait(controller);
      break;
    case enum_step::STEP_DECELERATE:
      step_decelerate__ini(controller);
      step_decelerate_separate__motor(controller, &motor_left_value, &motor_right_value, ESC_MID, 100);
      step_decelerate__end(controller, enum_step::STEP_FINISH, enum_step::STEP_FINISH);
      break;
    case enum_step::STEP_WAIT_FINISH:
      step_wait_finish(controller);
      break;
    case enum_step::STEP_FINISH:
      step_finish__ini(controller);
      step_finish__motor(controller);
      step_finish__end(controller);
    break;
    default:
      // Manejo de otros pasos si es necesario
      break;
  }
}


void TestI::receiveData() {
}
