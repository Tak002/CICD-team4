#pragma once

class Beverage {
    private:
        int bevId;
        int bevQuantity;
    
    public:
        bool isSameId(int id);
        bool isEnough(int id);
        void reduceBeverage(int num);
        int getBevId() const;
        int getBevQuantity() const;
};
