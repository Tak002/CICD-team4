// Controller.cpp
#include "Controller.hpp"
#include <list>



//  초기설정. stock, msg 설정, 미구현
void Controller:: setController(){};

void Controller:: runShowScreenCommand(CommandType cmd){
     CommandType type = cmd;

    switch (type) {
        case CommandType::DISPLAY_CURRENT_STOCK:
            showScreen.displayCurrentStock(stock.getCurrentStock());
            break;
        case CommandType::DISPLAY_FIRST_SCREEN:
            showScreen.displayFirstScreen();
            break;
            // showScreen.displayPrepayLocation(coor_x :int ,coor_y : int); 
            break;
        case CommandType::DISPLAY_ENTER_CARD_NUM:
            showScreen.displayEnterCardNum();
            break;
        case CommandType::DISPLAY_PAYMENT_RESULT_SCREEN:
            // showScreen.displayPaymentResultScreen(paymentSuccess : bool);
            break;
        case CommandType::DISPLAY_BEVERAGE:
            // showScreen.displayBeverage(item_id: int, item_num :int);
            break;
        case CommandType::DISPLAY_CERT_CODE_ENTER:
            showScreen.displayCertCodeEnter();
            break;
        case CommandType::DISPLAY_CERT_CODE_FAILED:
            showScreen.displayCertCodeFailed();
            break;
    }
}

void Controller::printStock(){
    showScreen.displayCurrentStock(stock.getCurrentStock());
}

bool Controller::checkBeverage(int item_code, int item_num){
    return stock.isPrepayment(item_code,item_num);
}


// 선결제 과정에서 결제가 이뤄진 후 제공 자판기에 메시지 전송
// return이 false시 rollback
bool Controller::sendPrePaymentMSG(const std::string& cert_code){
    // Require cert_code to MESSAGE method
    // return msg.sendMessage(SENDING MESSAGE);  
}

// 인증 코드의 유효성 검사
bool Controller::isValidCertCode(const std::string& cert_code){
    // Require CertCode Manager Class interface
    // return certCodeManager.isValidCertCode(cert_code);
}

// bank에 결제 요청 전달
bool Controller::requestPayment(int card_num, int price){
    return bank.reqeustPayment(card_num,price);
}