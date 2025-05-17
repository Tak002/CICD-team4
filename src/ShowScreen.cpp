
// ShowScreen.cpp

#include <list>
#include <string>
#include <iterator>
#include <iostream>

#include "ShowScreen.hpp"
#include "enum/BeverageType.hpp"

using namespace std;

void ShowScreen::display(string str){
    
    cout<<"-------------------------------"<<endl;
    cout<<str<<endl;
    cout<<"-------------------------------"<<endl;
}

void ShowScreen::returnToFirstScreen(){
    displayFirstScreen();
}



void ShowScreen::displayCurrentStock(std::list<Beverage> beverages) {
    std::string str;

    for (int i = 0; i < static_cast<int>(BeverageType::COUNT); i++) {
        auto it = beverages.begin();
        std::advance(it, i);  // i번째 요소

        int code = it->getBevId();
        int quantity = it->getBevQuantity();
        BeverageType type = static_cast<BeverageType>(i);

        // 항목 출력 형식
        str += "(" + std::to_string(code+1) + ") " + toString(type) + " : " + std::to_string(quantity) + "개 ";

        // 10개마다 줄바꿈
        if ((i + 1) % 10 == 0) {
            str += "\n";
        }
    }

    display(str);
}

void ShowScreen::displayFirstScreen(){
    string str;
    str += std::string("어서오세요.") + "\n";
    str += std::string("시작하려면 0을 눌러주세요.") + "\n";
    str += std::string("(선결제 인증코드 입력을 원하시면 인증코드를 입력하세요...)");

    display(str);
}

void ShowScreen::displayPrepayLocation(int coor_x, int coor_y){
    string str;
    str +=string("목표 위치 x: %d / y: %d",coor_x,coor_y);

    display(str);
}

void ShowScreen::displayEnterCardNum(){
    string str = "카드 번호를 입력해주세요.";

    display(str);
}

void  ShowScreen::displayPaymentSuccess(){
    string str = "결제에 성공했습니다";
}

void ShowScreen::displayPaymentFailed(){
    string str = "결제에 실패했습니다.\n";
    str+= "카드번호나 잔액을 확인해주세요";
    
}

void ShowScreen::displayCertCodeEnter(){
    string str = "인증코드를 입력하세요";
    display(str);
}
void ShowScreen::displayCertCodeFailed(){
    string str = "유효하지 않은 인증코드 입니다.";
    display(str);
}