// MSG.cpp
#include "MSG.hpp"
#include "Stock.hpp"
#include "Position.hpp"
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
#include <unistd.h>     //POSIX 운영 체제 API를 위한 헤더 파일
#include <fstream>      //파일 저장을 위한 헤더 파일
#include <sys/stat.h> //파일 상태를 확인하기 위한 헤더 파일
#include "Position.hpp"
#include <filesystem>
#include <vector>
#include <algorithm>
#include <regex>

#include <fstream>
#include "../include/nlohmann/json.hpp"

// 스레드 관련 라이브러리 msg 동작 확인을 위해 임시 추가
#include <thread> // 스레드 관련 헤더 파일
#include <chrono> // 스레드와 시간 관련 헤더 파일

using json = nlohmann::json; // JSON 라이브러리 사용

#define PORT_NUM 9000 // 서버 포트 번호
#define BUFSIZE 1024  // 버퍼 사이즈

// #define tmp_ip_address "127.0.0.1" // 임시 ip 주소

// #include <nlohmann/json.hpp>
using json = nlohmann::json;
namespace fs = std::filesystem;
using namespace std;

std::vector<fs::path> jsonFiles;
std::string directoryPath = "../msgdata"; // JSON 파일이 있는 디렉토리 경로

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


void clientMessage(const std::string &dst_id, const json &msg)
{

    std::string ip_address;

    char buffer[BUFSIZE];              // 버퍼 선언
    memset(buffer, 0, sizeof(buffer)); // 버퍼 초기화
    int clientSocketfd;                // 클라이언트 소켓 파일 디스크립터

    /*
        1. socket()        // 소켓 생성  --> 클라이언트로 다른 서버에게 알리는 일 braodcast 때와 req_prepay 때만 생성하면 된다.
        2. connect()       // 서버에 연결 요청 -->
        3. write()/send()  // 서버에 데이터 전송 --> 클라이언트가 매개변수로 받은 데이터를 서버에 전송한다.
        4. read()/recv()   // 서버로부터 데이터 수신 --> 서버가 응답한 데이터를 수신한다. 즉 서버가 ACK를 보내기 전까지 닫으면 안 된다.
        5. close()         // 소켓 닫기 --> 서버가 응답한 데이터를 수신한 후 소켓을 닫는다.
    */

    // 1. socket() --> 클라이언트 소켓 생성
    try
    {
        clientSocketfd = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocketfd < 0)
            throw clientSocketfd;
        std::cout << "Client Socket created" << std::endl;
    }
    catch (int clientsocketfd)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return;
    }

#pragma region socketconnect
    // JSON 파일에서 dst_id에 따른 IP주소 가져오기
    std::ifstream file("../msgdata/ip_address.json");
    if (!file.is_open())
    {
        std::cerr << "[ERROR] ip_address.json 파일을 열 수 없습니다." << std::endl;
        return;
    }

    nlohmann::json ip_data;
    try
    {
        file >> ip_data;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR] JSON 파싱 실패: " << e.what() << std::endl;
        return;
    }

    if (ip_data.contains(dst_id))
    {
        ip_address = ip_data[dst_id].get<std::string>();
    }
    else
    {
        std::cerr << "[ERROR] 해당 key가 존재하지 않습니다: " << dst_id << std::endl;
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;                              // IPv4
    server_addr.sin_port = htons(PORT_NUM);                        // 포트 번호 설정
    inet_pton(AF_INET, ip_address.c_str(), &server_addr.sin_addr); // IP 주소 변환

    std::cout << "[" << dst_id << "] Server address set" << std::endl;

    if (connect(clientSocketfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "connect failed. errno=" << errno << " : " << strerror(errno) << std::endl;
        if (errno == ECONNREFUSED)
        {
            std::cerr << " 서버가 연결을 거부했습니다 (ECONNREFUSED)" << std::endl;
        }
        close(clientSocketfd);
        return;
    }
#pragma endregion

    std::cout << "[" << dst_id << "] Connected to server" << std::endl;

    std::string jsonStrServer = msgFormat(msg["msg_type"], dst_id, msg["msg_content"]["item_code"], msg["msg_content"]["item_num"], msg["msg_content"]["coor_x"], msg["msg_content"]["coor_y"], msg["msg_content"]["cert_code"], msg["msg_content"]["availability"]);

    if (msg["msg_type"] == "req_stock")
    {
        std::cout << "[" << dst_id << "] Sending stock request" << std::endl;
    }
    else if (msg["msg_type"] == "req_prepay")
    {
        std::cout << "[" << dst_id << "] Sending prepay request" << std::endl;
    }
    else
    {
        std::cerr << "[" << dst_id << "] Unknown message type" << std::endl;
        close(clientSocketfd);
        return;
    }
    send(clientSocketfd, jsonStrServer.c_str(), jsonStrServer.length(), 0);
    std::cout << "[" << dst_id << "] Sent JSON: " << std::endl;

    int valread = recv(clientSocketfd, buffer, BUFSIZE, 0);
    if (valread > 0)
    {
        buffer[valread] = '\0';
        std::cout << "[" << dst_id << "] Received message" << std::endl;

        // JSON 파싱
        json msg = json::parse(buffer);
        std::cout << "[" << dst_id << "] Parsed message: " << msg.dump(2) << std::endl;
    }
    else
    {
        std::cerr << "[" << dst_id << "] Failed to receive message" << std::endl;
    }

    if (msg["msg_type"] == "resp_stock")
    {
        std::cout << "[" << dst_id << "] Stock request ACK received" << std::endl;
        // 각각의 재고 확인 메시지를 json 파일 형식으로 저장
        std::string fileName = "../msgdata/" + dst_id + "_stock.json";
        std::ofstream outFile(fileName);
        if (outFile.is_open())
        {
            outFile << msg.dump(2); // 2칸 들여쓰기로 예쁘게 출력
            outFile.close();
            std::cout << "[" << dst_id << "] Stock data saved to " << fileName << std::endl;
        }
        else
        {
            std::cerr << "[" << dst_id << "] Failed to open file for writing" << std::endl;
        }
    }
    else if (msg["msg_type"] == "req_prepay")
    {
        std::cout << "[" << dst_id << "] Prepay request ACK received" << std::endl;
    }
    else
    {
        std::cerr << "[" << dst_id << "] Unknown message type" << std::endl;
    }
    close(clientSocketfd); // 클라이언트 소켓 닫기
    std::cout << "[" << dst_id << "] Client socket closed" << std::endl;
    return;
}

void MSG::handleClient(int client_socket)
{
    char buffer[BUFSIZ] = {0};
    int valread = recv(client_socket, buffer, BUFSIZ, 0);
    if (valread > 0)
    {
        buffer[valread] = '\0';
        std::cout << "Received message: " << buffer << std::endl;

        json msg = json::parse(buffer);
        std::cout << "[Server] Parsed message: " << msg.dump(2) << std::endl;

        // 클라이언트 메시지에 따라 ACK를 다르게
        if (msg["msg_type"] == "req_stock")
        {
            std::cout << "[Server] Stock request received" << std::endl;
            // 재고 확인 요청 처리
            AskStockMessage(msg);
        }
        else if (msg["msg_type"] == "req_prepay")
        {
            std::cout << "[Server] Prepay request received" << std::endl;
            // 인증번호 전송 처리
            sendCertCode(msg["dst_id"], msg["msg_content"]["item_code"], msg["msg_content"]["item_num"], msg["msg_content"]["cert_code"]);
        }
        else
        {
            std::cerr << "[Server] Unknown message type" << std::endl;
        }
    }

    close(client_socket);
}

void MSG::serverMessageOpen()
{
        /*
            1. socket()        // 소켓 생성 --> 서버 소켓 생성 main 앞 부분에서 실행해야 하는 부분
            2. bind()          // IP주소와 포트번호를 소켓에 할당
            3. listen()        // 연결 요청 대기 상태

            이후 부분은 while문 안에서 반복적으로 실행되어야 한다.
            4. accept()        // 클라이언트 연결 수락
            5. read()/recv()   // 클라이언트로부터 데이터 수신
            6. write()/send()  // 클라이언트로 데이터 송신
            7. close()         // 소켓 닫기
        */
        char buffer[BUFSIZ]; // 버퍼 선언

    #pragma region socketcreate
        serverSocketfd = socket(AF_INET, SOCK_STREAM, 0); // 서버 소켓 생성
        if (serverSocketfd < 0)                           // 소켓 생성 실패 시 에러 메시지 출력
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
        if (::bind(serverSocketfd, (struct sockaddr *)&address, sizeof(address)) < 0) // bind 실패 시 에러 메시지 출력
        {
            std::cerr << "Bind failed. errno=" << errno << " : " << strerror(errno) << std::endl;
            close(serverSocketfd); // 소켓 닫기
            return;
        }
        std::cout << "Bind successful" << std::endl; // 바인드 성공 시 출력
    #pragma endregion

    #pragma region listen
        listen(serverSocketfd, 8);                          // 연결 요청 대기 상태
        std::cout << "Listening on port 9000" << std::endl; // 연결 요청 대기 상태 출력
    #pragma endregion

        while (true)
        {
            socklen_t addrlen = sizeof(struct sockaddr_in);
            int client_socket = accept(serverSocketfd, nullptr, nullptr);
            if (client_socket < 0)
            {
                std::cerr << "Accept failed" << std::endl;
                continue;
            }

            std::cout << "Client connected" << std::endl;

            // 클라이언트 처리 스레드 생성
            std::thread clientThread(&MSG::handleClient, this, client_socket);
            clientThread.detach(); // 클라이언트별로 비동기 처리
        }

        close(serverSocketfd); // 서버 소켓 닫기
        std::cout << "Server socket closed" << std::endl;
}

void clientSendMessage(int sockfd, const std::string &msg)
{
    // 메시지를 send 했어
    try
    {
        if (::send(sockfd, msg.c_str(), msg.length(), 0) < 0)
        {
            throw std::runtime_error("Send failed");
        }
        else
        {
            std::cout << "Message sent" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Exception] " << e.what() << std::endl;
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    int len = recv(sockfd, buffer, sizeof(buffer), 0);
    if (len > 0)
    {
        std::cout << "[ACK Received] " << buffer << std::endl;
    }
    else
    {
        std::cerr << "[ACK Error or Disconnected]" << std::endl;
    }
}

void broadMessage(const json &msg)
{
    std::vector<std::thread> threads;
    std::cout << "[Broadcast] Sending message to all" << std::endl;
    for (int i = 1; i < 9; ++i)
    {
        std::string dst_id = "T" + std::to_string(i);

        if (dst_id == "T4")
        {
            continue; // T4는 자기 자신에게 보내지 않도록
        }
        threads.emplace_back([msg, dst_id]()
        {
            try {
                clientMessage(dst_id, msg);
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
void sendCertCode(const std::string &dst_id, const std::string &item_code, const std::string &item_num, const std::string &cert_code)
{
    std::cout << "[Send] Authentication Number" << std::endl;
    json DVMMessageOutOfStock_MessageFormat = {
        {"msg_type", "req_prepay"},
        {"src_id", "T4"},
        {"dst_id", dst_id},
        {"msg_content", {{"item_code", item_code}, {"item_num", item_num}, {"coor_x", ""}, {"coor_y", ""}, {"cert_code", cert_code}, {"availability", ""}}}};
}

// 클라이언트 소켓을 생성하고 타 서버에 연결하는 함수를 구현
std::vector<std::string> DVMMessageOutofStock(int beverageId, int quantity)
{
    // 1. 브로드 캐스트를 이용해서 json 메시지를 받아온다.
    std::cout << "[Out of Stock] Beverage ID: " << beverageId << ", Quantity: " << quantity << std::endl; // 재고 부족 메시지 출력
    json DVMMessageOutofStock_MessageFormat = {
        {"msg_type", "req_stock"},
        {"src_id", "T4"},
        {"dst_id", "0"},
        {"msg_content", {{"item_code", std::to_string(beverageId)}, {"item_num", std::to_string(quantity)}, {"coor_x", std::to_string(0)}, {"coor_y", std::to_string(0)}, {"cert_code", ""}, {"availability", ""}}}};

    broadMessage(DVMMessageOutofStock_MessageFormat); // 재고 부족 메시지 전송

    // 2. 재고 메시지를 하나하나 calc에 보내준다.

    // 2.1 디렉토리 내 모든 .json 파일 수집
    for (const auto &entry : fs::recursive_directory_iterator(directoryPath))
    {
        if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".json")
        {
            jsonFiles.push_back(entry.path());
        }
    }
    // calc.nearestPosition(); // 거리 계산

    return {msgFormat("req_prepay", "T4", std::to_string(beverageId), std::to_string(quantity), std::to_string(0), std::to_string(0), "", "")}; // 인증번호 전송
}

// 다른 DVM에서 재고 확인 요청을 받았을 때 호출되는 함수
json MSG::AskStockMessage(json msg)
{
    std::cout << "[Ask Stock] Stock으로부터 확인 중" << msg << std::endl;


    Stock stock;
    stock.isBuyable(msg["msg_content"]["item_code"], msg["msg_content"]["item_num"]); // 재고 확인
    std::string resp_stock_msg = msgFormat("resp_stock", msg["src_id"], msg["msg_content"]["item_code"], msg["msg_content"]["item_num"], msg["msg_content"]["coor_x"], msg["msg_content"]["coor_y"], "", ""); // 재고 확인 메시지 포맷
    json resp_stock_msg;                                                                                                                                                                                       // 파싱된 JSON 메시지 저장 변수
    try
    {
        resp_stock_msg = json::parse(resp_stock_msg); // JSON 메시지 파싱
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR] JSON 파싱 실패: " << e.what() << std::endl;
        return json(); // 빈 JSON 반환
    }
    std::cout << "[Ask Stock] Parsed message: " << resp_stock_msg << std::endl; // 파싱된 메시지 출력
    return resp_stock_msg;
}

void SocketOpenInit(MSG *msg)
{
    std::thread serverThread = std::thread(&MSG::serverMessageOpen, msg); // 서버 수신 함수 백그라운드 실행
    serverThread.detach();                                     // 또는 joinable일 때 main에서 join (비차단 운영이면 detach)
}

bool sendMessage(const std::string msg_type, const std::string &msg)
{
    // 클라이언트 소켓을 생성하고 타 서버에 연결하는 함수를 구현
    std::cout << "[Send Message] " << msg_type << std::endl;
    clientMessage(msg_type, msg);
    return true;
}

int main()
{
    MSG msg;
    SocketOpenInit(&msg); // 소켓 초기화

    DVMMessageOutofStock(20, 99); // 재고 부족 메시지 전송

    return 0;
}
