// Input.cpp
#include "Input.hpp"

const std::string YES = "s";
const std::string CERT_CODE = "인증번호";

Input::Input(Controller* ctrl){
    controller = ctrl;
}

void Input::run(){
    controller->setController(); //set MSG thread and print Initial screen.
    while(true){
        controller->runShowScreenCommand(CommandType::DISPLAY_FIRST_SCREEN);
        std::string firstScreenAnswer = getFirstScreenAnswer();
        if (firstScreenAnswer==YES){ //음료 선택 및 결제 시나리오
            controller->runShowScreenCommand(CommandType::DISPLAY_CURRENT_STOCK);
            int itemID, itemNum;
            while (true) { //구매하고자 하는 음료의 ID와 갯수 입력받기기
                auto [_itemID, _itemNum] = getItemIDandNum();
                if (_itemID >= 1 && _itemID <= 20) {
                    break;   // 조건을 만족하면 반복 종료
                } else {
                    controller->runShowScreenCommand(CommandType::DISPLAY_INVALID_ID_RANGE);
                }
            }

            
        }
        else if (firstScreenAnswer == CERT_CODE){  // 인증번호 입력 후 음료 수령 시나리오

        }
        // 올바르지 않은 입력시 초기화면으로
    }

}


std::string Input::getFirstScreenAnswer(){
    std::string inputString;
    std::getline(std::cin,inputString);

    if(inputString == YES){return YES;}
    else if (inputString == CERT_CODE){return CERT_CODE;}
    else return NULL;
}



std::pair<int, int> getItemIDandNum() {
    int itemID, itemNum;
    std::cin >> itemID >> itemNum;
    return std::make_pair(itemID,itemNum);
}
