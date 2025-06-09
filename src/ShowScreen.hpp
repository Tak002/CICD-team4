#pragma once
#include <list>

#include "Beverage.hpp"


class ShowScreen {
    private:
        void display(const std::string &str) const;
        void displayPaymentSuccess();
        void displayPaymentFailed();

    public:
        void displayCurrentStock(std::list<Beverage> beverages);
        
        void displayFirstScreen();
        void displayPrepayLocation(int, int);

        void displayEnterCardNum();

        void displayPaymentResultScreen(bool paymentSuccess);


        void displayBeverage(int, int);

        void displayCertCodeEnter();
        void displayCertCodeFailed();

        void displayInvalidIdRange();
        void displayGetCardNum();

        void displayPositionAndCertCode(int x, int y, const std::string &cert_code);
        void displayPrePayCancel();
        void displayPrePayDisavailable();

        void displayInvalidItemNum();
        void displayInvalidArgument();
        void displayInvalidFirstScreenAnswer();
};
