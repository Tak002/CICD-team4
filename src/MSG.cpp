// MSG.cpp
#include "MSG.hpp"
// #include "Stock.hpp"
/*
    1. JSON 파일 생성 메시지 형식대로
    2. broadcast 또는 ACK 메시지 전송
*/

#include <iostream>     //표준 입력/출력 스트림 (예: std::cout)
#include <string.h>     //C++ 표준 문자열 클래스 사용
#include <sys/types.h>  //POSIX 시스템 호출을 위한 헤더 파일
#include <sys/socket.h> //소켓 프로그래밍을 위한 헤더 파일
#include <netinet/in.h> //인터넷 주소 체계에 대한 정의
#include <arpa/inet.h>  //IP 주소 변환을 위한 헤더 파일
#include <unistd.h>    //POSIX 운영 체제 API를 위한 헤더 파일
#include <fstream>      //파일 저장을 위한 헤더 파일
#include <sys/stat.h> //파일 상태를 확인하기 위한 헤더 파일

#include <fstream>
#include "../include/nlohmann/json.hpp"

// 스레드 관련 라이브러리 msg 동작 확인을 위해 임시 추가
#include <thread> // 스레드 관련 헤더 파일
#include <chrono> // 스레드와 시간 관련 헤더 파일

using json = nlohmann::json; // JSON 라이브러리 사용

#define PORT_NUM 9000 // 서버 포트 번호
#define BUFSIZE 1024 // 버퍼 사이즈

// #define tmp_ip_address "127.0.0.1" // 임시 ip 주소

// #include <nlohmann/json.hpp>
using json = nlohmann::json;

std::string msgFormat(
    const std::string &quest_type,
    const std::string &dst_id,
    const std::string &item_code,
    const std::string &item_num,
    const std::string &coor_x,
    const std::string &coor_y,
    const std::string &cert_code,
    const std::string &availability)
{
    json msg;
    msg["msg_type"] = quest_type;
    msg["src_id"] = "T4";
    msg["dst_id"] = dst_id;

    try
    {
        msg["msg_content"]["item_code"] = !item_code.empty() ? std::stoi(item_code) : 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Error] item_code 변환 실패: " << e.what() << std::endl;
        msg["msg_content"]["item_code"] = 0;
    }

    try
    {
        msg["msg_content"]["item_num"] = !item_num.empty() ? std::stoi(item_num) : 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Error] item_num 변환 실패: " << e.what() << std::endl;
        msg["msg_content"]["item_num"] = 0;
    }

    try
    {
        msg["msg_content"]["coor_x"] = !coor_x.empty() ? std::stoi(coor_x) : 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Error] coor_x 변환 실패: " << e.what() << std::endl;
        msg["msg_content"]["coor_x"] = 0;
    }

    try
    {
        msg["msg_content"]["coor_y"] = !coor_y.empty() ? std::stoi(coor_y) : 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Error] coor_y 변환 실패: " << e.what() << std::endl;
        msg["msg_content"]["coor_y"] = 0;
    }

    msg["msg_content"]["cert_code"] = cert_code;
    msg["msg_content"]["availability"] = availability;

    // std::cout << "[MSG] JSON Message: " << msg.dump(2) << std::endl; // 2칸 들여쓰기로 예쁘게 출력
    // std::ofstream file("../data/msg.json"); // JSON 파일 저장

    return msg.dump(2); // JSON string with pretty-print
}

#pragma region socketOpen
// void socketOpen() // 통신을 하기 위해 소켓을 생성하는 함수 --> 일반적으로 서버 소켓 생성과 클라이언트 소켓 생성은 별도로 구현하는 것이 일반적이다. 묶을 수 있다면 추후에 묶도록 하겠다.
// {
//     int sock = socket(AF_INET, SOCK_STREAM, 0); // 소켓 생성 IPv4, TCP 소켓, 프로토콜 0
//     if (sock < 0)                               // 소켓 생성 실패 시
//     {
//         std::cerr << "Socket creation failed" << std::endl;
//         return;
//     } // 소켓 생성 실패 시 에러 메시지 출력
// }
#pragma endregion

#pragma region bindSocket
// void bindSocket() // 서버가 클라이언트의 요청을 수신할 주소와 포트를 지정하는 함수 --> 즉 서버 소켓에서만 쓰이는 함수
// {
//     int serverSocket = socket(AF_INET, SOCK_STREAM, 0); // ipv4, TCP 소켓 생성
//     if (serverSocket < 0)
//     {
//         std::cerr << "Socket creation failed" << std::endl;
//         return;
//     }
    // 서버 주소 구조체 초기화
//     struct sockaddr_in address;           // 주소 구조체 선언 (short sin_family; // 주소 체계 (IPv4), u_short sin_port; // 포트 번호, struct in_addr sin_addr; // IP 주소, char sin_zero[8]; // 패딩)
//     memset(&address, 0, sizeof(address)); // 주소 구조체 초기화
//     address.sin_family = AF_INET;         // 주소 체계 (IPv4)
//     address.sin_addr.s_addr = INADDR_ANY; // 모든 IP로부터
//     address.sin_port = htons(PORT_NUM);   // 포트 9000 (네트워크 바이트 순서로 변환)
//
//     int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen); // bind 함수 선언
//
    // if (bind(serverSocket, (struct sockaddr *)&address, sizeof(address)) < 0)
    // {
    //     perror("bind failed");
    //     exit(EXIT_FAILURE);
    // }
// }
#pragma endregion

#pragma region connectSocket // 클라이언트가 서버에 연결하는 함수
// void connectSocket()
// {
//     int clientSocket = socket(AF_INET, SOCK_STREAM, 0); // TCP 소켓 생성
//     if (clientSocket < 0)
//     {
//         std::cerr << "Socket creation failed\n";
//     }
// 
//     sockaddr_in serverAddr;
//     memset(&serverAddr, 0, sizeof(serverAddr));
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(8080);                     // 포트 설정
//     inet_pton(AF_INET, tmp_ip_address, &serverAddr.sin_addr); // IP 설정
//
//     // 서버에 연결 시도
//     if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
//     {
//         std::cerr << "Connection failed\n";
//     }
//
//     std::cout << "서버 연결 성공\n";
//
//     close(clientSocket); 
// }
#pragma endregion

void clientMessageOpen(const std::string &quest_type, const std::string &dst_id, const std::string &item_code, const std::string &item_num, const std::string &coor_x, const std::string &coor_y, const std::string &cert_code, const std::string &availability)
{
    std::string ip_address; // IP 주소를 저장할 변수
    std::cout << "[Client] Start sending message to server" << std::endl;
    /*
        1. socket()        // 소켓 생성
        2. connect()       // 서버에 연결 요청
        3. write()/send()  // 서버에 데이터 전송
        4. read()/recv()   // 서버로부터 데이터 수신
        5. close()         // 소켓 닫기
    */
    char buffer[BUFSIZ]; // 버퍼 선언

    int client_fd = socket(AF_INET, SOCK_STREAM, 0); // 클라이언트 소켓 생성
    if (client_fd < 0) // 소켓 생성 실패 시 에러 메시지 출력
    {
        std::cerr << "Socket creation failed" << std::endl;
        return;
    }
    std::cout << "[" << dst_id << "] Socket created" << std::endl;

    // JSON 파일에서 dst_id에 따른 IP주소 가져오기
    #pragma region jsonFileReadToGetIpAddress
    std::ifstream file("../msgdata/ip_address.json");
    if (!file.is_open())
    {
        std::cerr << "[ERROR] ip_address.json 파일을 열 수 없습니다." << std::endl;
        return;
    }

    nlohmann::json j;
    try
    {
        file >> j;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR] JSON 파싱 실패: " << e.what() << std::endl;
        return ;
    }

    if (j.contains(dst_id))
    {
        ip_address = j[dst_id].get<std::string>();
    }
    else
    {
        std::cerr << "[ERROR] 해당 key가 존재하지 않습니다: " << dst_id << std::endl;
        return;
    }
    #pragma endregion

    
    struct sockaddr_in server_addr;  // 서버 주소 구조체 선언
    server_addr.sin_family = AF_INET;   // 주소 체계 (IPv4)
    server_addr.sin_port = htons(PORT_NUM); // 9000 포트로 설정
    inet_pton(AF_INET, ip_address.c_str(), &server_addr.sin_addr); // IP 주소 설정 (문자열 IP 주소를 이진 IP 주소로 변환)
    std::cout << "[" << dst_id << "] Server address set" << std::endl;

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        if (client_fd < 0){
            std::cerr << "Socket creation failed" << std::endl;
        }

            std::cerr << "connect failed. errno=" << errno << " : " << strerror(errno) << std::endl;

    if (errno == ECONNREFUSED) {
        std::cerr << " 서버가 연결을 거부했습니다 (ECONNREFUSED)" << std::endl;
    }
        
        std::cerr << "Connection failed" << std::endl;
        close(client_fd);
        return;
    }

    std::string jsonStrClient = msgFormat(quest_type, dst_id, item_code, item_num, coor_x, coor_y, cert_code, availability);
    send(client_fd, jsonStrClient.c_str(), jsonStrClient.length(), 0);
    std::cout << "[" << dst_id << "] Message sent" << std::endl;

    // 타임아웃 설정
    struct timeval timeout{};
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    int valread = recv(client_fd, buffer, BUFSIZ - 1, 0);
    if (valread > 0)
    {
        buffer[valread] = '\0';
        std::cout << "[" << dst_id << "] Response: " << buffer << std::endl;
    }
    else
    {
        std::cerr << "[" << dst_id << "] No response or timeout." << std::endl;
    }

    close(client_fd);
    std::cout << "[" << dst_id << "] Socket closed" << std::endl;
}

void serverMessageOpen(const std::string &quest_type, const std::string &dst_id, const std::string &item_code, const std::string &item_num, const std::string &coor_x, const std::string &coor_y, const std::string &cert_code, const std::string &availability)
{
    /*
        1. socket()        // 소켓 생성
        2. bind()          // IP주소와 포트번호를 소켓에 할당
        3. listen()        // 연결 요청 대기 상태
        4. accept()        // 클라이언트 연결 수락
        5. read()/recv()   // 클라이언트로부터 데이터 수신
        6. write()/send()  // 클라이언트로 데이터 송신
        7. close()         // 소켓 닫기
    */
    char buffer[BUFSIZ]; // 버퍼 선언

    #pragma region socketcreate
    int server_fd = socket(AF_INET, SOCK_STREAM, 0); // 서버 소켓 생성
    if (server_fd < 0) // 소켓 생성 실패 시 에러 메시지 출력
    {
        std::cerr << "Socket creation failed" << std::endl;
        return;
    }
    #pragma endregion

    #pragma region socketOption
    struct sockaddr_in address; // 서버 주소 구조체 선언
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT_NUM); // 9000 포트로 설정
    #pragma endregion

    #pragma region bind
    // bind 함수 호출 및 오류 처리
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) // bind 실패 시 에러 메시지 출력
    {
        std::cerr << "Bind failed. errno=" << errno << " : " << strerror(errno) << std::endl;
        close(server_fd);
        return;
    }
    std::cout << "Bind successful" << std::endl; // 바인드 성공 시 출력
    #pragma endregion

    #pragma region listen
    listen(server_fd, 8); // 연결 요청 대기 상태
    std::cout << "Listening on port 9000" << std::endl; // 연결 요청 대기 상태 출력
    #pragma endregion

    for (int i = 0; i < 8; ++i)
    { // 8번 반복해서 8개의 클라이언트 처리
        std::string dst_id = "T" + std::to_string(i);

        socklen_t addrlen = sizeof(address);
        int client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        if (client_socket < 0)
        {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }
        std::cout << "Client connected" << std::endl;

        int valread = read(client_socket, buffer, BUFSIZ);
        if (valread > 0)
        {
            buffer[valread] = '\0';

            // 현재 날짜와 시간을 기반으로 파일명 생성
            auto now = std::chrono::system_clock::now();
            std::time_t now_time = std::chrono::system_clock::to_time_t(now);
            std::tm* now_tm = std::localtime(&now_time);

            char tmpfilename[128];
            std::strftime(tmpfilename, sizeof(tmpfilename), "../msgdata/msg_%Y%m%d_%H%M%S", now_tm);
            if (mkdir(tmpfilename, 0755) == 0)
            {
                std::cout << "디렉토리 생성 성공" << std::endl;
            }
            else
            {
                perror("mkdir 실패");
            }

            std::string filename = std::string(tmpfilename) + "/_" + dst_id + ".json";

            // JSON 파싱 및 파일 저장
            json msg = json::parse(buffer);
            std::ofstream file(filename);
            if (file.is_open()) {
                file << msg.dump(2);
                file.close();
                std::cout << "Saved JSON to: " << filename << std::endl;
            } else {
                std::cerr << "Failed to open file: " << filename << std::endl;
            }

        }

        std::string jsonStrServer = msgFormat("resp_stock", dst_id, "2", "7", "5", "8", "", "");
        send(client_socket, jsonStrServer.c_str(), jsonStrServer.length(), 0);
        std::cout << "Sent JSON From Server" << std::endl;

        close(client_socket);
    }

    close(server_fd);
    // close(server_fd); // 서버 소켓 닫기 일시 정지

    // // std::cout << "Client IP: " << inet_ntoa(address.sin_addr) << ", Port: " << ntohs(address.sin_port) << std::endl; // 클라이언트 IP 주소 및 포트 번호 출력
    // read(client_socket, buffer, BUFSIZ);
    // send(client_socket, "Hello", strlen("Hello"), 0);
}

// TCP에서는 브로드캐스트가 안되기에 broadcast를 사용하지 않으므로 TCP 프로토콜을 사용해 그냥 모든 클라이언트에게 메시지를 전송하는 것으로 대체

void broadMessage(const json &msg)
{
    std::vector<std::thread> threads;
    for (int i = 1; i < 9; ++i)
    {
        std::string dst_id = "T" + std::to_string(i);
        threads.emplace_back([msg, dst_id]()
        {
            try {
                std::cout << "[Broadcast] Sending message to client" << std::endl;
                clientMessageOpen("resp_stock", dst_id, msg["msg_content"]["item_code"], msg["msg_content"]["item_num"], msg["msg_content"]["coor_x"], msg["msg_content"]["coor_y"], "", "");
            } catch (const std::exception& e) {
                std::cerr << "[Exception] " << e.what() << std::endl;
            } });
    }

    for (auto &t : threads)
    {
        if (t.joinable())
            t.join();
    }

    std::cout << "[Broadcast] 모든 메시지 전송 완료" << std::endl;

    return;
}

// 인증번호를 전송하는 함수 선결제 영역
void Send(const std::string &authenticationNum)
{
    std::cout << "[Send] Authentication Number: " << authenticationNum << std::endl;
}

// 클라이언트 소켓을 생성하고 타 서버에 연결하는 함수를 구현
void DVMMessageOutofStock(int beverageId, int quantity)
{
    std::cout << "[Out of Stock] Beverage ID: " << beverageId << ", Quantity: " << quantity << std::endl; // 재고 부족 메시지 출력
    json DVMMessageOutOfStock_MessageFormat = {
        {"msg_type", "req_stock"},
        {"dst_id", "0"},
        {"msg_content", {{"item_code", std::to_string(beverageId)}, {"item_num", std::to_string(quantity)}, {"coor_x", ""}, {"coor_y", ""}, {"cert_code", ""}, {"availability", ""}}}};

    broadMessage(DVMMessageOutOfStock_MessageFormat); // 재고 부족 메시지 전송

    return;
}

// 다른 DVM에서 재고 확인 요청을 받았을 때 호출되는 함수
void AskStockMessage(json msg)
{
    std::cout << "[Ask Stock] Stock으로부터 확인 중" << msg << std::endl;

    // 재고 확인 요청 메시지 처리
    // 일단 동작은 막아놓았다.
    // Stock stock;
    // stock.isBuyable(msg["msg_content"]["item_code"], msg["msg_content"]["item_num"]); // 재고 확인
    
}

// json 형식의 메시지를 읽고 그에 따라 적절한 메시지를 보내는 함수 즉 재고가 부족하다고 문자가 올 경우 현재 재고 상황을 보내주는 함수 및 req_prepay에 대한 ACK를 보내는 함수
void sendMessagge(const std::string &msg)
    {
        std::cout << "[Send Message] " << msg << std::endl;
    }


int main()
{
    json msg = {
        {"msg_type", "ack"},
        {"src_id", "T0"},
        {"dst_id", "T1"},
        {"msg_content", {{"item_code", 1}, {"item_num", 1}, {"coor_x", 0}, {"coor_y", 0}, {"cert_code", "ABC"}, {"availability", "Y"}}}};
    // 이 부분은 테스트용으로 작성된 코드입니다.
    // 실제로 서버가 먼저 실행되어야 합니다.
    std::thread serverThread([msg]()
    {
        serverMessageOpen("resp_stock", "2", "7", "5", "10", "", "", ""); // 또는 acceptAndRespond(server_fd, msg);
    });

    // 서버가 포트 바인딩 및 listen까지 준비될 시간을 주기
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    DVMMessageOutofStock(20, 99); // 재고 부족 메시지 전송

    serverThread.join();
    // clientThread.join();

    return 0;
}
