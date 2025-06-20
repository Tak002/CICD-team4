// CertCodeManager.cpp
#include "CertCodeManager.hpp"
#include "Controller.hpp"

#include <string>
#include <cstdlib> 
#include <ctime>    
#include <sstream>
#include <fstream>
#include <iostream>

#include "../include/nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;

using namespace std;

//controller 주석 존재
bool CertCodeManager::isValidCertCode(const std::string& code, int& itemID, int & itemNum) const
{
    string flnm = "certCode";
    flnm+=code;
    flnm+=".json";
    std::ifstream ifle(flnm);
    
    json js;

    try{
        ifle >> js;
        
        if(js==nullptr){
            return false;
        }
        
        itemID = js["item_code"];
        itemNum = js["item_num"];


          // 파일 삭제
        if (std::remove(flnm.c_str()) != 0) {
            cerr << "Failed to delete file: " << flnm << endl;
        }

        return true;

    }catch(const json::parse_error& e){
            // cerr << "Error parsing " << "certCode.json" << ": " << e.what() << std::endl;
            return false;
    }
}
