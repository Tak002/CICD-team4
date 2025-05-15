#pragma once

#include <string>

class Controller {
public:
    // 재고를 출력하는 함수
    std::string printStock();

    // 음료의 재고를 확인하는 함수
    bool checkBeverage(int item_code, int item_num);

    // 가장 가까운 자판기 위치 정보를 출력
    std::string printNearestPos(const std::string& DVMID);

    // 선결제 과정에서 결제가 이뤄진 후 제공 자판기에 메시지 전송
    // return이 false시 rollback
    bool sendPrePaymentMSG(const std::string& cert_code);

    // 인증 코드의 유효성 검사
    bool isValidCertCode(const std::string& cert_code);

    // bank에 결제 요청 전달
    bool requestPayment(int card_num, int price);
};
