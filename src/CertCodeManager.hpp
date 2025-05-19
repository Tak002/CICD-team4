#pragma once
#include <string>

class CertCodeManager {
    private:
        std::string cert_code;
    public:
        bool isValidateCertCode(std::string cert_code);
        bool saveCertCode(std::string cert_code,int item_id, int item_num);
};
