// Bank.cpp
#include "Bank.hpp"
#include <cstdlib> 
#include <ctime>    
#include <sstream>
#include <fstream>
#include <iostream>

#include "../include/nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;

//확정
bool Bank::checkCardValidity(int card_num, int price){
 
    std::ifstream ifle("card.json");
    
    json js;

    int c_num = -1;
    int c_price = -1;
    try{
        ifle >> js;
        
        if(js["card_num"] == card_num){
            if(js["balance"] >= price){
                return true;
            }
        }
        
    }catch(const std::exception& e){
            cerr << "Error parsing " << "card.json" << ": " << e.what() << std::endl;
    }

    return false;
}

//확정
void Bank::updateBalance(int card_num, int price){
    
    int balance;

    std::ifstream ifle("card.json");
    json js;
    
    ifle>>js;

    balance = js["balance"];

    json j;
    j["card_num"] = card_num;
    j["balance"] = balance - price;
    ofstream o("card.json");
    o << j.dump(2);
}


//확정
bool Bank::reqeustPayment(int card_num, int price){

    if(checkCardValidity(card_num,price)){
        updateBalance(card_num, price);
        return true;
    }

    return false;
}

//확정
void Bank::rollback(int card_num, int price){
    int balance;

    std::ifstream ifle("card.json");
    json js;
    
    ifle>>js;

    balance = js["balance"];

    json j;
    j["card_num"] = card_num;
    j["balance"] = balance + price;
    ofstream o("card.json");
    o << j.dump(2);
}   