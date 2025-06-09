#pragma once
#include <string>

using std::string;

class Input {
    
    public: 
    int getFirstScreenAnswer() const; // 0: 구매, 1: 선결제 음료 수령 2. 관리자 모드
    std::pair<int, int> getItemIDandNum() const;
    bool getBoolAnswer() const;
    int getCardNum() const;
    std::string getCertCode() const;
};
