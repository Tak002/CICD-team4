// Controller.cpp
#include "Controller.hpp"
#include <list>

void Controller::printStock(){
    // Require ShowScreen Class interface
    // showScreen.displayCurrentStock(stock.getCurrentStock());
}

bool Controller::checkBeverage(int item_code, int item_num){
    return stock.isPrepayment(item_code,item_num);
}


// 선결제 과정에서 결제가 이뤄진 후 제공 자판기에 메시지 전송
// return이 false시 rollback
bool Controller::sendPrePaymentMSG(const std::string& cert_code){
    // Require MSG Class interface
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