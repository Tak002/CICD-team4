#pragma once
#include <list>

#include "Beverage.hpp"

using namespace std;

class ShowScreen {
    private:
        void display(string str);
        void returnToFirstScreen();

    public:
        void displayCurrentStock(list<Beverage> beverages);
        void displayFirstScreen();
        void displayPrepayLocation(int, int);

        void displayEnterCardNum();

        void displayPaymentSuccess();
        void displayPaymentFailed();

        void displayCertCodeEnter();
        void displayCertCodeFailed();

};
