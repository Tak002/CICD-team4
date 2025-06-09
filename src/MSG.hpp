#pragma once
#include <string>

#include "../include/nlohmann/json.hpp"
using json = nlohmann::json; // JSON 라이브러리 사용

class MSG {
    private:
        // int clientSocketfd; // 클라이언트 소켓 파일 디스크립터
        int serverSocketfd; // 서버 소켓 파일 디스크립터
        std::string messageFormat(const std::string &msg); // 메시지 형식
        void broadMessage(const std::string &msg); // 브로드캐스트 메시지 전체 DVM에 메시지 전송
        void clientMessageOpen(); // 클라이언트 메시지 송신 후 수신 함수
        void handleClient(int client_socket); // 서버가 클라이언트 접속 후 수신 후 송신하는 쓰레드 함수
    public :
        static void SocketOpenInIt(MSG*); // 통신을 하기 위해 소켓을 생성하는 함수
        void serverMessageOpen();
        std::tuple<int,int,std::string> DVMMessageOutofStock(int beverageId, int quantity); // 재고 부족시 메시지 전송 -> 1. broadcast로 클라이언트가 스레드를 만들어서 총 7개의 json 메시지를 받아온 후 2. 7개의 메시지를 바탕으로 Calc에서 거리를 계산 후 3. Nearest Position을 호출하면 된다.
        json AskStockMessage(const json); // 1. 다른 DVM에서 제고 확인 요청을 받았을 때 서버가 스레드를 하나 만들어서 2. Stock에서 재고를 호출한 후 
        bool sendMessage(const std::tuple<std::string, int, int, std::string>& msgData);
};