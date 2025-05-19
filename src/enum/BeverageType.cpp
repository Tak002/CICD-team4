#include "BeverageType.hpp"

std::string toString(BeverageType type) {
    switch (type) {
        case BeverageType::Cola: return "콜라";
        case BeverageType::Cider: return "사이다";
        case BeverageType::GreenTea: return "녹차";
        case BeverageType::BlackTea: return "홍차";
        case BeverageType::MilkTea: return "밀크티";
        case BeverageType::SparklingWater: return "탄산수";
        case BeverageType::BarleyTea: return "보리차";
        case BeverageType::CanCoffee: return "캔커피";
        case BeverageType::Water: return "물";
        case BeverageType::CitronTea: return "유자차";
        case BeverageType::EnergyDrink: return "에너지 드링크";
        case BeverageType::Sikhye: return "식혜";
        case BeverageType::StrawberryJuice: return "딸기주스";
        case BeverageType::OrangeJuice: return "오렌지 주스";
        case BeverageType::GrapeJuice: return "포도 주스";
        case BeverageType::IonDrink: return "이온 음료";
        case BeverageType::Americano: return "아메리카노";
        case BeverageType::HotChoco: return "핫초코";
        case BeverageType::CafeLatte: return "카페라떼";
        default: return "Unknown";
    }
}
int toPrice(BeverageType type) {
    switch (type) {
        case BeverageType::Cola: return 1500;
        case BeverageType::Cider: return 1500;
        case BeverageType::GreenTea: return 1700;
        case BeverageType::BlackTea: return 1700;
        case BeverageType::MilkTea: return 2000;
        case BeverageType::SparklingWater: return 1400;
        case BeverageType::BarleyTea: return 1300;
        case BeverageType::CanCoffee: return 1500;
        case BeverageType::Water: return 1000;
        case BeverageType::EnergyDrink: return 2200;
        case BeverageType::CitronTea: return 1800;
        case BeverageType::Sikhye: return 1600;
        case BeverageType::IcedTea: return 1000;
        case BeverageType::StrawberryJuice: return 2000;
        case BeverageType::OrangeJuice: return 2000;
        case BeverageType::GrapeJuice: return 2000;
        case BeverageType::IonDrink: return 1700;
        case BeverageType::Americano: return 1900;
        case BeverageType::HotChoco: return 1800;
        case BeverageType::CafeLatte: return 2100;
        default: return 0;
    }
}
int getPrice(int bev_id){
    BeverageType bevType =  static_cast<BeverageType>(bev_id-1);
    return toPrice(bevType);
}