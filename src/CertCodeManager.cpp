// CertCodeManager.cpp
#include "CertCodeManager.hpp"
#include <string>
#include "struct/PendingBeverage.hpp"


bool CertCodeManager::isValidateCertCode(std::string cert_code){
    PendingBeverage& pending = PendingBeverage::instance();
    
}

//구현 필요
bool CertCodeManager:: saveCertCode(std::string cert_code, int item_id, int item_num){return true;}