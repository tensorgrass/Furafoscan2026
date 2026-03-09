#ifndef TESTE_HPP
#define TESTE_HPP

#include "TestBase.hpp"

class TestE : public TestBase {
   public:
    TestE(ControllerBase* controllerBaseValue);

    char getGroup() const override { return 'E'; }
    void sendData() override;
    void receiveData() override;

    void sendTrackerLeft();
    void sendTrackerRight();
    void sendTrackerMultipleLeft();
    void sendTrackerMultipleRight();
};

#endif  // TESTE_HPP
