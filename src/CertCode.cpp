// CertCode.cpp
#include "CertCode.hpp"
#include <iostream>
#include <string>
#include <random>
#include <ctime>

std::string CertCode::generateRandomString(size_t length) {
    const std::string charset =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    std::string result;
    result.reserve(length);

    std::mt19937 engine(std::random_device{}());
    std::uniform_int_distribution<> dist(0, charset.size() - 1);

    for (size_t i = 0; i < length; ++i) {
        result += charset[dist(engine)];
    }
    this->value = result;
    return result;    
}

void CertCode::createCertCode(){
    string code = generateRandomString(5);
    // Controller:: getInstance.sendPrePaymentMSG(code);
}

string CertCode::toString(){
    return value;
}