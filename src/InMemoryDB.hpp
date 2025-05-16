#pragma once
#include "PositionTable.hpp"

class InMemoryDB {
private:
    InMemoryDB() = default;
    InMemoryDB(const InMemoryDB&) = delete;
    InMemoryDB& operator=(const InMemoryDB&) = delete;

public:
    static InMemoryDB& instance();
    PositionTable positionTable;

    void initPosition();
};