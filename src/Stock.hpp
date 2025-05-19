#pragma once
#include <list>
#include "Beverage.hpp"

using namespace std;

class Stock {
    
    private:
        
        Beverage bev_to_buy; // 현재 구매 시도중인 음료
        list<Beverage> beverage_list;
        void updateStock(int, int);
        

    public:
        void requestBeverage(int item_id, int item_num);
        list<Beverage> getCurrentStock();
        bool isPrepayment(int item_code, int item_num);
        bool isBuyable(int item_code, int item_num);
        void blockSale();
        void editStock(int item_id, int item_num);

        void refreshStock();
        void cancleBuying();

        Beverage getBevToBuy();

};
