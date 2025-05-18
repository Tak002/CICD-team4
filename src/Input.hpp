#pragma once
#include "CurrentState_enum.hpp"
#include <string>
#include "Controller.hpp"
#include <iostream>
#include "enum/CommandType.hpp"

using std::string;

class Input {
    private:
    Controller* controller;
    string command;
    CurrentState currentState;
    std::string getFirstScreenAnswer();

    public: 
    Input(Controller *ctrl);
    void run();
};
