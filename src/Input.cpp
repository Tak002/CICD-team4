// Input.cpp
#include "Input.hpp"
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;
int Input::getFirstScreenAnswer() const{
        
        std::string inputString;
        std::getline(std::cin,inputString);
        if(inputString == "s"){return 0;}
        else if (inputString == "인증코드"){return 1;}
        return -1;

}


//오류 검출 과정 추가 필요
std::pair<int, int> Input::getItemIDandNum() const{
    while(true){
        std::string line;
        std::getline(std::cin, line);

        std::istringstream iss(line);
        int itemID;
        int itemNum;
        std::string extra;
        if ((iss >> itemID >> itemNum) && !(iss >> extra)) {
            return {itemID, itemNum}; // 정상 입력
        } else { // 해당 로직은 따로 처리할 필요 존재
            std::cerr << "[입력 오류] 잘못된 인자가 입력되었습니다. 다시 입력해주세요\n";
        }
    }
}



bool Input::getBoolAnswer() const{
    std::string inputString;
    while(true){
        std::getline(std::cin,inputString);
        if(inputString == "Y" || inputString == "y"){return true;}
        else if (inputString == "N"||inputString == "n"){return false;}
        cout<< "올바르지 않은 입력입니다. y나 n을 입력하세요"<<endl;
    }
}

bool isAllDigits(const std::string& str) {
    if (str.empty()) return false;
    for (char ch : str) {
        if (!std::isdigit(static_cast<unsigned char>(ch))) {
            return false;
        }
    }
    return true;
}
//오류 검출 과정 추가 필요
int Input::getCardNum() const{
    int value;
    std::string line;
    while(true){
        std::getline(std::cin, line);

        if (isAllDigits(line)) {
            value = std::stoi(line);  // 문자열을 int로 변환
            return value;
        } else {
            std::cout << "유효하지 않은 입력입니다. 숫자만 입력하세요.\n";
        }
    }
}

//오류 검출 과정 추가 필요
std::string Input::getCertCode()const {

    std::string inputString;
    std::getline(std::cin,inputString);
    return inputString;
}