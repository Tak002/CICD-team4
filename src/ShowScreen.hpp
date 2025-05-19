#pragma once
#include <list>

#include "Beverage.hpp"

using namespace std;

class ShowScreen {
    private:
        void display(string str);
        void displayPaymentSuccess();
        void displayPaymentFailed();

    public:
        void displayCurrentStock(list<Beverage> beverages);
        
        void displayFirstScreen();
        void displayPrepayLocation(int, int);

        void displayEnterCardNum();

        void displayPaymentResultScreen(bool paymentSuccess);


        void displayBeverage(int, int);

        void displayCertCodeEnter();
        void displayCertCodeFailed();

        void displayInvalidIdRange();
        void displayGetCardNum();

        void displayPositionAndCertCode(int x, int y, std::string cert_code);
        void displayPrePayCancel();
};
