// Input.cpp
#include "Input.hpp"
#include <iostream>
#include <limits>
#include <sstream>

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
    while(true){
        std::string line;
        std::getline(std::cin, line);

        std::istringstream iss(line);
        int itemID, itemNum;
        std::string extra;
        if ((iss >> itemID >> itemNum) && !(iss >> extra)) {
            return {itemID, itemNum}; // 정상 입력
        } else { // 해당 로직은 따로 처리할 필요 존재
            std::cerr << "[입력 오류] 잘못된 인자가 입력되었습니다. 다시 입력해주세요\n";
        }
    }
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