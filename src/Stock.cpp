// Stock.cpp
#include "Stock.hpp"


void Stock::updateStock(int item_code, int item_num){

    for(Beverage beverage : beverage_list){
        if(beverage.isSameId(item_code)){
            beverage.reduceBeverage(item_num);
        }
    }

}

list<Beverage> Stock::getCurrentStock(){

    return beverage_list;
}

bool Stock::isPrepayment(int item_code, int item_num){
    
    for(Beverage beverage : beverage_list){
        if (beverage.isSameId(item_code)){
            if(beverage.isEnough(item_num)){
                return true;
            }
        }
    }

    return false;
}

void Stock::requestBeverage(int item_code, int item_num){
    updateStock(item_code, item_num);
}


void Stock::DVMMessageOutofStock(int item_code, int item_num){
    // todo
}

void Stock::blockSale(){
    // todo
}

void Stock::editStock(int item_code, int item_num){
    // todo
}