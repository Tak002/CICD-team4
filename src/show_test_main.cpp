#include <iostream>
#include <list>
#include "ShowScreen.hpp"
#include "Beverage.hpp"
#include "enum/BeverageType.hpp"

// toString 함수는 enum/BeverageType.cpp에 구현되어 있다고 가정

int main() {
    ShowScreen screen;

    // 1. 첫 화면 출력
    std::cout << "[TEST] displayFirstScreen\n";
    screen.displayFirstScreen();

    // // 2. 음료 재고 출력
    // std::cout << "[TEST] displayCurrentStock\n";
    // std::list<Beverage> beverages;
    // for (int i = 0; i < static_cast<int>(BeverageType::COUNT); ++i) {
    //     beverages.emplace_back(i, (i + 1) * 5); // id, quantity
    // }
    // screen.displayCurrentStock(beverages);

    // 3. 선결제 위치 안내
    std::cout << "[TEST] displayPrepayLocation\n";
    screen.displayPrepayLocation(7, 15);

    // 4. 카드번호 입력 화면
    std::cout << "[TEST] displayEnterCardNum\n";
    screen.displayEnterCardNum();

    // 5. 결제 결과(성공)
    std::cout << "[TEST] displayPaymentResultScreen(true)\n";
    screen.displayPaymentResultScreen(true);

    // 6. 결제 결과(실패)
    std::cout << "[TEST] displayPaymentResultScreen(false)\n";
    screen.displayPaymentResultScreen(false);

    // 7. 잘못된 ID 범위 안내
    std::cout << "[TEST] displayInvalidIdRange\n";
    screen.displayInvalidIdRange();

    // 8. 음료 선택 결과 화면
    std::cout << "[TEST] displayBeverage\n";
    screen.displayBeverage(2, 8);

    // 9. 인증코드 입력 화면
    std::cout << "[TEST] displayCertCodeEnter\n";
    screen.displayCertCodeEnter();

    // 10. 인증코드 실패 안내
    std::cout << "[TEST] displayCertCodeFailed\n";
    screen.displayCertCodeFailed();

    // 11. 첫 화면으로 복귀
    // std::cout << "[TEST] returnToFirstScreen\n";
    // screen.returnToFirstScreen();

    return 0;
}
