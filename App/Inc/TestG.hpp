#ifndef TESTG_HPP
#define TESTG_HPP

#include "TestBase.hpp"

class TestG : public TestBase {
   public:
    TestG(ControllerBase* controllerBaseValue);

    char getGroup() const override { return 'G'; }
    void sendData() override;
    void receiveData() override;

    void sendTrackerLeft();
    void sendTrackerRight();
    void sendTrackerMultipleLeft();
    void sendTrackerMultipleRight();
};

#endif  // TESTG_HPP
