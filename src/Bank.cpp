// Bank.cpp
#include "Bank.hpp"


Bank::Bank() {
    // todo 
    // 임의의 생성자. 카드 정보를 외부의 다른 시스템이 가질 수 있도록
    srand(static_cast<unsigned int>(time(nullptr)));

    // 임의의 카드 5개를 생성
    for (int i = 0; i < 5; ++i) {
        int card_num = 1000 + rand() % 9000;    // 1000~9999 범위 카드 번호
        int balance = 1000 + rand() % 10000;    // 1000~10999 잔고
        cards[card_num] = balance;
    }
}

bool Bank::checkCardValidity(int card_num, int price){
    auto it = cards.find(card_num);

    if (it == cards.end()) {
        return false;
    }

    if (it->second < price) {
        return false;
    }

    updateBalance(card_num, price);
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