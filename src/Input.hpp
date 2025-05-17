#pragma once
#include "CurrentState_enum.hpp"
#include <string>
#include <Controller.hpp>
using std::string;

class Input {
    private:
    Controller* controller;
    string command;
    CurrentState currentState;

    public: 
    Input(Controller *ctrl);
    void run();
    void sendOperation(CurrentState, string);
    void printStock();
    void selectBeverage(int, int);
    void enterCardNum(int);
};
