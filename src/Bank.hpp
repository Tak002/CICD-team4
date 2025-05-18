#pragma once
#include <map>

using namespace std;

class Bank {
    private:
        map<int, int> cards;
        bool checkCardValidity(int card_num, int price);
        void updateBalance(int card_num, int price);

        friend class BankTest;
        
    public:
        Bank();
        bool reqeustPayment(int card_num, int price);
        void rollback(int card_num, int price);
        
};
