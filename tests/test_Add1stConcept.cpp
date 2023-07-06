#include "../include/Mutators.hpp"
#include "../include/Utils.hpp"

#include <iostream>

int main() {
  std::string code = read("../pool/1.cpp");
  code = Add1stConcept(code);
  std::cout << code << std::endl;
}