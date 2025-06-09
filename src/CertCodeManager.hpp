#pragma once
#include <string>

class CertCodeManager {
    private:
        std::string cert_code;
    public:
        bool isValidCertCode(const std::string& code,  int& itemID, int & itemNum) const;
};
