// Calc.cpp
#include "Calc.hpp"
#include <map>
#include <string>
#include <unordered_map>
#include <tuple>
#include <limits>   
#include <cmath>    



#include "struct/Position.hpp"
#include "database/InMemoryDB.hpp"



std::map<int, int> Calc::nearestPosition(){

    std::tuple<std::string, int, int> rslt = calculateNearesPosition();



    // 여기서 화면을 어떻게 출력해아할지...
    return {{std::get<1>(rslt),std::get<2>(rslt)}};
} 


void Calc::DVMPosition(std::string src_id, int coor_x, int coor_y){
    auto& db = InMemoryDB::instance();

    db.positionTable.insert({src_id, coor_x, coor_y});
}

std::string Calc::nearestID(){
    std::tuple<std::string, int, int> rslt = calculateNearesPosition();

    return std::get<0>(rslt);
}






std::tuple<std::string, int, int> Calc::calculateNearesPosition(){
    auto& db = InMemoryDB::instance();
    std::unordered_map<std::string,Position> positions = db.positionTable.findAll();

    double minDistance = std::numeric_limits<double>::max();
    int nearestX = 0;
    int nearestY = 0;
    std::string src_id;

    // 이 좌표는 추후 시스템에서 얻어오게 수정해야함
    int myX = 1;
    int myY = 1;

    for (const auto& [id, pos] : positions){
        double dx = pos.coor_x - myX;
        double dy = pos.coor_y - myY;
        double distance = std::sqrt(dx*dx +dy*dy);

        if(distance<minDistance){
            minDistance = distance;
            src_id = pos.src_id;
            nearestX = static_cast<int>(pos.coor_x);
            nearestY = static_cast<int>(pos.coor_y);
        }
    }

    return {src_id, nearestX, nearestY};
}