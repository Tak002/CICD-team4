#pragma once
#include <tuple>
#include <list>
#include <string>

class CertCodeTable{
    private:
        std::list<std::tuple<std::string, int, int>> codeAndItem;
    public:
        void insert(const std::string,int code, int num);
        Beverage find(const std::string&id);
        void init();
};
