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