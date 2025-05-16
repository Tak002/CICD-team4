#include "InMemoryDB.hpp"

InMemoryDB& InMemoryDB::instance() {
    static InMemoryDB db;
    return db;
}

void InMemoryDB::initPosition(){
    positionTable.init();
}