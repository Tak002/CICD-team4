// Stock.cpp
#include "Stock.hpp"
#include <list>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>


#include "Beverage.hpp"
#include "MSG.hpp"


#include "../include/nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;

//확정
void Stock::updateStock(int item_code, int item_num)
{
    ostringstream oss;
    oss << "item" << item_code << ".json";
    string filename = oss.str();

    std::ifstream ifle(filename);
    if (!ifle.is_open())
    {
        std::cerr << "파일 열기 실패: " << filename << std::endl;
        return;
    }


    json js;
    try
    {
        ifle >> js;
        ifle.close();

        int current_num = js["item_num"];

        js["item_num"] = current_num - item_num;
        std::ofstream ofle(filename);
        if (!ofle.is_open())
        {
            std::cerr << "파일 열기 실패 (쓰기): " << filename << std::endl;
            return;
        }

        ofle << js.dump(2);
        ofle.close();
        filesystem::remove("orderItem.json");
    }
    catch (const std::exception &e)
    {
        std::cerr << "JSON 파싱 오류: " << e.what() << std::endl;
    }
}

//확정
list<Beverage> Stock::getCurrentStock()
{
    list<Beverage> beverages;
    for (int i = 1; i <= 20; i++)
    {
        string filename;
        ostringstream oss;
        oss << "item" << i << ".json";
        filename.append(oss.str());
        std::ifstream ifle(filename);

        if (!ifle.is_open())
        {
            // 파일 존재 안함
            continue;
        }

        json js;
        try
        {
            ifle >> js;

            int item_code = js["item_code"];
            int item_num = js["item_num"];
            int price = js["item_price"];

            Beverage bev(item_code, item_num, price);
            beverages.push_back(bev);
        }catch(const std::exception& e){
            cerr << "Error parsing " << filename << ": " << e.what() << std::endl;
        }

    }

    return beverages;
}


// 확정
bool Stock::isPrepayment(int item_code, int item_num)
{
    list<Beverage> beverage_list = getCurrentStock();
    for(Beverage bev : beverage_list){
        if(bev.isSameId(item_code)){
            json j;
            j["item_code"] = item_code;
            j["item_num"] = item_num;
            j["item_price"] = bev.getPrice();

            ofstream o("orderItem.json");
            o << j.dump(3);
            if(bev.isEnough(item_num)){
                return false;
            }
        }
    }

    return true;
}

//확정
bool Stock::isBuyable(std::string cert_code, int item_code, int item_num)
{
    list<Beverage> beverage_list = getCurrentStock();
    
    for (Beverage bev : beverage_list) {
        if (bev.isSameId(item_code)&&bev.isEnough(item_num)) {
                // JSON 생성
                json j;
                j["item_code"] = item_code;
                j["item_num"] = item_num;
                j["cert_code"] = cert_code;

                // 파일명 생성
                std::ostringstream oss;
                oss << "certCode" << cert_code << ".json";
                std::string filename = oss.str();
                // 파일로 저장
                std::ofstream ofile(filename);
                if (ofile.is_open()) {
                    ofile << j.dump(4);  // 들여쓰기 4칸
                    ofile.close();
                    updateStock(item_code, item_num); // 재고 업데이트
                    return true;
                } else {
                    std::cerr << "Failed to open file: " << filename << std::endl;
                }   
        }
    }
    return false;
}
//확정
void Stock::requestBeverage(int item_code, int item_num)
{
    updateStock(item_code, item_num);
}
