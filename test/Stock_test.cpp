#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <sstream>
#include "../src/Stock.hpp"
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

// ---------- 테스트 유틸 ----------

void createItemFile(int code, int num, int price) {
    ostringstream oss;
    oss << "item" << code << ".json";
    ofstream ofile(oss.str());
    ofile << "{ \"item_code\": " << code << ", \"item_num\": " << num << ", \"item_price\": " << price << " }";
    ofile.close();
}

void removeItemFile(int code) {
    ostringstream oss;
    oss << "item" << code << ".json";
    std::filesystem::remove(oss.str());
}

void removeOrderItemFile() {
    std::filesystem::remove("orderItem.json");
}

void removeCertCodeFile(const string& cert_code) {
    ostringstream oss;
    oss << "certCode" << cert_code << ".json";
    std::filesystem::remove(oss.str());
}

// ---------- 단위 테스트 ----------

/*
TEST(StockTest, GetCurrentStock_NormalFiles_ReturnsCorrectBeverages) {
    createItemFile(1, 5, 1000);
    createItemFile(2, 0, 800);

    Stock stock;
    auto beverages = stock.getCurrentStock();

    int count = 0;
    for (const auto& bev : beverages) {
        if (bev.getBevCode() == 1) {
            EXPECT_EQ(bev.getBevNum(), 5);
            EXPECT_EQ(bev.getPrice(), 1000);
        } else if (bev.getBevCode() == 2) {
            EXPECT_EQ(bev.getBevNum(), 0);
            EXPECT_EQ(bev.getPrice(), 800);
        }
        count++;
    }
    EXPECT_EQ(count, 2);

    removeItemFile(1);
    removeItemFile(2);
}

TEST(StockTest, GetCurrentStock_NoFiles_ReturnsEmptyList) {
    Stock stock;
    auto beverages = stock.getCurrentStock();
    EXPECT_TRUE(beverages.empty());
}

*/
TEST(StockTest, IsPrepayment_EnoughStock_ReturnsFalseAndCreatesOrderItem) {
    createItemFile(3, 10, 1200);

    Stock stock;
    bool result = stock.isPrepayment(3, 5);
    EXPECT_FALSE(result);

    ifstream in("orderItem.json");
    ASSERT_TRUE(in.is_open());
    json j; in >> j;
    EXPECT_EQ(j["item_code"], 3);
    EXPECT_EQ(j["item_num"], 5);
    EXPECT_EQ(j["item_price"], 1200);
    in.close();

    removeItemFile(3);
    removeOrderItemFile();
}

TEST(StockTest, IsPrepayment_NotEnoughStock_ReturnsTrue) {
    createItemFile(4, 1, 1300);

    Stock stock;
    bool result = stock.isPrepayment(4, 5);
    EXPECT_TRUE(result);

    removeItemFile(4);
    removeOrderItemFile();
}

TEST(StockTest, IsBuyable_EnoughStock_CreatesCertCodeFileAndReturnsTrue) {
    createItemFile(5, 10, 900);
    string cert = "CERTX";

    Stock stock;
    bool result = stock.isBuyable(cert, 5, 6);
    EXPECT_TRUE(result);

    ostringstream oss;
    oss << "certCode" << cert << ".json";
    ifstream in(oss.str());
    ASSERT_TRUE(in.is_open());
    json j; in >> j;
    EXPECT_EQ(j["item_code"], 5);
    EXPECT_EQ(j["item_num"], 6);
    EXPECT_EQ(j["cert_code"], cert);

    in.close();
    removeItemFile(5);
    removeCertCodeFile(cert);
}

TEST(StockTest, IsBuyable_NotEnoughStock_ReturnsFalseNoFile) {
    createItemFile(6, 1, 1000);
    string cert = "NOFILE";

    Stock stock;
    bool result = stock.isBuyable(cert, 6, 3);
    EXPECT_FALSE(result);

    ostringstream oss;
    oss << "certCode" << cert << ".json";
    ifstream in(oss.str());
    EXPECT_FALSE(in.is_open());

    removeItemFile(6);
}

TEST(StockTest, RequestBeverage_DecreasesStock) {
    createItemFile(7, 10, 2000);

    Stock stock;
    stock.requestBeverage(7, 4); // 10 → 6

    ifstream in("item7.json");
    json j; in >> j;
    EXPECT_EQ(j["item_num"], 6);
    in.close();

    removeItemFile(7);
}

TEST(StockTest, RequestBeverage_FileNotExist_NoCrash) {
    Stock stock;
    stock.requestBeverage(999, 1); // 존재하지 않아도 crash 없어야 함
}

TEST(StockTest, RequestBeverage_DecreaseToNegative) {
    createItemFile(8, 3, 1500);
    Stock stock;
    stock.requestBeverage(8, 5); // 3 - 5 = -2

    ifstream in("item8.json");
    json j; in >> j;
    EXPECT_EQ(j["item_num"], -2);
    in.close();

    removeItemFile(8);
}