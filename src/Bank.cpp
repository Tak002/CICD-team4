// Bank.cpp
#include "Bank.hpp"



bool Bank::checkCardValidity(int card_num, int price){
    auto it = cards.find(card_num);

    if (it == cards.end()) {
        return false;
    }

    if (it->second < price) {
        return false;
    }

    return true;
}

void Bank::updateBalance(int card_num, int price){
    auto it = cards.find(card_num);

    it->second = it -> second - price;

}

bool Bank::reqeustPayment(int card_num, int price){
    if(checkCardValidity(card_num,price)){
        updateBalance(card_num, price);
        return true;
    }

    return false;
}

void Bank::rollback(int card_num, int price){
    auto it = cards.find(card_num);
    if(it == cards.end()){
        return;
    }

    it->second += price;
}   