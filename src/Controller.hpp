#pragma once
#include "Bank.hpp"
#include "Beverage.hpp"
#include "CertCode.hpp"
#include "CertCodeManager.hpp"
#include "MSG.hpp"
#include "ShowScreen.hpp"
#include "Stock.hpp"
#include "Input.hpp"
#include "enum/BeverageType.hpp"
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
    int itemID;
    int itemNum;
    string src_id;
    int coor_x;
    int coor_y;

public:
    void run();

    // 처음 시작시 설정
    void createTestData();
    void setController();
    void handlePurchaseFlow();
    void handlePrepaidCollection();
};
