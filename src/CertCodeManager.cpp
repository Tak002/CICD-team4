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
bool CertCodeManager::isValidCertCode(std::string cert_code,int& itemID, int & itemNum){
    string flnm = "certCode";
    flnm+=cert_code;
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

    }catch(const std::exception& e){
            cerr << "Error parsing " << "certCode.json" << ": " << e.what() << std::endl;
            return false;
    }
}
