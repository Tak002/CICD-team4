// Beverage.cpp
#include "Beverage.hpp"

bool Beverage::isSameId(int id){
    if(id == bevId){
        return true;
    }
    return false;
}

bool Beverage::isEnough(int num){
    if(num<=bevQuantity){
        return true;
    }
    return false;
}

void Beverage::reduceBeverage(int num){
    if(this->bevQuantity>=num){
        bevQuantity -= num;
    }
}