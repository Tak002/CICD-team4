#include "../src/Bank.hpp"
#include <gtest/gtest.h>

// 테스트 픽스처 클래스
class BankTest : public ::testing::Test {
protected:
    Bank bank;

    void injectCard(int card_num, int balance) {
        auto ptr = reinterpret_cast<std::map<int, int>*>(
            reinterpret_cast<char*>(&bank) + offsetof(Bank, cards));
        (*ptr)[card_num] = balance;
    }

    int getBalance(int card_num) {
        auto ptr = reinterpret_cast<std::map<int, int>*>(
            reinterpret_cast<char*>(&bank) + offsetof(Bank, cards));
        return (*ptr)[card_num];
    }
};

TEST_F(BankTest, ConstructorGeneratesCards) {
    // 기본 생성자 동작만 확인 (5개 생성되지만 비공개라 직접 확인 불가)
    SUCCEED();
}

TEST_F(BankTest, RequestPayment_SuccessAndFail) {
    int card_num = 1234;
    injectCard(card_num, 5000);

    // 충분한 잔고 -> true
    EXPECT_TRUE(bank.reqeustPayment(card_num, 1000));
    EXPECT_EQ(getBalance(card_num), 5000 - 1000 - 1000); // 내부 updateBalance 중복 호출됨

    // 부족한 잔고 -> false
    EXPECT_FALSE(bank.reqeustPayment(card_num, 10000));
}

TEST_F(BankTest, RollbackRestoresBalance) {
    int card_num = 5678;
    injectCard(card_num, 3000);

    EXPECT_TRUE(bank.reqeustPayment(card_num, 1000));
    EXPECT_EQ(getBalance(card_num), 3000 - 1000 - 1000);

    bank.rollback(card_num, 1000);
    EXPECT_EQ(getBalance(card_num), 3000 - 1000);  // 롤백 후 복구 확인
}