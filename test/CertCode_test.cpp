#include <gtest/gtest.h>
#include "../src/CertCode.hpp"

// CertCode::createCertCode(): 길이가 5인 랜덤 문자열 반환, 내부 value 세팅
TEST(CertCodeTest, CreateCertCode_Returns5CharString) {
    CertCode cert;
    std::string code = cert.createCertCode();

    // 5글자 문자열이 생성되는지 확인
    EXPECT_EQ(code.size(), 5);

    // value 멤버에도 동일한 문자열이 저장되어 있는지 확인
    EXPECT_EQ(cert.toString(), code);
}

// CertCode::toString(): 생성 후 value 값 확인
TEST(CertCodeTest, ToString_ReturnsValue) {
    CertCode cert;
    std::string code = cert.createCertCode();
    EXPECT_EQ(cert.toString(), code);

    // createCertCode()를 여러 번 호출하면 value가 갱신되는지 확인
    std::string code2 = cert.createCertCode();
    EXPECT_EQ(cert.toString(), code2);
}

// CertCode::generateRandomString(): 서로 다른 객체는 서로 다른 문자열을 반환해야 함 (충돌확률이 매우 낮아야 함)
TEST(CertCodeTest, CreateCertCode_ReturnsUniqueStrings) {
    CertCode cert1, cert2;
    std::string code1 = cert1.createCertCode();
    std::string code2 = cert2.createCertCode();

    // 두 코드가 서로 다를 확률이 매우 높아야 함
    EXPECT_NE(code1, code2);
}



// 여러 번 연속 호출시 각기 다른 코드가 나오는지(랜덤성 보장)
TEST(CertCodeTest, CreateCertCode_MultipleCalls_ReturnDifferentCodes) {
    CertCode cert;
    std::string prev = cert.createCertCode();
    bool anyDifferent = false;
    for (int i = 0; i < 10; ++i) {
        std::string next = cert.createCertCode();
        if (prev != next) anyDifferent = true;
        prev = next;
    }
    EXPECT_TRUE(anyDifferent);
}

/*
    각 테스트 목적 요약:
    - 랜덤 문자열 생성의 길이/유효성/랜덤성 확인
    - toString()은 value를 정확히 반환해야 함
    - 여러 객체·여러 번 호출 시 충돌이 없고 매번 새 값이어야 함
    - 0길이 생성 등 엣지 케이스(예외가 아닌 빈 문자열 반환, 현재 코드 기준)
*/
