#ifndef TESTC_HPP
#define TESTC_HPP

#include "TestBase.hpp"

class TestC : public TestBase {
   public:
    TestC(ControllerBase* controllerBaseValue);

    char getGroup() const override { return 'C'; }
    void sendData() override;
    void receiveData() override;

    void sendPressButtonStart();
    void sendPressButtonStart2();

   private:
    uint8_t first_cicle_button_start = 0;

    uint8_t previus_state_button = 0;
    uint32_t num_ms_button = 0;
    uint32_t ini_time = 0;
};

#endif  // TESTC_HPP
