// Input.cpp
#include "Input.hpp"
#include <iostream>
#include <limits>

using namespace std;
int Input::getFirstScreenAnswer(){

        std::string inputString;
        std::getline(std::cin,inputString);
        if(inputString == "s"){return 0;}
        else if (inputString == "인증번호"){return 1;}
        return -1;

}


//오류 검출 과정 추가 필요
std::pair<int, int> Input::getItemIDandNum() {
    int itemID, itemNum;
    std::cin >> itemID >> itemNum; 
    return std::make_pair(itemID,itemNum);
}



bool Input::getBoolAnswer(){

    while(true){
        std::string inputString;
        std::getline(std::cin,inputString);
        if(inputString == "Y"){return true;}
        else if (inputString == "N"){return false;}
    }
}

//오류 검출 과정 추가 필요
int Input::getCardNum(){
    int cardNum;
    cin>>cardNum;
    return cardNum;
}

//오류 검출 과정 추가 필요
std::string Input::getCertCode(){

    std::string inputString;
    std::getline(std::cin,inputString);
    return inputString;
}