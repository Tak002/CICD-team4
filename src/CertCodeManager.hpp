#pragma once
#include <string>

class CertCodeManager {
    private:
        std::string cert_code;
    public:
        bool isValidateCertCode(std::string code);
};
