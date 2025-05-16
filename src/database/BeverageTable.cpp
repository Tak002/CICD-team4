#include "BeverageTable.hpp"
#include "Beverage.hpp"
#include <unordered_map>

void BeverageTable::insert(const Beverage& bev) {
    beverages[bev.getBevId()] = bev;
} 

Beverage* BeverageTable::find(int id) {
    auto it = beverages.find(id);
    return it != beverages.end() ? &it->second : nullptr;
}

std::unordered_map<int, Beverage>  BeverageTable::findAll(){
    return beverages;
}

void BeverageTable::init(){
    beverages.clear();
}

