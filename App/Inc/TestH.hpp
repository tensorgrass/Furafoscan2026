#ifndef TESTH_HPP
#define TESTH_HPP

#include "TestBase.hpp"

class TestH : public TestBase {
   public:
    TestH(ControllerBase* controllerBaseValue);

    char getGroup() const override { return 'H'; }
    void sendData() override;
    void receiveData() override;

    void sendMotorLeft();
    void sendMotorRight();
    void sendMotorEncoderLeft();
    void sendMotorEncoderRight();
   private:
    uint32_t motor_value = ESC_MID;
};

#endif  // TESTH_HPP
