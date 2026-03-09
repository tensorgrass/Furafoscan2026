#ifndef TESTA_HPP
#define TESTA_HPP

#include "TestBase.hpp"

class TestA : public TestBase {
   public:
    TestA(ControllerBase* controllerBaseValue);

    char getGroup() const override { return 'A'; }
    void sendData() override;
    void receiveData() override;

    void sendOneMessage();
    void sendMultipleMessages();
};

#endif  // TESTA_HPP