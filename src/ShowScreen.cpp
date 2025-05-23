
// ShowScreen.cpp

#include <list>
#include <string>
#include <iterator>
#include <iostream>
#include <sstream>

#include "Beverage.hpp"
#include "ShowScreen.hpp"
#include "enum/BeverageType.hpp"

using namespace std;

//화면 출력 틀 / private
void ShowScreen::display(string str){
    
    cout<<"-------------------------------"<<endl;
    cout<<str<<endl;
    cout<<"-------------------------------"<<endl;
}



//화면에 음료 재고 출력, s 입력하면 나올 화면
void ShowScreen::displayCurrentStock(std::list<Beverage> beverages) {
    std::string str;

    for (int i = 0; i < static_cast<int>(BeverageType::COUNT); i++) {
        auto it = beverages.begin();  
        std::advance(it, i);  // i번째 요소

        int code = it->getBevCode();
        int quantity = it->getBevNum();
        BeverageType type = static_cast<BeverageType>(i);

        // 항목 출력 형식
        str += "(" + std::to_string(code) + ") " + toString(type) + " : " + std::to_string(quantity) + "개 " + std::to_string(toPrice(type))+"원 ";

        // 10개마다 줄바꿈
        if ((i + 1) % 5 == 0) {
            str += "\n";
        }
    }
    str+=  "\n음료 ID와 갯수를 입력해주세요. (e.g. 3 2)\n";
    display(str);
}

void ShowScreen::displayFirstScreen(){
    string str;
    str += std::string("어서오세요.") + "\n";
    str += std::string("시작하려면 's'를 입력하세요.") + "\n";
    str += std::string("(선결제 인증코드 입력을 원하시면 인증코드를 입력하세요...)");

    display(str);
}

void ShowScreen::displayPrepayLocation(int coor_x, int coor_y){
    string str;
    str +=string("목표 위치 x: %d / y: %d\n",coor_x,coor_y);
    str +=string("결제를 원하시면 'y', 원하지 않으시면 'n'를 입력해주세요\n");
    str +=string("결제 여부: ");
    display(str);
}

void ShowScreen::displayEnterCardNum(){
    string str = "카드 번호를 입력해주세요.";

    display(str);
}



void ShowScreen::displayPaymentResultScreen(bool paymentSuccess){
    if(paymentSuccess) displayPaymentSuccess();
    else displayPaymentFailed();
}

void  ShowScreen::displayPaymentSuccess(){
    string str = "결제에 성공했습니다";
    display(str);
}

void ShowScreen::displayPaymentFailed(){
    string str = "결제에 실패했습니다.\n";
    str+= "카드번호나 잔액을 확인해주세요";

    display(str);
}

void ShowScreen::displayInvalidIdRange(){
    string str = "id는 1~20 사이여야 합니다. 다시 입력하세요.\n";
    display(str);
}

void ShowScreen::displayBeverage(int item_id, int item_num){
    BeverageType type = static_cast<BeverageType>(item_id-1);
    std::ostringstream oss;
    oss <<"음료수 명 : "<<toString(type)<<", 갯수: "<<item_num<<"\n안녕히가십시오.";
    string str = oss.str();
    display(str);
}


void ShowScreen::displayCertCodeEnter(){
    string str = "인증코드를 입력하세요";
    display(str);
}
void ShowScreen::displayCertCodeFailed(){
    string str = "유효하지 않은 인증코드 입니다.";
    display(str);
}

void ShowScreen::displayGetCardNum(){
    string str = "카드번호를 입력해주세요";
    display(str); 
}

void ShowScreen::displayPositionAndCertCode(int x, int y, std::string cert_code){
    std::string str = "x좌표: " + std::to_string(x) + ", y좌표: " + std::to_string(y) + ", 인증번호: " + cert_code;
    display(str);
}

void ShowScreen::displayPrePayCancel(){
    std::string str = "선결제 자판기에서 요청이 거부되었습니다. 금액을 환불합니다.";
    display(str);
}

void ShowScreen::displayPrePayDisavailable(){
    std::string str = "선결제 가능한 자판기가 존재하지 않습니다.";
    display(str);
}