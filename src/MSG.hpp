#pragma once
#include <string>

#include "../include/nlohmann/json.hpp"
using json = nlohmann::json; // JSON 라이브러리 사용

class MSG {
    private:
        int clientSocketfd; // 클라이언트 소켓 파일 디스크립터
        int serverSocketfd; // 서버 소켓 파일 디스크립터
        void socketOpen(); // 통신을 하기 위해 소켓을 생성하는 함수
        void bindSocket(); // 서버가 클라이언트의 요청을 수신할 주소와 포트를 지정하는 함수
        std::string messageFormat(const std::string &msg);
        void broadMessage(const std::string &msg);
        void sendCertCode(const std::string &dst_id, const std::string &item_code, const std::string &item_num, const std::string &cert_code);
        void clientMessageOpen();
        void send(int sockfd, const std::string &msg);
        void handleClient(int client_socket);
    public :
        void serverMessageOpen();
        std::vector<std::string> DVMMessageOutofStock(); // 재고 부족시 메시지 전송 -> 1. broadcast로 클라이언트가 스레드를 만들어서 총 7개의 json 메시지를 받아온 후 2. 7개의 메시지를 바탕으로 Calc에서 거리를 계산 후 3. Nearest Position을 호출하면 된다.
        json AskStockMessage(const json); // 1. 다른 DVM에서 제고 확인 요청을 받았을 때 서버가 스레드를 하나 만들어서 2. Stock에서 재고를 호출한 후 
        bool sendMessage(const std::string msg_type, const std::string &msg); // 고민 중 private으로 바꿔도 될지 모르겠음 일단 clientMessage를 보내는 부분임 스레드로 만들 때 이 함수를 호출할 예정
};
