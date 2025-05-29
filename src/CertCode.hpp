#pragma once
#include <string>

class CertCode {
    private:
        std::string value;
        std::string generateRandomString(size_t length);
    public:
        std::string createCertCode();
        std::string toString() const;
};
