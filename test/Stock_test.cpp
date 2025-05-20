#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include <filesystem>
#include "../src/Stock.hpp"
#include "nlohmann/json.hpp"

using namespace std;

// ---------- 테스트를 위한 유틸 함수 ----------
// 테스트용 음료 파일 생성
void createItemFile(int code, int num, int price) {
    std::ostringstream oss;
    oss << "item" << code << ".json";
    std::ofstream ofile(oss.str());
    ofile << "{ \"item_code\": " << code << ", \"item_num\": " << num << ", \"item_price\": " << price << " }";
    ofile.close();
}

// 테스트용 음료 파일 삭제
void removeItemFile(int code) {
    std::ostringstream oss;
    oss << "item" << code << ".json";
    std::filesystem::remove(oss.str());
}

// orderItem.json 삭제
void removeOrderItemFile() {
    std::filesystem::remove("orderItem.json");
}

// certCodeXXX.json 삭제
void removeCertCodeFile(const std::string& cert_code) {
    std::ostringstream oss;
    oss << "certCode" << cert_code << ".json";
    std::filesystem::remove(oss.str());
}

// ---------- 단위 테스트 ----------

// getCurrentStock: 정상 파일로부터 음료 리스트 반환
TEST(StockTest, GetCurrentStock_NormalFiles_ReturnsCorrectBeverages) {
    createItemFile(1, 7, 900);
    createItemFile(2, 0, 800);
    Stock stock;

    auto beverages = stock.getCurrentStock();

    // 2개의 음료 파일이 있으므로 2개가 반환돼야 함
    int cnt = 0;
    for (const auto& b : beverages) {
        if (b.getBevCode() == 1) {
            EXPECT_EQ(b.getBevNum(), 7);
            EXPECT_EQ(b.getPrice(), 900);
        } else if (b.getBevCode() == 2) {
            EXPECT_EQ(b.getBevNum(), 0);
            EXPECT_EQ(b.getPrice(), 800);
        }
        cnt++;
    }
    EXPECT_EQ(cnt, 2);

    removeItemFile(1);
    removeItemFile(2);
}

// getCurrentStock: 파일이 아예 없을 때
TEST(StockTest, GetCurrentStock_NoFiles_ReturnsEmptyList) {
    Stock stock;
    auto beverages = stock.getCurrentStock();
    EXPECT_TRUE(beverages.empty());
}

// isPrepayment: 재고 충분하면 false 반환 + orderItem.json 정상 생성
TEST(StockTest, IsPrepayment_EnoughStock_ReturnsFalseAndCreatesOrderItem) {
    createItemFile(3, 5, 1200);
    Stock stock;
    bool result = stock.isPrepayment(3, 3); // 재고 5개, 요청 3개

    EXPECT_FALSE(result); // 선결제 아님
    // orderItem.json이 생성되었는지 체크
    std::ifstream ifile("orderItem.json");
    EXPECT_TRUE(ifile.is_open());

    // 내용 확인
    nlohmann::json js;
    ifile >> js;
    EXPECT_EQ(js["item_code"], 3);
    EXPECT_EQ(js["item_num"], 3);
    EXPECT_EQ(js["item_price"], 1200);

    ifile.close();
    removeItemFile(3);
    removeOrderItemFile();
}

// isPrepayment: 재고 부족하면 true 반환
TEST(StockTest, IsPrepayment_NotEnoughStock_ReturnsTrue) {
    createItemFile(5, 2, 1100);
    Stock stock;
    bool result = stock.isPrepayment(5, 5); // 재고 2개, 요청 5개

    EXPECT_TRUE(result); // 선결제 필요
    removeItemFile(5);
    removeOrderItemFile();
}

// isBuyable: 재고 충분하면 certCode파일 생성 & true 반환
TEST(StockTest, IsBuyable_EnoughStock_CreatesCertCodeFileAndReturnsTrue) {
    createItemFile(7, 8, 1000);
    Stock stock;
    std::string cert = "TESTCERT1";
    bool result = stock.isBuyable(cert, 7, 5); // 8개 중 5개

    EXPECT_TRUE(result);

    // 파일 내용 확인
    std::ostringstream oss;
    oss << "certCode" << cert << ".json";
    std::ifstream ifile(oss.str());
    ASSERT_TRUE(ifile.is_open());
    nlohmann::json js;
    ifile >> js;
    EXPECT_EQ(js["item_code"], 7);
    EXPECT_EQ(js["item_num"], 5);
    EXPECT_EQ(js["cert_code"], cert);

    ifile.close();
    removeItemFile(7);
    removeCertCodeFile(cert);
}

// isBuyable: 재고 부족 시 false, 파일 생성 안 됨
TEST(StockTest, IsBuyable_NotEnoughStock_ReturnsFalseNoFile) {
    createItemFile(8, 1, 500);
    Stock stock;
    std::string cert = "TESTCERT2";
    bool result = stock.isBuyable(cert, 8, 2); // 1개 중 2개 요청

    EXPECT_FALSE(result);
    std::ostringstream oss;
    oss << "certCode" << cert << ".json";
    std::ifstream ifile(oss.str());
    EXPECT_FALSE(ifile.is_open());

    removeItemFile(8);
}

// updateStock: 재고 차감 정상 동작 (requestBeverage에서 호출)
TEST(StockTest, RequestBeverage_DecreasesStock) {
    createItemFile(12, 10, 2000);
    Stock stock;
    stock.requestBeverage(12, 4); // 10->6
    // 파일에서 확인
    std::ostringstream oss;
    oss << "item" << 12 << ".json";
    std::ifstream ifile(oss.str());
    nlohmann::json js;
    ifile >> js;
    EXPECT_EQ(js["item_num"], 6);

    ifile.close();
    removeItemFile(12);
}

// updateStock: 파일이 없을 때 (예외 상황)
TEST(StockTest, RequestBeverage_FileNotExist_NoCrash) {
    Stock stock;
    // 파일이 없으면 그냥 에러메시지만 출력되고, 크래시 안 남
    // 정상 종료되면 통과
    stock.requestBeverage(55, 2);
}

// updateStock: 재고 차감시 음수가 되면 음수 저장 (엣지케이스)
TEST(StockTest, RequestBeverage_DecreaseToNegative) {
    createItemFile(14, 3, 400);
    Stock stock;
    stock.requestBeverage(14, 7); // 3-7 = -4
    std::ostringstream oss;
    oss << "item" << 14 << ".json";
    std::ifstream ifile(oss.str());
    nlohmann::json js;
    ifile >> js;
    EXPECT_EQ(js["item_num"], -4); // 음수도 저장됨 (현행 로직상)

    ifile.close();
    removeItemFile(14);
}

