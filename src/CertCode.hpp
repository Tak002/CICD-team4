#pragma once
#include <string>

using namespace std;

class CertCode {
    private:
        string value;
        string generateRandomString(size_t length);
    public:
        string createCertCode();
        string toString();
};
