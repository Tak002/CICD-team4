#pragma once
#include <string>

using std::string;

class Input {
    
    public: 
    int getFirstScreenAnswer(); // 0: 구매, 1: 선결제 음료 수령 2. 관리자 모드
    std::pair<int, int> getItemIDandNum();
    bool getBoolAnswer();
    int getCardNum();
    std::string getCertCode();
};
