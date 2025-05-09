cmake_minimum_required(VERSION 3.9)# 최소 cmake 버전 명시
project(sonar_scanner_example)# 프로젝트 이름 명시

set(CMAKE_CXX_STANDARD 17)# 사용할 C++ 표준(여기선 17)

add_executable(sonar_scanner_example src/main.cpp)# 빌드할 실행파일 및 소스 목록
