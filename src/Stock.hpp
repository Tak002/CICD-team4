#pragma once
#include <list>
#include "Beverage.hpp"

using namespace std;

class Stock {
    
    private:
        Beverage beverage;
        void updateStock(int, int);
        void DVMMessageOutofStock(int item_id, int item_num);
        

    public:
        void requestBeverage(int item_id, int item_num);
        list<Beverage> getCurrentStock(int beverageId);
        bool isPrepayment(int item_code, int item_num);
        bool isBuyable(int item_code, int item_num);
        void blockSale();
        void editStock(int item_id, int item_num);


};
