#pragma once
#include <string>
#include "../enum/BeverageType.hpp"


struct PendingBeverage {


    int total_price() const {
        return item_num * unit_price;
    }

    int getId() const {
        return item_id;
    }

    int getNum() const {
        return item_num;
    }

    static PendingBeverage& instance(){
        static PendingBeverage singletonInstance;
        return singletonInstance;
    }

    bool setOnce(int id, int num){
        if(locked) return false;
        item_id = id;
        item_num = num;
        
        BeverageType type = static_cast<BeverageType>(id - 1);
        unit_price = toPrice(type);
        return true;
    }

    void reset(){
        item_id = -1;
        item_num = -1;
        unit_price = -1;
        locked = false;
    }

    private:
        int item_id;            // 음료 코드
        int item_num;           // 수량
        int unit_price;         // 단가
        bool locked;


        PendingBeverage() :item_id(0), item_num(0), unit_price(0) {}
};
