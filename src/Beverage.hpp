#pragma once

class Beverage {
    private:
        int bevId;
        int bevQuantity;

        friend class BeverageTest;

    public:
        Beverage() : bevId(0), bevQuantity(0) {}
        Beverage(int id, int quantity) : bevId(id), bevQuantity(quantity) {}

        bool isSameId(int id);
        bool isEnough(int id);
        void reduceBeverage(int num);
        int getBevId() const;
        int getBevQuantity() const;
};
