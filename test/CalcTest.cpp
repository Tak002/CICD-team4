#include "../src/Calc.hpp"
#include "../src/database/InMemoryDB.hpp"
#include <gtest/gtest.h>

class CalcTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 매 테스트 전 DB 초기화
        auto& db = InMemoryDB::instance();
        db.initPosition();

        // 예시 위치 데이터 삽입
        calc.DVMPosition("A", 5, 5);
        calc.DVMPosition("B", 2, 1);
        calc.DVMPosition("C", 10, 10);
    }

    Calc calc;
};

// nearestID() 테스트
TEST_F(CalcTest, TestNearestID) {
    std::string nearest = calc.nearestID();
    EXPECT_EQ(nearest, "B"); // (2,1)이 (1,1)에 가장 가까움
}

// nearestPosition() 테스트
TEST_F(CalcTest, TestNearestPositionCoordinates) {
    std::map<int, int> pos = calc.nearestPosition();
    EXPECT_EQ(pos.begin()->first, 2);   // x
    EXPECT_EQ(pos.begin()->second, 1);  // y
}

// DVMPosition 삽입 동작 테스트
TEST_F(CalcTest, TestInsertPosition) {
    auto& db = InMemoryDB::instance();
    calc.DVMPosition("D", 7, 7);
    auto* pos = db.positionTable.find("D");

    ASSERT_NE(pos, nullptr);
    EXPECT_EQ(pos->coor_x, 7);
    EXPECT_EQ(pos->coor_y, 7);
}