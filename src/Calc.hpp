#pragma once
#include <string>
#include <map>
#include <tuple>

class Calc {
    private:
        std::string nearest_id;
        std::tuple<std::string, int, int> calculateNearesPosition();
    public:
        std::map<int, int> nearestPosition();
        void DVMPosition(std::string, int, int);
        std::string nearestID();    
};
