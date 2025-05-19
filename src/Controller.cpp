// Controller.cpp
#include "Controller.hpp"
#include <list>




void Controller:: run(){
    Controller::setController();
    while(true){
        showScreen.displayFirstScreen();
        int firstScreenAnswer = input.getFirstScreenAnswer();
        //음료 선택 및 결제 과정
        if(firstScreenAnswer==0){ 
            //구매하고자 하는 음료수 id, 갯수 입력
            showScreen.displayCurrentStock(stock.getCurrentStock());
            auto [itemID, itemNum] = input.getItemIDandNum();
            //범위를 벗어난 id입력시 초기화면으로 이동
            if (itemID < 1 || itemID > 20) { 
                showScreen.displayInvalidIdRange();
                continue; 
            }
            //재고 부족시 선결제 여부 확인
            if(stock.isPrepayment(itemID,itemNum)){
                // 가장 가까운 주소 확인 및 출력
                
                //coor_x = ?, coor_y = ?

                // 구매 여부 확인
                if(!input.getBoolAnswer())continue; //구매 거절시 초기화면으로
                isPrepayment = true;
            }
            int price = getPrice(itemID) * itemNum; //price 계산 구현 필요
            // 결제
            showScreen.displayGetCardNum();
            int cardNum= input.getCardNum();
            //카드 정보 유효한지확인도 넣어야할수도 있음.
            bool isPaymentSuccess = bank.reqeustPayment(cardNum,price);
            showScreen.displayPaymentResultScreen(isPaymentSuccess);
            if(!isPaymentSuccess)continue; //결제 실패시 초기화면으로

            // 선결제 후 다른 자판기에 메시지 송신
            if(isPrepayment = true){
                std::string newCertCode = certCode.createCertCode();  
                //선결제 메시지 송신 및 결과 수신



                bool availability; // = 선결제 메시지 결과 수신. From MSG
                if(availability){
                    showScreen.displayPositionAndCertCode(coor_x, coor_y, newCertCode);
                }
                else{
                    showScreen.displayPrePayCancel();
                    bank.rollback(cardNum,price);
                }
                isPrepayment = false;
            }else{ // 일반 결제 후 음료 수령
                stock.requestBeverage(itemID,itemNum);
                showScreen.displayBeverage(itemID,itemNum);
                continue; //초기화면으로

            }
        }



        //선결제 음료 수령 과정
        else if (firstScreenAnswer==1){ 
            showScreen.displayCertCodeEnter();
            std::string certCode = input.getCertCode();
            bool isValid = certCodeManager.isValidateCertCode(certCode); //CertCodeManager 미구현
            //인증번호가 유효할 경우
            if(isValid){
                // itemID, itemNum from certCodeManager
                // showScreen.displayBeverage(itemID,itemNum);
                continue;
            }
            //인증번호가 유효하지 않을 경우
            else{
                showScreen.displayCertCodeFailed();
                continue;

            }
        }
    }
}

//  초기설정. stock 재고 기입, msg 수신 시작
void Controller:: setController(){
    isPrepayment = false;
};


void Controller::saveCertCode(string newCertCode,int item_id, int item_num){
    certCodeManager.saveCertCode(newCertCode,item_id,item_num);
}