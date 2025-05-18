#include "../src/Beverage.hpp"
#include <gtest/gtest.h>

class BeverageTest : public ::testing::Test {
protected:
    Beverage bev;

    void SetUp() override {
        // 비공개 멤버 초기화를 위해 reinterpret_cast 사용
        auto ptr = reinterpret_cast<int*>(
            reinterpret_cast<char*>(&bev) + offsetof(Beverage, bevId));
        *ptr = 42;

        ptr = reinterpret_cast<int*>(
            reinterpret_cast<char*>(&bev) + offsetof(Beverage, bevQuantity));
        *ptr = 10;
    }
};

TEST_F(BeverageTest, IsSameIdReturnsTrueForSameId) {
    EXPECT_TRUE(bev.isSameId(42));
}

TEST_F(BeverageTest, IsSameIdReturnsFalseForDifferentId) {
    EXPECT_FALSE(bev.isSameId(99));
}

TEST_F(BeverageTest, IsEnoughReturnsTrueWhenQuantitySufficient) {
    EXPECT_TRUE(bev.isEnough(5));
}

TEST_F(BeverageTest, IsEnoughReturnsFalseWhenQuantityInsufficient) {
    EXPECT_FALSE(bev.isEnough(15));
}

TEST_F(BeverageTest, ReduceBeverageReducesQuantity) {
    bev.reduceBeverage(3);
    EXPECT_EQ(bev.getBevQuantity(), 7);
}

TEST_F(BeverageTest, ReduceBeverageDoesNotGoNegative) {
    bev.reduceBeverage(15);
    EXPECT_EQ(bev.getBevQuantity(), 10); // 변화 없음
}

TEST_F(BeverageTest, GetBevIdReturnsCorrectId) {
    EXPECT_EQ(bev.getBevId(), 42);
}

TEST_F(BeverageTest, GetBevQuantityReturnsCorrectQuantity) {
    EXPECT_EQ(bev.getBevQuantity(), 10);
}