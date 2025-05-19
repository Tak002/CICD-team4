#pragma once
#include "Bank.hpp"
#include "Beverage.hpp"
#include "CertCode.hpp"
#include "CertCodeManager.hpp"
#include "MSG.hpp"
#include "ShowScreen.hpp"
#include "Stock.hpp"
#include "enum/CommandType.hpp"
#include "Input.hpp"
#include <string>

class Controller {
private: 
    Bank bank;
    CertCode certCode;
    CertCodeManager certCodeManager;
    MSG msg;
    Stock stock;
    ShowScreen showScreen;
    Input input;
    bool isPrepayment;

public:
    void run();

    // 처음 시작시 설정
    void setController();
    


    // 선결제 과정에서 결제가 이뤄진 후 제공 자판기에 메시지 전송
    // return이 false시 rollback
    bool sendPrePaymentMSG(const std::string& cert_code);

};
