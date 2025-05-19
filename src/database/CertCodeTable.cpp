#include "CertCodeTable.hpp"
#include "Beverage.hpp"
#include <string>
#include <tuple>


void CertCodeTable::insert(std::string cert_code, int item_code, int item_num) {
    std::tuple<std::string, int, int> cab(cert_code,item_code, item_num);
    codeAndItem.back(cab);
}

Beverage CertCodeTable::find(const std::string& id) {
    codeAndItem.
}

std::unordered_map<std::string, Position>  PositionTable::findAll(){
    return positions;
}

void PositionTable::init(){
    positions.clear();
}
