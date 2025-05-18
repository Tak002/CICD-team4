cmake_minimum_required(VERSION 3.30)
project(sonar_scanner_example)

set(CMAKE_CXX_STANDARD 17)

# 메인 실행 파일
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

# ------------------------
#  GTest FetchContent 방식
# ------------------------
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include(GoogleTest)

# 테스트 타겟 1: SampleTest.cpp (기존 테스트)
add_executable(unit_tests
    test/SampleTest.cpp
    src/CertCode.cpp        

)

target_link_libraries(unit_tests
    gtest
    gtest_main
)


gtest_discover_tests(unit_tests)

# 테스트 타겟 2: BankTest.cpp
add_executable(BankTest
    test/BankTest.cpp
    src/Bank.cpp
)

target_include_directories(BankTest PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(BankTest
    gtest
    gtest_main
    pthread
)

gtest_discover_tests(BankTest)

# 테스트 타겟 3: BeverageTest.cpp
add_executable(BeverageTest
    test/BeverageTest.cpp
    src/Beverage.cpp
)

target_include_directories(BeverageTest PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(BeverageTest
    gtest
    gtest_main
    pthread
)

gtest_discover_tests(BeverageTest)


# 테스트 타겟 4: CalcTest.cpp
add_executable(CalcTest
    test/CalcTest.cpp
    src/Calc.cpp
    src/database/InMemoryDB.cpp
    src/database/PositionTable.cpp
    src/struct/Position.hpp
    src/database/BeverageTable.cpp
    src/Beverage.cpp
)

target_include_directories(CalcTest PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(CalcTest
    gtest
    gtest_main
    pthread
)

gtest_discover_tests(CalcTest)
