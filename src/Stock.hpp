#pragma once
#include <list>
#include "Beverage.hpp"
#include <string>


class Stock {
    
    private:
        
        Beverage bev_to_buy; // 현재 구매 시도중인 음료
        void updateStock(const int, const int);
        

    public:
        void requestBeverage(int item_id, int item_num);
        std::list<Beverage> getCurrentStock() const;
        bool isPrepayment(int item_code, int item_num);
        bool isBuyable(std::string certCode,int item_code, int item_num);
        void blockSale();
        void editStock(int item_id, int item_num);

        void refreshStock();
        void cancleBuying();

        Beverage getBevToBuy();

};
