#pragma once
#include "CurrentState_enum.hpp"
#include <string>

using std::string;

class Input {
    private:
    string command;
    CurrentState currentState;

    public: 
    void sendOperation(CurrentState, string);
    void printStock();
    void selectBeverage(int, int);
    void enterCardNum(int);
};
