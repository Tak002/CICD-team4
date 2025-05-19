// Position.cpp
#include <string>
#include <unordered_map>
#include <tuple>
#include <limits>   
#include <cmath>    

#include "Position.hpp"






// 확정
float Position::calcDistance(int coor_x, int coor_y){
    double dstnc = std::sqrt((coor_x-myX)*(coor_x-myX) + (coor_y-myY)*(coor_y-myY));
    return dstnc;

}

