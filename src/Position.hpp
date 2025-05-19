#pragma once
#include <string>
#include <map>
#include <tuple>

class Position {
    private:
        int myX = 0;
        int myY = 0;
    public:
        float calcDistance(int, int);
};
