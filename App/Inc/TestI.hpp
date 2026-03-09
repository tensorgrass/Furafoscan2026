#ifndef TESTI_HPP
#define TESTI_HPP

#include "TestBase.hpp"

class TestI : public TestBase {
   public:
    TestI(ControllerBase* controllerBaseValue);

    char getGroup() const override { return 'I'; }
    void sendData() override;
    void receiveData() override;

    void sendMotorEncoder();
    void sendMotorEncoderTracker();
    void sendMotorEncoderTrackerDistance();
    void sendMotorEncoderTrackerDistanceBySpeed();
    void sendMotorEncoderTrackerLeft();
    void sendMotorEncoderTrackerRight();
    void sendMotorEncoderDistanceLeft();
    void sendMotorEncoderDistanceRight();
   private:
    uint32_t motor_value = ESC_MID;
    uint32_t motor_left_value = ESC_MID;
    uint32_t motor_right_value = ESC_MID;
    uint32_t motor_left_before = ESC_MID;
    uint32_t motor_right_before = ESC_MID;
    uint16_t tracker_left_value;
    uint16_t tracker_left_count;
    uint16_t tracker_right_value;
    uint16_t tracker_right_count;
    uint16_t distance_left_value;
    uint16_t distance_left_count;
    uint16_t distance_right_value;
    uint16_t distance_right_count;

    uint8_t first_cicle_trakcer_detected = 0;

    uint8_t num_test_repetition;
};

#endif  // TESTI_HPP
