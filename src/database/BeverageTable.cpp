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

std::list<Beverage> BeverageTable::findAll() {
    std::list<Beverage> result;
    for (const auto& pair : beverages) {
        result.push_back(pair.second);
    }
    return result;
}

void BeverageTable::init(){
    beverages.clear();
}

