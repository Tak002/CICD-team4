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



add_executable(certCodeManager_tests
    test/CertCodeManager_test.cpp
    src/CertCodeManager.cpp
)

# add_executable(MSG_tests
#     test/MSG_test.cpp
#     src/MSG.cpp
# )

add_executable(bank_tests
    test/Bank_test.cpp
    src/Bank.cpp
)

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

target_link_libraries(certCodeManager_tests
    gtest
    gtest_main
)
gtest_discover_tests(certCodeManager_tests)

# target_link_libraries(MSG_tests
#     gtest
#     gtest_main
# )
# gtest_discover_tests(certCode_tests)

target_link_libraries(bank_tests
    gtest
    gtest_main
)
gtest_discover_tests(bank_tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")