#include "MSG.hpp"
#include "Stock.hpp"
#include "Position.hpp"
#include "Beverage.hpp"

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

const int PORT_NUM = 9000; // 서버 포트 번호
const int BUFSIZE = 1024;

using json = nlohmann::json;
namespace fs = std::filesystem;

const std::string directoryPath = "../msgdata/stock/"; // JSON 파일이 있는 디렉토리 경로
bool isPrepayValid; // 다른 서버의 메시지의 ["msg_content"]["availability"]가 true인지 false인지 확인하는 변수

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
        msg["msg_content"]["item_code"] = std::stoi(item_code);
        if (msg["msg_content"]["item_code"] > 20)
        {
            throw std::out_of_range("item_code는 21 이상일 수 없습니다.");
        }
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "[Error] 숫자가 아닌 값을 입력했습니다: " << e.what() << std::endl;
        msg["msg_content"]["item_code"] = 0;
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "[Error] 값이 너무 큽니다: " << e.what() << std::endl;
        msg["msg_content"]["item_code"] = 0;
    }

    try
    {
        msg["msg_content"]["item_num"] = std::stoi(item_num);
        if (msg["msg_content"]["item_num"] > 99)
        {
            throw std::out_of_range("item_num은 100 이상일 수 없습니다.");
        }
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "[Error] 숫자가 아닌 값을 입력했습니다: " << e.what() << std::endl;
        msg["msg_content"]["item_num"] = 0;
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "[Error] item_num 변환 실패: " << e.what() << std::endl;
        msg["msg_content"]["item_num"] = 0;
    }

    try
    {
        msg["msg_content"]["coor_x"] = std::stoi(coor_x);
        if (msg["msg_content"]["coor_x"] < 0 || msg["msg_content"]["coor_x"] > 99)
        {
            throw std::out_of_range("coor_x는 0 이상 100 이하의 값이어야 합니다.");
        }
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "[Error] coor_x 숫자가 아닌 값을 입력했습니다: " << e.what() << std::endl;
        msg["msg_content"]["coor_x"] = 0;
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "[Error] coor_x 변환 실패: " << e.what() << std::endl;
        msg["msg_content"]["coor_x"] = 0;
    }

    try
    {
        msg["msg_content"]["coor_y"] = std::stoi(coor_y);
        if (msg["msg_content"]["coor_y"] < 0 || msg["msg_content"]["coor_y"] > 99)
        {
            throw std::out_of_range("coor_y는 0 이상 100 이하의 값이어야 합니다.");
        }
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "[Error] coor_y 숫자가 아닌 값을 입력했습니다: " << e.what() << std::endl;
        msg["msg_content"]["coor_y"] = 0;
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "[Error] coor_y 변환 실패: " << e.what() << std::endl;
        msg["msg_content"]["coor_y"] = 0;
    }

    msg["msg_content"]["cert_code"] = cert_code;
    msg["msg_content"]["availability"] = availability;

    return msg.dump(2);
}

int serverSocketfd;

void clientMessage(const std::string &dst_id, const json &msg)
{
    std::string ip_address;

    std::array<char, BUFSIZE> buffer;              // 버퍼 선언
    memset(buffer.data(), 0, sizeof(buffer)); // 버퍼 초기화

    int clientSocketfd;                // 클라이언트 소켓 파일 디스크립터


    try
    {
        clientSocketfd = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocketfd < 0)
            throw clientSocketfd;
    }
    catch (int clientSocketfd)
    {
        std::cerr << "Socket creation failed" << clientSocketfd << std::endl;
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
    catch (const nlohmann::json::parse_error &e)
    {
        std::cerr << "[ERROR] JSON 파싱 오류 (구문 오류): " << e.what() << std::endl;
        return;
    }
    catch (const nlohmann::json::type_error &e)
    {
        std::cerr << "[ERROR] JSON 타입 오류 (예상과 다른 타입): " << e.what() << std::endl;
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
    server_addr.sin_family = AF_INET;                              
    server_addr.sin_port = htons(PORT_NUM);                        
    inet_pton(AF_INET, ip_address.c_str(), &server_addr.sin_addr); 

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

    if (parsed_request_msg["msg_type"] == "req_stock") {
        std::string send_req_stock_msg = parsed_request_msg.dump(2); // JSON 문자열로 변환
        send(clientSocketfd, send_req_stock_msg.c_str(), send_req_stock_msg.length(), 0);
    }
    else if (parsed_request_msg["msg_type"] == "req_prepay") {
        parsed_request_msg["msg_content"]["cert_code"] = msg["msg_content"]["cert_code"];

        std::string send_req_prepay_msg = parsed_request_msg.dump(2); 
        send(clientSocketfd, send_req_prepay_msg.c_str(), send_req_prepay_msg.length(), 0);
    }
    else {
        cerr << "없는 request type입니다. " << endl;
        return ;
    }


    // 메시지 수신전에 버퍼 초기화
    memset(buffer.data(), 0, sizeof(buffer));
    ssize_t valread = recv(clientSocketfd, buffer.data(), BUFSIZE, 0);
    json recv_parsing_msg;
    if (valread > 0)
    {
        // JSON 파싱
        recv_parsing_msg = json::parse(buffer.begin(), buffer.begin() + valread);
    }
    else
    {
        std::cerr << "[" << dst_id << "] Failed to receive message" << std::endl;
    }

    if (recv_parsing_msg["msg_type"] == "resp_stock")
    {
        // 각각의 재고 확인 메시지를 json 파일 형식으로 저장
        std::string fileName = "../msgdata/stock/" + dst_id + "_stock.json";
        std::ofstream outFile(fileName);
        if (outFile.is_open())
        {
            outFile << recv_parsing_msg.dump(2); // 2칸 들여쓰기로 예쁘게 출력
            outFile.close();
        }
        else
        {
            std::cerr << "[" << dst_id << "] Failed to open file for writing" << std::endl;
        }
    }
    else if (recv_parsing_msg["msg_type"] == "resp_prepay")
    {

        if (recv_parsing_msg["msg_content"]["availability"] == "T")
        {
            isPrepayValid = true;
            // 인증번호 전송 처리 완료
            return;
        }
        else
        {
            isPrepayValid = false;
            return;
            // ROLLBACK 처리 
        }
    }
    else
    {
        std::cerr << "[" << dst_id << "] Unknown message type" << std::endl;
    }
    close(clientSocketfd); // 클라이언트 소켓 닫기
    return;
}

void MSG::handleClient(int client_socket)
{
    std::array<char, BUFSIZE> buffer;         // 버퍼 선언
    memset(buffer.data(), 0, sizeof(buffer)); // 버퍼 초기화

    if (ssize_t valread = recv(client_socket, buffer.data(), BUFSIZ, 0) > 0)
    {
        buffer[valread] = '\0';

        json msg = json::parse(buffer.begin(), buffer.begin() + valread);

        // 클라이언트 메시지에 따라 ACK를 다르게
        if (msg["msg_type"] == "req_stock")
        {
            json read_ = MSG::AskStockMessage(msg);

            // Stock을 통해 item_code와 , item_num을 호출해야 하면 coor_x, coor_y도 가져와야 한다.

            std::string resp_stock_msg = msgFormat(
                "resp_stock",
                read_["dst_id"],
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

            
        }
        else if (msg["msg_type"] == "req_prepay") // 현재 재고 상태를 확인 후 resp_prepay availability를 T나 F로 보낸다.
        {
            Stock stock;

            bool availability = stock.isBuyable(msg["msg_content"]["cert_code"], msg["msg_content"]["item_code"], msg["msg_content"]["item_num"]); // 재고 확인 메시지 포맷 현재 재고 상태에 대해 전송이 아닌 현재 재고 상태를 확인만 하고 T F를 보내기만 하면 된다.

            std::string resp_prepay_msg = msgFormat(
                "resp_prepay",
                msg["src_id"],
                std::to_string(msg["msg_content"]["item_code"].get<int>()),
                std::to_string(msg["msg_content"]["item_num"].get<int>()),
                "", // 필요없는 정보 x좌표
                "", // 필요없는 정보 y좌표
                "",
                availability? "T" : "F"); // availability는 T나 F로 보내야 한다.

            ::send(client_socket, resp_prepay_msg.c_str(), resp_prepay_msg.size(), 0); // 클라이언트에게 ACK 메시지 전송

        }
        else
        {
            std::cerr << "[Server] Unknown message type" << std::endl;
        }
    }
    // 클라이언트 종료와 함께 소켓 닫기
    close(client_socket);
}

void MSG::serverMessageOpen()
{
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
#pragma endregion

#pragma region listen
    listen(serverSocketfd, 8);     // 연결 요청 대기 상태
#pragma endregion

    while (true)
    {
        int client_socket = accept(serverSocketfd, nullptr, nullptr);
        if (client_socket < 0)
        {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }


        // 클라이언트 처리 스레드 생성
        std::thread clientThread(&MSG::handleClient, this, client_socket);        
        clientThread.join(); // 클라이언트별로 비동기 처리
    }

    close(serverSocketfd); // 서버 소켓 닫기
}

// client threads들이 다른 서버와 연결 후 recv까지 하는 함수
void MSG::broadMessage(const std::string &msg)
{
    std::vector<std::thread> threads;
    for (int i = 1; i < 8; ++i)
    {
        std::string dst_id = "T" + std::to_string(i);

        if (dst_id == "T4")
        {
            continue; // T4는 자기 자신에게 보내지 않도록
        }
        json req_msg = json::parse(msg);
        threads.emplace_back([dst_id, req_msg]()
        {
            clientMessage(dst_id, req_msg);
        });
    }

    for (auto &t : threads)
    {
        if (t.joinable())
            t.join();
    }

    return;
}

// 클라이언트 소켓을 생성하고 타 서버에 연결하는 함수를 구현
std::tuple<int,int, std::string> MSG::DVMMessageOutofStock(int beverageId, int quantity)
{
    std::vector<fs::path> jsonFiles;

    // 1. 브로드 캐스트를 이용해서 json 메시지를 받아온다.
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
    broadMessage(DVMMessageOutofStock_MessageFormat);                       // 재고 부족 메시지 전송

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
                otherDVMstock = std::stoi(j["msg_content"]["item_num"].get<std::string>());
            } else if (j["msg_content"]["item_num"].is_number_integer()) {
                otherDVMstock = j["msg_content"]["item_num"].get<int>();
            }
            

            if(otherDVMstock >= quantity){
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
    return {nearest_x, nearest_y, shortest_id};
}

// 다른 DVM에서 재고 확인 요청을 받았을 때 호출되는 함수 --> 서버가 받은 메시지에서 다시 ACK로 보내는 메시지를 반환하는 함수
json MSG::AskStockMessage(json msg)
{
    // stock 객체 생성 후 현재 재고를 호출
    Stock stock;
    bool canBuy = stock.isPrepayment(msg["msg_content"]["item_code"], msg["msg_content"]["item_num"]); // 재고 확인
    json parsed_resp_stock_msg; // 파싱된 JSON 메시지 저장 변수
    if(canBuy){
        std::string resp_stock_msg = msgFormat(
            "resp_stock", // 재고 요청 확인에 대한 응답 메시지
            msg["src_id"].get<std::string>(), // 요청을 보낸 DVM의 ID를 dst_id로 설정
            std::to_string(msg["msg_content"]["item_code"].get<int>()), // 문의한 아이템 코드
            std::to_string(0), // 재고가 부족하면 0으로 설정
            std::to_string(msg["msg_content"]["coor_x"].get<int>()), // 현재 x 좌표
            std::to_string(msg["msg_content"]["coor_y"].get<int>()), // 현재 y 좌표
            "",
            "F");
        parsed_resp_stock_msg = json::parse(resp_stock_msg);                                                                                                                                                                                     // 파싱된 JSON 메시지 저장 변수
    }
    else { // 만약 재고가 있는 경우
        std::string resp_stock_msg = msgFormat(
            "resp_stock",
            msg["src_id"].get<std::string>(),
            std::to_string(msg["msg_content"]["item_code"].get<int>()),
            std::to_string(msg["msg_content"]["item_num"].get<int>()), // 재고가 더 있더라도 일단 요청한 아이템 수량을 그대로 반환
            std::to_string(msg["msg_content"]["coor_x"].get<int>()),
            std::to_string(msg["msg_content"]["coor_y"].get<int>()),
            "",
            "T");
            parsed_resp_stock_msg = json::parse(resp_stock_msg); // 파싱된 JSON 메시지 저장 변수
    }

    // Json 데이터가 null인지 확인
    // 만약 null이라면 에러 메시지를 출력
    if (parsed_resp_stock_msg.is_null())
    {
        std::cerr << "[ERROR] JSON 데이터가 null입니다." << std::endl;
        return json(); // 빈 JSON 반환
    }

    return parsed_resp_stock_msg;
}

void MSG::SocketOpenInIt(MSG *msg)
{
    auto  serverThread = std::thread(&MSG::serverMessageOpen, msg); // 서버 수신 함수 백그라운드 실행
    serverThread.detach();                                     // 또는 joinable일 때 main에서 join (비차단 운영이면 detach)
}

// sendMessage에서는 선결제 메시지 송신 및 결과 수신
bool MSG::sendMessage(const std::tuple<std::string, int, int, std::string>& msgData) // 정확하게는 지금은 certCode만을 보내는 함수이다.
{
    isPrepayValid = false;
    std::thread thread;

    const auto& [dst_id, itemID, itemNum, newCertCode] = msgData;

    std::string req_prepay_msg = msgFormat(
        "req_prepay",
        dst_id,
        std::to_string(itemID),
        std::to_string(itemNum),
        "",
        "",
        newCertCode,
        "");
    json parsed_req_stock_msg;

    parsed_req_stock_msg = json::parse(req_prepay_msg); // JSON 메시지 파싱


    clientMessage(dst_id, parsed_req_stock_msg);


    if (thread.joinable())
        thread.join();

    return isPrepayValid;
}