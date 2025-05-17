// MSG.cpp
#include "MSG.hpp"

#include "Stock.hpp"
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

std::string msgFormat(const json &params)
{
    json msg;

    // 기본값 설정 (필요할 경우)
    msg["msg_type"] = params.value("msg_type", "");
    msg["src_id"] = params.value("src_id", "T4");
    msg["dst_id"] = params.value("dst_id", "T1");

    // msg_content 처리
    if (params.contains("msg_content"))
    {
        msg["msg_content"]["item_code"] = params["msg_content"].value("item_code", "");
        msg["msg_content"]["item_num"] = params["msg_content"].value("item_num", 0);
        msg["msg_content"]["coor_x"] = params["msg_content"].value("coor_x", 0);
        msg["msg_content"]["coor_y"] = params["msg_content"].value("coor_y", 0);
        msg["msg_content"]["cert_code"] = params["msg_content"].value("cert_code", "");
        msg["msg_content"]["availability"] = params["msg_content"].value("availability", "T");
    }

    return msg.dump(2); // 2칸 들여쓰기로 예쁘게 출력
}

// void socketOpen() // 통신을 하기 위해 소켓을 생성하는 함수 --> 일반적으로 서버 소켓 생성과 클라이언트 소켓 생성은 별도로 구현하는 것이 일반적이다. 묶을 수 있다면 추후에 묶도록 하겠다.
// {
//     int sock = socket(AF_INET, SOCK_STREAM, 0); // 소켓 생성 IPv4, TCP 소켓, 프로토콜 0
//     if (sock < 0)                               // 소켓 생성 실패 시
//     {
//         std::cerr << "Socket creation failed" << std::endl;
//         return;
//     } // 소켓 생성 실패 시 에러 메시지 출력
// }

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

// void connectSocket() // 클라이언트가 서버에 연결하는 함수
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

void messageFormat(const std::string &msg)
{
    std::cout << "[Message Format] " << msg << std::endl;
}

void clientMessageOpen()
{
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

    struct sockaddr_in server_addr;  // 서버 주소 구조체 선언
    server_addr.sin_family = AF_INET;   // 주소 체계 (IPv4)
    server_addr.sin_port = htons(PORT_NUM); // 9000 포트로 설정
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr); // IP 주소 설정 (문자열 IP 주소를 이진 IP 주소로 변환)

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Connection failed" << std::endl;
        close(client_fd);
        return;
    }

    std::string jsonStrClient = msgFormat(msg);
    send(client_fd, jsonStrClient.c_str(), jsonStrClient.length(), 0);
    std::cout << "[" << msg["dst_id"] << "] Message sent" << std::endl;

    // 타임아웃 설정
    struct timeval timeout{};
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    int valread = recv(client_fd, buffer, BUFSIZ - 1, 0);
    if (valread > 0)
    {
        buffer[valread] = '\0';
        std::cout << "[" << msg["dst_id"] << "] Response: " << buffer << std::endl;
    }
    else
    {
        std::cerr << "[" << msg["dst_id"] << "] No response or timeout." << std::endl;
    }

    close(client_fd);
    std::cout << "[" << msg["dst_id"] << "] Socket closed" << std::endl;
}

// void serverMessageOpen(json msg)
// {
//     /*
//         1. socket()        // 소켓 생성
//         2. bind()          // IP주소와 포트번호를 소켓에 할당
//         3. listen()        // 연결 요청 대기 상태
//         4. accept()        // 클라이언트 연결 수락
//         5. read()/recv()   // 클라이언트로부터 데이터 수신
//         6. write()/send()  // 클라이언트로 데이터 송신
//         7. close()         // 소켓 닫기
//     */
//     char buffer[BUFSIZ]; // 버퍼 선언
//     int server_fd = socket(AF_INET, SOCK_STREAM, 0); // 서버 소켓 생성
//     if (server_fd < 0) // 소켓 생성 실패 시 에러 메시지 출력
//     {
//         std::cerr << "Socket creation failed" << std::endl;
//         return;
//     }
//
//     struct sockaddr_in address; // 서버 주소 구조체 선언
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT_NUM); // 9000 포트로 설정
//
//     #pragma region bind
//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
//     {
//         std::cerr << "Bind failed" << std::endl;
//         close(server_fd);
//         return;
//     }
//     std::cout << "Bind successful" << std::endl; // 바인드 성공 시 출력
//     #pragma endregion
// 
//     #pragma region listen
//     listen(server_fd, 8); // 연결 요청 대기 상태
//     std::cout << "Listening on port 9000" << std::endl; // 연결 요청 대기 상태 출력
//     #pragma endregion
//
//     socklen_t addrlen = sizeof(address); // addrlen 선언 및 초기화
//     int client_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen); // 클라이언트 연결 수락
//     if (client_socket < 0) // 클라이언트 연결 수락 실패 시 에러 메시지 출력
//     {
//         std::cerr << "Accept failed" << std::endl;
//         return;
//     }
//     std::cout << "Client connected" << std::endl; // 클라이언트 연결 성공 시 출력
//
//     // 클라이언트 메시지 수신
//  
//     // read(client_socket, buffer, BUFSIZ); // 클라이언트로부터 데이터 수신
//     int valread = read(client_socket, buffer, BUFSIZ);
//     if (valread > 0)
//     {
//         buffer[valread] = '\0';
//         std::cout << "Received: " << buffer << std::endl;
//     }
//
//     // json params = {
//     //     {"msg_type", "Server"},
//     //     {"dst_id", "T1"},
//     //     {"msg_content", {{"item_code", "00"}, {"item_num", 10}, {"coor_x", 5}, {"coor_y", 8}, {"cert_code", "ABCDE"}, {"availability", "Y"}}}};
//
//     // JSON 메시지 생성 및 전송
//     std::string jsonStrServer = msgFormat(msg);
//     send(client_socket, jsonStrServer.c_str(), jsonStrServer.length(), 0);
//     std::cout << "Sent JSON From Server\n" << std::endl;
//
//     close(client_socket);
//     // close(server_fd); // 서버 소켓 닫기 일시 정지
//
//     // // std::cout << "Client IP: " << inet_ntoa(address.sin_addr) << ", Port: " << ntohs(address.sin_port) << std::endl; // 클라이언트 IP 주소 및 포트 번호 출력
//     // read(client_socket, buffer, BUFSIZ);
//     // send(client_socket, "Hello", strlen("Hello"), 0);
// }

// 서버 포트 오픈 및 바인드() 함수
int startServerSocket()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    struct sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT_NUM);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    // {
    //     std::cerr << "Bind failed" << std::endl;
    //     close(server_fd);
    //     return -1;
    // }
    std::cout << "Bind successful" << std::endl;

    if (listen(server_fd, 8) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
        close(server_fd);
        return -1;
    }

    std::cout << "Listening on port " << PORT_NUM << std::endl;
    return server_fd;
}

// 서버 메시지 수신 및 응답 함수
void acceptAndRespond(int server_fd, json msg)
{
    char buffer[BUFSIZ];
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);

    int client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);
    if (client_socket < 0)
    {
        std::cerr << "Accept failed" << std::endl;
        return;
    }

    std::cout << "Client connected from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;

    int valread = read(client_socket, buffer, BUFSIZ);
    if (valread > 0)
    {
        buffer[valread] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }

    json params = {
        {"msg_type", "Server"},
        {"dst_id", "T1"},
        {"msg_content", {{"item_code", "00"}, {"item_num", 10}, {"coor_x", 5}, {"coor_y", 8}, {"cert_code", "ABCDE"}, {"availability", "Y"}}}};

    // JSON 메시지 생성 및 전송
    std::string jsonStrServer = msgFormat(params);
    send(client_socket, jsonStrServer.c_str(), jsonStrServer.length(), 0);
    std::cout << "Sent JSON From Server\n" << std::endl;

    close(client_socket);
    close(server_fd);

    // // std::cout << "Client IP: " << inet_ntoa(address.sin_addr) << ", Port: " << ntohs(address.sin_port) << std::endl; // 클라이언트 IP 주소 및 포트 번호 출력
    // read(client_socket, buffer, BUFSIZ);
    // send(client_socket, "Hello", strlen("Hello"), 0);
}

// void messageFormat(const std::string &msg)
// {
//     // JSON 파일 생성
//     nlohmann::json jsonData;
//     jsonData["message"] = msg;

//     // JSON 파일 저장
//     std::ofstream file("message.json");
//     if (file.is_open())
//     {
//         file << jsonData.dump(4); // 4는 들여쓰기 수준
//         file.close();
//         std::cout << "JSON file created successfully" << std::endl;
//     }
//     else
//     {
//         std::cerr << "Failed to create JSON file" << std::endl;
//     }
// }
// TCP에서는 브로드캐스트가 안되기에 broadcast를 사용하지 않으므로 TCP 프로토콜을 사용해 그냥 모든 클라이언트에게 메시지를 전송하는 것으로 대체
void broadMessage(const std::string &msg)
{
    // socketOpen(); // 소켓 생성 함수 호출
    // bindSocket(); // 바인드 함수 호출

    std::cout << "[Broadcast] " << msg << std::endl;
}

void Send(const std::string &authenticationNum)
{
    std::cout << "[Send] Authentication Number: " << authenticationNum << std::endl;
}

// 클라이언트 소켓을 생성하고 타 서버에 연결하는 함수를 구현
void DVMMessageOutofStock(int beverageId, int quantity)
{
    std::cout << "[Out of Stock] Beverage ID: " << beverageId << ", Quantity: " << quantity << std::endl;
}

void AskStockMessage(const std::string &msg)
{
    std::cout << "[Ask Stock] " << msg << std::endl;
}

void sendMessagge(const std::string &msg)
    {
        std::cout << "[Send Message] " << msg << std::endl;
    }


int main()
{
    std::thread serverThread(serverMessageOpen);

    // 서버가 포트 바인딩 및 listen까지 준비될 시간을 주기
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::thread clientThread(clientMessageOpen);

    serverThread.join();
    clientThread.join();

    return 0;
}
