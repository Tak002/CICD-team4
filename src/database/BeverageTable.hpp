#pragma once
#include <unordered_map>

#include "Beverage.hpp"


class BeverageTable{
    private:
        std::unordered_map<int, Beverage> beverages;
    public:
        void insert(const Beverage& bev);
        Beverage* find(const int item_code);
        void init();
        std::unordered_map<int, Beverage> findAll();
};