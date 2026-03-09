#ifndef TESTD_HPP
#define TESTD_HPP

#include "TestBase.hpp"

class TestD : public TestBase {
   public:
    TestD(ControllerBase* controllerBaseValue);

    char getGroup() const override { return 'D'; }
    void sendData() override;
    void receiveData() override;

    void sendTrackerLeft();
    void sendTrackerRight();
    void sendTrackerMultipleLeft();
    void sendTrackerMultipleRight();
};

#endif  // TESTD_HPP
