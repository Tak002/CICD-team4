#include <gtest/gtest.h>
#include "../src/Beverage.hpp"

// ========== 테스트 목적 ==========
// - isSameId : bev_code와 입력 id가 같은지 검사
// - isEnough : 음료 재고(bev_num)가 요청 수량 이상인지 검사
// - reduceBeverage : bev_num에서 지정 수만큼 차감 (재고 부족시 차감X)
// - getBevCode/getBevNum/getPrice : 멤버값 getter

// ========== 정상 동작 테스트 ==========

// isSameId: bev_code가 같은 경우 true 반환
TEST(BeverageTest, IsSameId_SameId_ReturnsTrue) {
    Beverage b(10, 5, 1000); // id=10
    EXPECT_TRUE(b.isSameId(10));
}

// isSameId: bev_code가 다를 때 false 반환
TEST(BeverageTest, IsSameId_DifferentId_ReturnsFalse) {
    Beverage b(10, 5, 1000);
    EXPECT_FALSE(b.isSameId(7));
}

// isEnough: 충분한 재고일 때 true 반환
TEST(BeverageTest, IsEnough_Enough_ReturnsTrue) {
    Beverage b(20, 10, 1200);
    EXPECT_TRUE(b.isEnough(10)); // 재고=10, 요청=10
    EXPECT_TRUE(b.isEnough(5));  // 재고=10, 요청=5
}

// isEnough: 재고 부족일 때 false 반환
TEST(BeverageTest, IsEnough_NotEnough_ReturnsFalse) {
    Beverage b(20, 3, 1500);
    EXPECT_FALSE(b.isEnough(4)); // 재고=3, 요청=4
    EXPECT_FALSE(b.isEnough(100));
}

// reduceBeverage: 재고 충분하면 수량이 줄어드는지
TEST(BeverageTest, ReduceBeverage_EnoughStock_DecreasesStock) {
    Beverage b(3, 10, 500);
    b.reduceBeverage(4); // 10->6
    EXPECT_EQ(b.getBevNum(), 6);
}

// reduceBeverage: 재고 부족시 수량 변화 없음
TEST(BeverageTest, ReduceBeverage_NotEnoughStock_NoChange) {
    Beverage b(3, 2, 500);
    b.reduceBeverage(5); // 2->2 (차감안됨)
    EXPECT_EQ(b.getBevNum(), 2);
}

// getBevCode/getBevNum/getPrice 정상 반환값
TEST(BeverageTest, GetterFunctions_ReturnCorrectValues) {
    Beverage b(7, 8, 3000);
    EXPECT_EQ(b.getBevCode(), 7);
    EXPECT_EQ(b.getBevNum(), 8);
    EXPECT_EQ(b.getPrice(), 3000);
}

// ========== 비정상 입력/엣지 케이스 테스트 ==========

// isEnough: 음수 요청 (정상적 사용은 아니지만, 구현상 true로 동작)
TEST(BeverageTest, IsEnough_NegativeRequest_ReturnsTrue) {
    Beverage b(1, 2, 100);
    EXPECT_TRUE(b.isEnough(-1)); // -1 <= 2, true
}

// reduceBeverage: 0개 또는 음수 개수 요청 시 (변화 없음)
// failded
TEST(BeverageTest, ReduceBeverage_ZeroOrNegativeRequest_NoChange) {
    Beverage b(4, 7, 800);
    b.reduceBeverage(0); // 7->7
    EXPECT_EQ(b.getBevNum(), 7);
    b.reduceBeverage(-2); // 7->7
    EXPECT_EQ(b.getBevNum(), 7);
}

// 생성자: 기본값 체크
TEST(BeverageTest, DefaultConstructor_InitializesZero) {
    Beverage b;
    EXPECT_EQ(b.getBevCode(), 0);
    EXPECT_EQ(b.getBevNum(), 0);
    // price는 명시적으로 0으로 초기화하지 않았으나, 생성자에서 보장 필요
}

// price: 2매개 생성자 사용시 price=0
TEST(BeverageTest, Constructor_TwoArgs_PriceZero) {
    Beverage b(10, 5);
    EXPECT_EQ(b.getPrice(), 0);
}

// 엣지: 최대 int값 요청 시 동작 확인
TEST(BeverageTest, ReduceBeverage_MaxIntRequest_NoChange) {
    int INT_MAX = 2147483647;
    Beverage b(5, 3, 500);
    b.reduceBeverage(INT_MAX); // 재고보다 크므로 변화 없음
    EXPECT_EQ(b.getBevNum(), 3);
}

