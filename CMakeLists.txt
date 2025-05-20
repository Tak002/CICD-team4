cmake_minimum_required(VERSION 3.30)
project(dvm)

set(CMAKE_CXX_STANDARD 17)

# 메인 실행 파일
add_executable(dvm
    src/main.cpp
    src/Auth.cpp
    src/Bank.cpp
    src/Beverage.cpp 
    src/Position.cpp
    src/CertCode.cpp
    src/CertCodeManager.cpp
    src/CheckAuth.cpp
    src/Controller.cpp
    src/Input.cpp
    src/MSG.cpp
    src/ShowScreen.cpp
    src/Stock.cpp
    src/enum/BeverageType.cpp

 
)

# ------------------------
#  GTest FetchContent 방식
# ------------------------
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
FetchContent_Declare(
    json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(json)
enable_testing()
include_directories(${json_SOURCE_DIR}/include)

include(GoogleTest)

# 테스트 타겟 1: SampleTest.cpp (기존 테스트)
add_executable(unit_tests
    test/SampleTest.cpp
    src/CertCode.cpp        

)

add_executable(input_tests
    test/Input_test.cpp
    src/Input.cpp        

)
add_executable(beverage_tests
    test/Beverage_test.cpp
    src/Beverage.cpp        

)

add_executable(stock_tests
    test/Stock_test.cpp
    src/Stock.cpp    
    src/Beverage.cpp    
)

add_executable(certCode_tests
    test/CertCode_test.cpp
    src/CertCode.cpp
)

target_link_libraries(unit_tests
    gtest
    gtest_main
)
gtest_discover_tests(unit_tests)

target_link_libraries(beverage_tests
    gtest
    gtest_main
)
gtest_discover_tests(beverage_tests)

target_link_libraries(input_tests
    gtest
    gtest_main
)
gtest_discover_tests(input_tests)

target_link_libraries(stock_tests
    gtest
    gtest_main
)
gtest_discover_tests(stock_tests)

target_link_libraries(certCode_tests
    gtest
    gtest_main
)
gtest_discover_tests(certCode_tests)