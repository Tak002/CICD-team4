#include <gtest/gtest.h>
#include "../src/Bank.hpp"
#include <fstream>
#include "../include/nlohmann/json.hpp"

using json = nlohmann::json;
using namespace std;

// 카드 초기 상태를 세팅하는 함수 (테스트 독립성 보장)
void resetCardJson(int card_num = 12345, int balance = 10000) {
    json js;
    js["card_num"] = card_num;
    js["balance"] = balance;
    ofstream o("card.json");
    o << js.dump(2);
}


// Bank::reqeustPayment: 결제 성공 시 잔액 감소, 실패 시 유지
TEST(BankTest, RequestPayment_SuccessfulPayment) {
    resetCardJson(12345, 10000);
    Bank bank;
    EXPECT_TRUE(bank.reqeustPayment(12345, 5000));

    // 잔액이 실제로 차감되었는지 확인
    ifstream i("card.json");
    json js;
    i >> js;
    EXPECT_EQ(js["balance"], 5000);
}

TEST(BankTest, RequestPayment_FailedPayment_NoChange) {
    resetCardJson(12345, 4000);
    Bank bank;
    EXPECT_FALSE(bank.reqeustPayment(12345, 5000));

    // 잔액이 변하지 않았는지 확인
    ifstream i("card.json");
    json js;
    i >> js;
    EXPECT_EQ(js["balance"], 4000);
}


// Bank::rollback: 금액 복구 테스트
TEST(BankTest, Rollback_IncreaseBalance) {
    resetCardJson(12345, 5000);
    Bank bank;
    bank.rollback(12345, 2000);

    ifstream i("card.json");
    json js;
    i >> js;
    EXPECT_EQ(js["balance"], 7000);
}

// Bank::rollback 후 reqeustPayment 가능 여부
TEST(BankTest, Rollback_ThenPayment) {
    resetCardJson(12345, 2000);
    Bank bank;
    bank.rollback(12345, 3000); // +3000 → 5000

    EXPECT_TRUE(bank.reqeustPayment(12345, 5000)); // 정확히 맞으면 결제 가능
}
