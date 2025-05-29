#pragma once
#include <string>

class CertCodeManager {
    private:
        std::string cert_code;
    public:
        bool isValidCertCode(std::string code, int& itemID, int & itemNum) const;
};
