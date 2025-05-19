
// ShowScreen.cpp

#include <list>
#include <string>
#include <iterator>
#include <iostream>
#include <sstream>

#include "ShowScreen.hpp"
#include "enum/BeverageType.hpp"

using namespace std;

//화면 출력 틀 / private
void ShowScreen::display(string str){
    
    cout<<"-------------------------------"<<endl;
    cout<<str<<endl;
    cout<<"-------------------------------"<<endl;
}

void ShowScreen::returnToFirstScreen(){
    displayFirstScreen();
}


//확정
void ShowScreen::displayCurrentStock(std::list<Beverage> beverages) {
    
    
    cout<<"---------------------------------------------------------"<<endl<<endl;
    std::string str;

    for (int i = 0; i < static_cast<int>(BeverageType::COUNT); i++) {
        auto it = beverages.begin();  
        std::advance(it, i);  // i번째 요소

        int code = it->getBevCode();
        int quantity = it->getBevNum();
        int price = it->getPrice();
        BeverageType type = static_cast<BeverageType>(i);

        // 항목 출력 형식
        str += "(" + std::to_string(code+1) + ") " + toString(type) + " : " + std::to_string(quantity) + "개 " + to_string(price) + "원 ";

        // 10개마다 줄바꿈
        if ((i + 1) % 5 == 0) {
            str += "\n";
        }
    }
    str+=  "\n음료 ID와 갯수를 입력해주세요. (e.g. 3 2)\n";
    cout<<str;
    cout<<"---------------------------------------------------------"<<endl;
}

//확정
void ShowScreen::displayFirstScreen(){
    cout<<"---------------------------------------------------------"<<endl<<endl;
    cout<<"어서오세요. "<<endl;
    cout<<"시작하려면 's'를 입력하세요."<<endl<<endl;
    cout<<"(선결제 인증코드 입력을 원하시면 인증코드를 입력하세요)"<<endl<<endl;
    cout<<"---------------------------------------------------------"<<endl;
}


// 확정
void ShowScreen::displayEnterCardNum(){
    cout<<"---------------------------------------------------------"<<endl<<endl;
    cout<<"카드 번호를 입력해주세요."<<endl;
    cout<<"---------------------------------------------------------"<<endl;
}


// 확정
void ShowScreen::displayPaymentResultScreen(bool paymentSuccess){
    if(paymentSuccess) displayPaymentSuccess();
    else displayPaymentFailed();
}

//확정
void  ShowScreen::displayPaymentSuccess(){
    cout<<"---------------------------------------------------------"<<endl<<endl;
    cout<<"결제에 성공했습니다"<<endl;
    cout<<"---------------------------------------------------------"<<endl;
}

// 확정
void ShowScreen::displayPaymentFailed(){
    cout<<"---------------------------------------------------------"<<endl<<endl;
    cout<<"결제에 실패했습니다."<<endl;
    cout<<"카드번호나 잔액을 확인해주세요"<<endl;
    cout<<"---------------------------------------------------------"<<endl;
}

void ShowScreen::displayInvalidIdRange(){
    string str = "id는 1~20 사이여야 합니다. 다시 입력하세요.\n";
    display(str);
}

//확정
void ShowScreen::displayBeverage(int item_id, int item_num){
    BeverageType type = static_cast<BeverageType>(item_id-1);
    std::ostringstream oss;
    oss <<"음료수 명 : "<<toString(type)<<", 갯수: "<<item_num<<"\n안녕히가십시오.";
    string str = oss.str();
    

    cout<< "---------------------------------------------------------"<<endl<<endl;
    cout<< str<<endl<<endl;
    cout<<"---------------------------------------------------------"<<endl;
}


//확정
void ShowScreen::displayCertCodeEnter(){
    cout<<  "---------------------------------------------------------"<<endl<<endl;
    cout<<  "인증코드를 입력해주세요"<<endl<<endl;
    cout<<"---------------------------------------------------------"<<endl;
}

void ShowScreen::displayCertCodeFailed(){
    cout<<  "---------------------------------------------------------"<<endl<<endl;
    cout<<  "유효하지 않은 인증코드 입니다.;"<<endl<<endl;
    cout<<"---------------------------------------------------------"<<endl;
}

// 확정
void ShowScreen::displayNearestPosition(int x, int y){

    cout<<  "---------------------------------------------------------"<<endl<<endl;
    cout<<  "선택하신 음료와 수량을 갖고 있는 자판기는 x: "<<x<<" y: "<<y<<"에 존재합니다."<<endl;
    cout<<  "선결제를 마저 진행하시겠습니까?"<<endl<<endl;
    cout<<  "           (y/n)"<<endl;
    cout<<"---------------------------------------------------------"<<endl;


}


// 확정
void ShowScreen::displayPrePayCancle(){
    cout<<  "---------------------------------------------------------"<<endl<<endl;
    cout<<  "자판기 시스템에 의해 선결제가 취소되었습니다. 금액이 환불되었습니다."<<endl;
    cout<<  "처음 화면으로 돌아갑니다."<<endl<<endl;
    cout<<"---------------------------------------------------------"<<endl;
    cout<<endl<<endl<<endl;
}

// 확정
void ShowScreen::displayPositionAndCertCode(int coor_x, int coor_y, string cert_code){
    cout<<  "---------------------------------------------------------"<<endl<<endl;
    cout<<  "위치 : ( "<<coor_x<<", "<<coor_y<<" ) / 인증코드 : "<<cert_code<<endl;
    cout<<  "감사합니다"<<endl<<endl;
    cout<<"---------------------------------------------------------"<<endl;
    cout<<endl<<endl<<endl;
}
