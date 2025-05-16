#pragma once
#include "Position.hpp"
#include <unordered_map>
#include <string>

class PositionTable{
    private:
        std::unordered_map<std::string, Position> positions;
    public:
        void insert(const Position& pos);
        Position* find(const std::string&id);
        void init();
        std::unordered_map<std::string, Position> findAll();
};