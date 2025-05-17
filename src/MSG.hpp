#pragma once
#include <string>

class MSG {
    private:
        void socketOpen(); // 통신을 하기 위해 소켓을 생성하는 함수
        void bindSocket(); // 서버가 클라이언트의 요청을 수신할 주소와 포트를 지정하는 함수
        void messageFormat(const std::string &msg);
        void broadMessage(const std::string &msg);
        void Send(const std::string &authenticationNum);
        void serverMessageOpen();
        void clientMessageOpen();
    public : void DVMMessageOutofStock();
        void AskStockMessage(const std::string &msg);
        void sendMessagge(const std::string &msg);
};
