#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <cstdio> // remove
#include "../src/CertCodeManager.hpp"

using namespace std;

// 테스트용 JSON 파일 생성 함수
void createCertCodeFile(const std::string& cert_code, int item_code, int item_num) {
    std::ostringstream oss;
    oss << "certCode" << cert_code << ".json";
    std::ofstream ofile(oss.str());
    if (ofile.is_open()) {
        ofile << "{ \"item_code\": " << item_code << ", \"item_num\": " << item_num << " }";
        ofile.close();
    }
}

// 테스트가 끝난 후 파일을 삭제해주는 함수 (테스트 클린업)
void removeCertCodeFile(const std::string& cert_code) {
    std::ostringstream oss;
    oss << "certCode" << cert_code << ".json";
    std::remove(oss.str().c_str());
}

// 정상적인 인증코드 케이스
TEST(CertCodeManagerTest, IsValidCertCode_ValidFile_ReturnsTrueAndSetsValues) {
    CertCodeManager manager;
    std::string cert_code = "12345";
    int expectedItemID = 7;
    int expectedItemNum = 3;

    // 1. 테스트용 JSON 파일 생성
    createCertCodeFile(cert_code, expectedItemID, expectedItemNum);

    // 2. 값 검증
    int itemID = 0;
    int itemNum = 0;
    bool result = manager.isValidCertCode(cert_code, itemID, itemNum);

    // 3. 기대값 확인
    EXPECT_TRUE(result); // 파일이 존재하고, 값이 정상적으로 세팅되면 true여야 함
    EXPECT_EQ(itemID, expectedItemID);   // itemID 값이 일치하는지
    EXPECT_EQ(itemNum, expectedItemNum); // itemNum 값이 일치하는지

    // 4. 파일이 삭제되었는지도 체크
    std::ostringstream oss;
    oss << "certCode" << cert_code << ".json";
    std::ifstream file(oss.str());
    EXPECT_FALSE(file.is_open()); // 파일이 삭제되었으므로 열 수 없어야 함
}

// 인증코드 파일이 없을 때 (존재하지 않는 인증코드)
TEST(CertCodeManagerTest, IsValidCertCode_FileNotFound_ReturnsFalse) {
    CertCodeManager manager;
    std::string cert_code = "doesnotexist";
    int itemID = -1, itemNum = -1;
    // 파일 없음: 실패 반환, 값 변화 없음
    bool result = manager.isValidCertCode(cert_code, itemID, itemNum);
    EXPECT_FALSE(result);
}

// 인증코드 파일이 비정상/파싱 에러일 때
TEST(CertCodeManagerTest, IsValidCertCode_InvalidJson_ReturnsFalse) {
    CertCodeManager manager;
    std::string cert_code = "badjson";
    // 비정상 JSON 파일 생성
    std::ostringstream oss;
    oss << "certCode" << cert_code << ".json";
    std::ofstream ofile(oss.str());
    if (ofile.is_open()) {
        ofile << "NOT_JSON";
        ofile.close();
    }
    int itemID = -1, itemNum = -1;
    bool result = manager.isValidCertCode(cert_code, itemID, itemNum);

    EXPECT_FALSE(result);

    // 파일 정리
    removeCertCodeFile(cert_code);
}

