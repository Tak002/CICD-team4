#pragma once
#include <list>

#include "Beverage.hpp"

using namespace std;

class showScreen {
    private:
        void display();
        void returnToFirstScreen();

    public:
        void displayCurrentStock(list<Beverage> beverages);
        

};
