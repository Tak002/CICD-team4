#include "BeverageTable.hpp"
#include "../Beverage.hpp"
#include <unordered_map>
#include <list>


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

void BeverageTable::updateStock(int item_code, Beverage item ){
    
    beverages[item_code] = item;
}

void BeverageTable::init(){
    beverages.clear();
    for(int i = 0; i<20; i++){
        
    }
}

