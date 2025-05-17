// Input.cpp
#include "Input.hpp"

Input::Input(Controller* ctrl){
    controller = ctrl;
}

void Input::run(){}
void Input::sendOperation(CurrentState, string){};
void Input::printStock(){};
void Input::selectBeverage(int, int){};
void Input::enterCardNum(int){};