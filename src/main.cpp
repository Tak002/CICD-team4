#include <iostream>
#include "Controller.hpp"
#include "Input.hpp"
using namespace std;

int main(int argc, char* argv[]) {

  Controller controller;
  while(true){
    controller.run();
  }
  return 0;
}