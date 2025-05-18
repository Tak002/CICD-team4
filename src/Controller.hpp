#pragma once
#include "Bank.hpp"
#include "Beverage.hpp"
#include "CertCode.hpp"
#include "CertCodeManager.hpp"
#include "MSG.hpp"
#include "ShowScreen.hpp"
#include "Stock.hpp"
#include "enum/CommandType.hpp"

#include <string>

class Controller {
private: 
    Bank bank;
    CertCode certCode;
    CertCodeManager certCodeManager;
    MSG msg;
    Stock stock;
    ShowScreen showScreen;

public:
    // 처음 시작시 설정
    void setController();
    
    // showScreen 실행
    void runShowScreenCommand(CommandType  cmd);

    // 재고를 출력하는 함수
    void printStock();

    bool checkBeverage(int item_code, int item_num);


    // 선결제 과정에서 결제가 이뤄진 후 제공 자판기에 메시지 전송
    // return이 false시 rollback
    bool sendPrePaymentMSG(const std::string& cert_code);

    // 인증 코드의 유효성 검사
    bool isValidCertCode(const std::string& cert_code);

    // bank에 결제 요청 전달
    bool requestPayment(int card_num, int price);
};
