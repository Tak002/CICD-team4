#pragma once

class Beverage {
    private:
        int bev_code;
        int bev_num;
        int price;

        friend class BeverageTest;

    public:
        Beverage() : bev_code(0), bev_num(0) {}
        Beverage(int item_code, int item_num) : bev_code(item_code), bev_num(item_num), price(0) {}
        Beverage(int item_code, int item_num, int p) : bev_code(item_code), bev_num(item_num),price(p) {}

        bool isSameId(int id);
        bool isEnough(int id);
        void reduceBeverage(int num);
        int getBevCode() const;
        int getBevNum() const;
        int getPrice() const;
};
