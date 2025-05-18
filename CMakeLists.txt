cmake_minimum_required(VERSION 4.0)# 최소 cmake 버전 명시
project(sonar_scanner_example)# 프로젝트 이름 명시

set(CMAKE_CXX_STANDARD 17)# 사용할 C++ 표준(여기선 17)


add_executable(sonar_scanner_example
    src/main.cpp
    src/Auth.cpp
    src/Bank.cpp
    src/Beverage.cpp
    src/Calc.cpp
    src/CertCode.cpp
    src/CertCodeManager.cpp
    src/CheckAuth.cpp
    src/Controller.cpp
    src/Input.cpp
    src/MSG.cpp
    src/ShowScreen.cpp
    src/Stock.cpp
    src/enum/BeverageType.cpp
    src/database/InMemoryDB.cpp           
    src/database/BeverageTable.cpp         
    src/database/PositionTable.cpp         
)
