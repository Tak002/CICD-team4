#pragma once
#include "PositionTable.hpp"
#include "BeverageTable.hpp"
#include "CertCodeTable.hpp"

class InMemoryDB {
private:
    InMemoryDB() = default;
    InMemoryDB(const InMemoryDB&) = delete;
    InMemoryDB& operator=(const InMemoryDB&) = delete;

public:
    static InMemoryDB& instance();
    PositionTable positionTable;
    BeverageTable beverageTable;
    CertCodeTable certcodeTable;

    void initPosition();
    void initBeverage();
};