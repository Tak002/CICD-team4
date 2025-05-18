#include <gtest/gtest.h>
#include "../src/CertCode.hpp"

// createCertCode가 호출되면 toString()으로 5자리 코드가 생성되어야 함
TEST(CertCodeTest, CreateCertCodeGeneratesFiveCharString) {
    CertCode cert;
    cert.createCertCode();
    std::string code = cert.toString();

    // 길이는 정확히 5자
    EXPECT_EQ(code.length(), 5);

    // 비어 있으면 안 됨
    EXPECT_FALSE(code.empty());

    // 문자들이 유효한 charset 내에 있는지 확인 (선택사항)
    for (char c : code) {
        EXPECT_TRUE(std::isalnum(c));  // 알파벳 또는 숫자여야 함
    }
}

// createCertCode() 여러번 호출 시 값이 바뀌는지 테스트
TEST(CertCodeTest, CreateCertCodeGeneratesNewValueEachTime) {
    CertCode cert;
    cert.createCertCode();
    std::string first = cert.toString();
    cert.createCertCode();
    std::string second = cert.toString();
    
    EXPECT_NE(first, second);  // 랜덤이므로 값이 달라야 정상
}

// 호출 이전에는 빈 값인지 테스트
TEST(CertCodeTest, ToStringReturnsEmptyBeforeGeneration) {
    CertCode cert;
    std::string result = cert.toString();

    EXPECT_TRUE(result.empty());
}

// 생성된 코드가 유효한 charset만 사용하는지 테스트
TEST(CertCodeTest, GeneratedCodeUsesValidCharset) {
    CertCode cert;
    cert.createCertCode();
    std::string code = cert.toString();

    const std::string validChars =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (char c : code) {
        EXPECT_NE(validChars.find(c), std::string::npos);  // validChars 내에 존재해야 함
    }
}

// 여러 인스턴스가 서로 다른 값을 갖는지 테스트
TEST(CertCodeTest, DifferentInstancesProduceDifferentCodes) {
    CertCode cert1;
    CertCode cert2;

    cert1.createCertCode();
    cert2.createCertCode();

    EXPECT_NE(cert1.toString(), cert2.toString());
}
