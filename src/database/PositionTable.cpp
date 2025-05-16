#include "PositionTable.hpp"
#include "Position.hpp"
#include <unordered_map>

void PositionTable::insert(const Position& pos) {
    positions[pos.src_id] = pos;  // pos.id가 key로 들어감
}

Position* PositionTable::find(const std::string& id) {
    auto it = positions.find(id);
    return it != positions.end() ? &it->second : nullptr;
}

std::unordered_map<std::string, Position>  PositionTable::findAll(){
    return positions;
}

void PositionTable::init(){
    positions.clear();
}