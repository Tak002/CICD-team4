// Stock.cpp
#include "Stock.hpp"
#include "database/InMemoryDB.hpp"
#include "MSG.hpp"
#include "struct/PendingBeverage.hpp"

void Stock::updateStock(int item_code, int item_num){

    InMemoryDB& db = InMemoryDB::instance();
    list<Beverage> beverages = getCurrentStock();
    // db 업데이트
    for(Beverage beverage : beverages){
        if(beverage.isSameId(item_code)){
            beverage.reduceBeverage(item_num);
            db.beverageTable.updateStock(item_code, beverage);
            break;
        }
    }

    // 구매과정 중인 음료 초기화
    PendingBeverage& pending = PendingBeverage::instance();
    pending.reset();

}

list<Beverage> Stock::getCurrentStock(){
    refreshStock();
    return beverage_list;
}

void Stock::refreshStock(){
    auto& db = InMemoryDB::instance();
    list<Beverage> beverages = db.beverageTable.findAll();
    this->beverage_list = beverages;
}

bool Stock::isPrepayment(int item_code, int item_num){
    
    PendingBeverage& pending = PendingBeverage::instance();
    pending.setOnce(item_code, item_num);

    return !isBuyable(item_code, item_num);
}

bool Stock::isBuyable(int item_code, int item_num){
    refreshStock();
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
    MSG message;
    // message.DVMMessageOutofStock(item_code, item_num);
}

void Stock::blockSale(){
    // todo
    // 관리자 모드
}

void Stock::editStock(int item_code, int item_num){
    // todo
    // 관리자 모드
}

Beverage Stock::getBevToBuy(){
    return bev_to_buy;
}

void Stock::cancleBuying(){
    // 구매과정 중인 음료 초기화
    PendingBeverage& pending = PendingBeverage::instance();
    pending.reset();
}