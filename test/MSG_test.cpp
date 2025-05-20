#include <gtest/gtest.h>
#include "../src/MSG.hpp"

class MSGTest : public ::testing::Test
{
protected:
    MSG msg;
};
/*
1. request json 메시지를 정상적으로 받는지 확인 서버가 켜져있고 다른 클라이언트가 접속해서 소통하는지 확인
2. response json 메시지를 받은 메시지 종류에 따라 ACK를 다르게 보내는지 확인
*/

// 재고가 없는 상황을 가정해 다른 DVM에 메시지를 전송했을 때 올바른 튜플을 반환하는지 테스트
TEST_F(MSGTest, DVMMessageOutofStock_ReturnsCorrectTuple)
{
    int beverageId = 10;
    int quantity = 5;

    auto result = msg.DVMMessageOutofStock(beverageId, quantity);

    int x = std::get<0>(result);
    int y = std::get<1>(result);
    std::string id = std::get<2>(result);

    // 단순 타입 검사 및 값 확인
    EXPECT_GE(x, 0); // x 좌표는 가장 가까운 좌표여야 함   
    EXPECT_GE(y, 0); // y 좌표는 가장 가까운 좌표여야 함
    EXPECT_FALSE(id.empty()); // ID는 비어있지 않아야 함 
}

// 재고 확인 요청 메시지를 생성했을 때 올바른 JSON 형식으로 반환되는지 테스트
TEST_F(MSGTest, AskStockMessage_ReturnsValidJson)
{
    json input = {
        {"src_id", "T2"},
        {"msg_content", {{"item_code", "10"}, {"item_num", 12}}}};

    json result = msg.AskStockMessage(input);

    EXPECT_TRUE(result.contains("src_id"));
    EXPECT_TRUE(result.contains("msg_content"));
    EXPECT_TRUE(result["msg_content"].contains("is_enough")); // 예상 반환 필드
}
