#pragma once
#include <map>


class Bank {
    private:
        bool checkCardValidity(int card_num, int price) const;
        void updateBalance(int card_num, int price) const ;

        friend class BankTest;
        
    public:
        bool reqeustPayment(int card_num, int price);
        void rollback(int card_num, int price) const ;
        
};
