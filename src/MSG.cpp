// MSG.cpp
#include "MSG.hpp"
#include "Stock.hpp"
#include "Position.hpp"
#include "Beverage.hpp"
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
#include <sys/stat.h>   //파일 상태를 확인하기 위한 헤더 파일
#include "Position.hpp"
#include <filesystem>
#include <vector>
#include <algorithm>
#include <regex>
#include <stdexcept>

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

std::string directoryPath = "../msgdata"; // JSON 파일이 있는 디렉토리 경로
std::vector<fs::path> jsonFiles;

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

int serverSocketfd;

void clientMessage(const std::string &dst_id, const json &msg)
{
    std::string ip_address;

    char buffer[BUFSIZE];              // 버퍼 선언
    memset(buffer, 0, sizeof(buffer)); // 버퍼 초기화

    int clientSocketfd;                // 클라이언트 소켓 파일 디스크립터

    /*
        1. socket()        // 소켓 생성  --> 클라이언트로 다른 서버에게 알리는 일 broadcast 때와 req_prepay 때만 생성하면 된다.
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
        // std::cout << "Client Socket created" << std::endl;
    }
    catch (int clientsocketfd)
    {
        // std::cerr << "Socket creation failed" << std::endl;
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

    // std::cout << "[" << dst_id << "] Server address set" << std::endl;

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

    std::cout << "소켓 생성하고 연결하고 완료" << std::endl;

    // 메시지 전송
    std::string request_msg = msgFormat(
        msg["msg_type"],
        dst_id,
        std::to_string(msg["msg_content"]["item_code"].get<int>()),
        std::to_string(msg["msg_content"]["item_num"].get<int>()),
        std::to_string(msg["msg_content"]["coor_x"].get<int>()),
        std::to_string(msg["msg_content"]["coor_y"].get<int>()),
        "",
        "");

    json parsed_request_msg;
    parsed_request_msg = json::parse(request_msg); // JSON 메시지 파싱

    std::cout << msg["msg_type"] << std::endl;

    if (parsed_request_msg["msg_type"] == "req_stock") {
        std::string send_req_stock_msg = parsed_request_msg.dump(2); // JSON 문자열로 변환
        send(clientSocketfd, send_req_stock_msg.c_str(), send_req_stock_msg.length(), 0);
    }
    else if (parsed_request_msg["msg_type"] == "req_prepay") {
        parsed_request_msg["msg_content"]["cert_code"] = msg["msg_content"]["cert_code"];

        std::string send_req_prepay_msg = parsed_request_msg.dump(2); // JSON 문자열로 변환
        send(clientSocketfd, send_req_prepay_msg.c_str(), send_req_prepay_msg.length(), 0);
    }
    else {
        cerr << "없는 request type입니다. " << endl;
        return ;
    }


    // 메시지 수신
    int valread = recv(clientSocketfd, buffer, BUFSIZE, 0);
    json recv_parsing_msg;
    if (valread > 0)
    {
        buffer[valread] = '\0';
        // std::cout << "[" << dst_id << "] Received message" << std::endl;

        // JSON 파싱
        recv_parsing_msg = json::parse(buffer);
        // std::cout << "[" << dst_id << "] Parsed message: " << recv_parsing_msg.dump(2) << std::endl;
    }
    else
    {
        std::cerr << "[" << dst_id << "] Failed to receive message" << std::endl;
    }

    std::cout << "메시지를 받는 중입니다." << std::endl;

    // std::cout << "[" << dst_id << "] Received message type: " << recv_parsing_msg["msg_type"] << std::endl;
    if (recv_parsing_msg["msg_type"] == "resp_stock")
    {
        cout<<recv_parsing_msg<<endl;
        // std::cout << "[" << dst_id << "] Stock request ACK received" << std::endl;
        // 각각의 재고 확인 메시지를 json 파일 형식으로 저장
        std::string fileName = "../msgdata/" + dst_id + "_stock.json";
        std::ofstream outFile(fileName);
        if (outFile.is_open())
        {
            
            outFile << msg.dump(2); // 2칸 들여쓰기로 예쁘게 출력
            outFile.close();
            // std::cout << "[" << dst_id << "] Stock data saved to " << fileName << std::endl;
        }
        else
        {
            std::cerr << "[" << dst_id << "] Failed to open file for writing" << std::endl;
        }
    }
    else if (recv_parsing_msg["msg_type"] == "resp_prepay")
    {
        // std::cout << "[" << dst_id << "] Prepay request ACK received" << std::endl;

        if (recv_parsing_msg["msg_content"]["availability"] == "true")
        {
            // std::cout << "[" << dst_id << "] Prepay request successful" << std::endl;
            // 인증번호 전송 처리 완료
            return;
        }
        else
        {
            // std::cout << "[" << dst_id << "] Prepay request failed" << std::endl;
            return;
            // ROLLBACK 처리 
        }
    }
    else
    {
        std::cerr << "[" << dst_id << "] Unknown message type" << std::endl;
    }
    close(clientSocketfd); // 클라이언트 소켓 닫기
    // std::cout << "[" << dst_id << "] Client socket closed" << std::endl;
    return;
}

void MSG::handleClient(int client_socket)
{
    char buffer[BUFSIZ] = {0};
    int valread = recv(client_socket, buffer, BUFSIZ, 0);
    if (valread > 0)
    {
        buffer[valread] = '\0';
        // std::cout << "Received message: " << buffer << std::endl;

        json msg = json::parse(buffer);

        // 클라이언트 메시지에 따라 ACK를 다르게
        if (msg["msg_type"] == "req_stock")
        {
            // std::cout << "[Server] Stock request received" << std::endl;
            json read_ = MSG::AskStockMessage(msg);

            // Stock을 통해 item_code와 , item_num을 호출해야 하면 coor_x, coor_y도 가져와야 한다.

            std::string resp_stock_msg = msgFormat(
                "resp_stock",
                read_["src_id"],
                std::to_string(read_["msg_content"]["item_code"].get<int>()),
                std::to_string(read_["msg_content"]["item_num"].get<int>()),
                std::to_string(read_["msg_content"]["coor_x"].get<int>()),
                std::to_string(read_["msg_content"]["coor_y"].get<int>()),
                "",
                "");

            json parsed_resp_stock_msg;
            parsed_resp_stock_msg = json::parse(resp_stock_msg); // JSON 메시지 파싱인가?
            std::string resp_stock_msg_to_string = parsed_resp_stock_msg.dump(2);
            ::send(client_socket, resp_stock_msg_to_string.c_str(), resp_stock_msg_to_string.size(), 0); // 클라이언트에게 ACK 메시지 전송
            // 재고 확인 요청 처리 
            // json read_ = AskStockMessage(msg);

            
        }
        else if (msg["msg_type"] == "req_prepay") // 현재 재고 상태를 확인 후 resp_prepay Availiability를 T나 F로 보낸다.
        {
            Stock stock;
            // std::cout << "[Server] Prepay request received" << std::endl;
            // list<Beverage> beverage_list = stock.getCurrentStock(); //..? stock에서 
            json read_ = AskStockMessage(msg); // 재고 확인 메시지 포맷 현재 재고 상태에 대해 전송이 아닌 현재 재고 상태를 확인만 하고 T F를 보내기만 하면 된다.

            ::send(client_socket, read_.dump(2).c_str(), read_.dump(2).size(), 0); // 클라이언트에게 ACK 메시지 전송
            
            // 인증번호 전송 처리
            // sendCertCode(msg["dst_id"], msg["msg_content"]["item_code"], msg["msg_content"]["item_num"], msg["msg_content"]["cert_code"]);
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
    // std::cout << "Bind successful" << std::endl; // 바인드 성공 시 출력
#pragma endregion

#pragma region listen
    listen(serverSocketfd, 8);                          // 연결 요청 대기 상태
    // std::cout << "Listening on port 9000" << std::endl; // 연결 요청 대기 상태 출력
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

        // std::cout << "Client connected" << std::endl;

        // 클라이언트 처리 스레드 생성
        std::thread clientThread(&MSG::handleClient, this, client_socket);        
        clientThread.detach(); // 클라이언트별로 비동기 처리
    }

    close(serverSocketfd); // 서버 소켓 닫기
    // std::cout << "Server socket closed" << std::endl;
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
            // std::cout << "Message sent" << std::endl;
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
        // std::cout << "[ACK Received] " << buffer << std::endl;
    }
    else
    {
        std::cerr << "[ACK Error or Disconnected]" << std::endl;
    }
}

// client threads들이 다른 서버와 연결 후 recv까지 하는 함수
void MSG::broadMessage(const std::string &msg)
{
    std::vector<std::thread> threads;
    // 테스트로 1번으로 줄였습니다.
    for (int i = 1; i < 2; ++i)
    {
        std::string dst_id = "T" + std::to_string(i);

        if (dst_id == "T4")
        {
            continue; // T4는 자기 자신에게 보내지 않도록
        }
        json req_msg = json::parse(msg);
        threads.emplace_back([dst_id, req_msg]()
        {
            try {
                clientMessage(dst_id, req_msg);
            } catch (const std::exception& e) {
                std::cerr << "[Exception] " << e.what() << std::endl;
            } });
    }

    for (auto &t : threads)
    {
        if (t.joinable())
            t.join();
    }

    // std::cout << "[Broadcast] 모든 메시지 전송 완료" << std::endl;

    return;
}

// 인증번호를 전송하는 함수 선결제 영역
json sendCertCode(const json &msg)
{
    std::cout << "[Send] Authentication Number" << std::endl;

    std::string resp_stock_msg = msgFormat(
        "resp_stock",
        msg["dst_id"],
        std::to_string(msg["msg_content"]["item_code"].get<int>()),
        std::to_string(msg["msg_content"]["item_num"].get<int>()),
        std::to_string(msg["msg_content"]["coor_x"].get<int>()),
        std::to_string(msg["msg_content"]["coor_y"].get<int>()),
        "",
        "");

    json parsed_resp_stock_msg = json::parse(resp_stock_msg);

    return parsed_resp_stock_msg;

    // std::cout << "[]"
}

// 클라이언트 소켓을 생성하고 타 서버에 연결하는 함수를 구현
std::tuple<int,int, std::string> MSG::DVMMessageOutofStock(int beverageId, int quantity)
{
    // 1. 브로드 캐스트를 이용해서 json 메시지를 받아온다.
    // std::cout << "[Out of Stock] Beverage ID: " << beverageId << ", Quantity: " << quantity << std::endl; // 재고 부족 메시지 출력

    std::string DVMMessageOutofStock_MessageFormat = msgFormat(
        "req_stock",
        "",
        std::to_string(beverageId),
        std::to_string(quantity),
        "",
        "",
        "",
        "");
    json parsed_req_stock_msg;
    parsed_req_stock_msg = json::parse(DVMMessageOutofStock_MessageFormat); // JSON 메시지 파싱
    broadMessage(DVMMessageOutofStock_MessageFormat);                                     // 재고 부족 메시지 전송

    // 2. 재고 메시지를 하나하나 Position에 보내준다.

    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // 2.1 디렉토리 내 모든 .json 파일 수집
    for (const auto &entry : fs::recursive_directory_iterator(directoryPath))
    {
        if (std::filesystem::is_regular_file(entry) && entry.path().extension() == ".json")
        {
            jsonFiles.push_back(entry.path());
        }
    }
    float shortest_distance = std::numeric_limits<float>::max();
    std::string shortest_id;
    int nearest_x = -1;
    int nearest_y = -1;
    Position pos;
    for (const auto& path : jsonFiles)
    {
        
        try {
            std::ifstream file(path);
            json j;
            file >> j;

            int x = 0;
            if (j["msg_content"]["coor_x"].is_string()) {
                x = std::stoi(j["msg_content"]["coor_x"].get<std::string>());
            } else if (j["msg_content"]["coor_x"].is_number_integer()) {
                x = j["msg_content"]["coor_x"].get<int>();
            }
            int y = 0;
            if (j["msg_content"]["coor_y"].is_string()) {
                y = std::stoi(j["msg_content"]["coor_y"].get<std::string>());
            } else if (j["msg_content"]["coor_y"].is_number_integer()) {
                y = j["msg_content"]["coor_y"].get<int>();
            }
            std::string src_id = j["src_id"];
            
            int otherDVMstock = 0;
            if (j["msg_content"]["item_num"].is_string()) {
                otherDVMstock = std::stoi(j["msg_content"]["coor_y"].get<std::string>());
            } else if (j["msg_content"]["item_num"].is_number_integer()) {
                otherDVMstock = j["msg_content"]["item_num"].get<int>();
            }
            

            if(otherDVMstock<quantity){
                float distance = pos.calcDistance(x, y);
                if(distance<shortest_distance){
                    shortest_distance =distance;
                    shortest_id = src_id;
                    nearest_x = x;
                    nearest_y = y;
                }
            }
        }catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to process file " << path << ": " << e.what() << std::endl;
        continue;
        }
    } 
    // std::cout << "[Out of Stock] Nearest Position: " << nearest_x << ", " << nearest_y << ", ID: " << shortest_id << std::endl;
    return {nearest_x, nearest_y, shortest_id};
}
// // 재고 확인 요청 메시지 처리 Server에서 다른 클라이언트부터 요청을 받았을 때
// json AskStockMessage(json msg)

// 다른 DVM에서 재고 확인 요청을 받았을 때 호출되는 함수 --> 서버가 받은 메시지에서 다시 ACK로 보내는 메시지를 반환하는 함수
json MSG::AskStockMessage(json msg)
{
    // std::cout << "[Ask Stock] Stock으로부터 확인 중" << msg << std::endl;


    Stock stock;
    bool canBuy = !stock.isPrepayment(msg["msg_content"]["item_code"], msg["msg_content"]["item_num"]); // 재고 확인
    
    cout<<"canBuy"<<canBuy<<endl;
    
    if(canBuy==false){
          std::string resp_stock_msg = msgFormat(
        "resp_stock",
        msg["src_id"].get<std::string>(),
        std::to_string(msg["msg_content"]["item_code"].get<int>()),
        std::to_string(0),
        std::to_string(msg["msg_content"]["coor_x"].get<int>()),
        std::to_string(msg["msg_content"]["coor_y"].get<int>()),
        "",
        "");
        json parsed_resp_stock_msg;                                                                                                                                                                                      // 파싱된 JSON 메시지 저장 변수
        try
        {
            parsed_resp_stock_msg = json::parse(resp_stock_msg); // JSON 메시지 파싱
        }
        catch (const std::exception &e)
        {
            std::cerr << "[ERROR] JSON 파싱 실패: " << e.what() << std::endl;
            return json(); // 빈 JSON 반환
        }
        // std::cout << "[Ask Stock] Parsed message: " << resp_stock_msg << std::endl;
        return parsed_resp_stock_msg;
    }

    std::string resp_stock_msg = msgFormat(
        "resp_stock",
        msg["src_id"].get<std::string>(),
        std::to_string(msg["msg_content"]["item_code"].get<int>()),
        std::to_string(msg["msg_content"]["item_num"].get<int>()),
        std::to_string(msg["msg_content"]["coor_x"].get<int>()),
        std::to_string(msg["msg_content"]["coor_y"].get<int>()),
        "",
        "");
    json parsed_resp_stock_msg;                                                                                                                                                                                      // 파싱된 JSON 메시지 저장 변수
    try
    {
        parsed_resp_stock_msg = json::parse(resp_stock_msg); // JSON 메시지 파싱
    }
    catch (const std::exception &e)
    {
        std::cerr << "[ERROR] JSON 파싱 실패: " << e.what() << std::endl;
        return json(); // 빈 JSON 반환
    }
    // std::cout << "[Ask Stock] Parsed message: " << resp_stock_msg << std::endl;
    return parsed_resp_stock_msg;

    // 재고 확인 요청 메시지 처리
    // 일단 동작은 막아놓았다.
    // Stock stock;
    // stock.isBuyable(msg["msg_content"]["cert_code"], msg["msg_content"]["item_code"], msg["msg_content"]["item_num"]); // 재고 확인
    // std::string resp_stock_msg = msgFormat("resp_stock", msg["src_id"], msg["msg_content"]["item_code"], msg["msg_content"]["item_num"], msg["msg_content"]["coor_x"], msg["msg_content"]["coor_y"], "", ""); // 재고 확인 메시지 포맷
    // json resp_stock_msg;                                                                                                                                                                                      // 파싱된 JSON 메시지 저장 변수
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

void MSG::SocketOpenInIt(MSG *msg)
{
    std::thread serverThread = std::thread(&MSG::serverMessageOpen, msg); // 서버 수신 함수 백그라운드 실행
    serverThread.detach();                                     // 또는 joinable일 때 main에서 join (비차단 운영이면 detach)
}

bool MSG::sendMessage(const std::tuple<std::string, int, int, std::string>& msgData)
{

    const auto& [dst_id, itemID, itemNum, newCertCode] = msgData;

    // 클라이언트 소켓을 생성하고 타 서버에 연결하는 함수를 구현
    string path = directoryPath+"ip_address.json";
    std::ifstream file(path);
    json ipjson;
    file >> ipjson;
    string ipaddress = ipjson[dst_id];

    string msg_type = "req_prepay";

    string jsonstr = msgFormat(msg_type,dst_id, std::to_string(itemID), std::to_string(itemNum), "","",newCertCode,"");
    // std::cout << "[Send Message] " << msg_type << std::endl;
    json msg = json(jsonstr);
    clientMessage(dst_id, msg);


    // 선결제 무조건 가능하다고 가정
    return true;
}