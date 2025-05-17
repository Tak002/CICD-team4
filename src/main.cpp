#include <iostream>
#include "Controller.hpp"
#include "Input.hpp"
using namespace std;

int main(int argc, char* argv[]) {

  Controller controller;
  Input input(&controller);
  input.run();
  
  return 0;
}