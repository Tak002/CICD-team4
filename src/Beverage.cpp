// Beverage.cpp
#include "Beverage.hpp"

bool Beverage::isSameId(int id){
    if(id == bev_code){
        return true;
    }
    return false;
}

bool Beverage::isEnough(int num){
    if(num<=bev_num){
        return true;
    }
    return false;
}

void Beverage::reduceBeverage(int num){
    if(this->bev_num>=num){
        bev_num -= num;
    }
}

int Beverage::getBevCode() const {
    return bev_code;
}

int Beverage::getBevNum() const {
    return bev_num;
}

int Beverage::getPrice() const {
    return price;
}