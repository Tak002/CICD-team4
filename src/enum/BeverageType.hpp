#pragma once

#include <string>

enum class BeverageType {
    Cola,               // 콜라
    Cider,              // 사이다
    GreenTea,           // 녹차
    BlackTea,           // 홍차
    MilkTea,            // 밀크티
    SparklingWater,     // 탄산수
    BarleyTea,          // 보리차
    CanCoffee,          // 캔커피
    Water,              // 물
    CitronTea,          // 유자차
    EnergyDrink,        // 에너지 드링크
    Sikhye,             // 식혜
    StrawberryJuice,    // 딸기주스
    OrangeJuice,        // 오렌지 주스
    GrapeJuice,         // 포도 주스
    IonDrink,           // 이온 음료
    Americano,          // 아메리카노
    HotChoco,           // 핫초코
    CafeLatte,          // 카페라떼
    COUNT               // (개수용)
};

// 문자열 변환 함수 선언
std::string toString(BeverageType type);