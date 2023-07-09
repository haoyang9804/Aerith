#include <iostream>

#include "../include/Mutators.hpp"
#include "../include/Utils.hpp"

int main() {
  std::string code = read("../pool/1.cpp");
  code = Add1stConcept(code);
  std::cout << code << std::endl;
  std::cout << "==========" << std::endl;
  std::cout << AddTrivialConcept(code, "Aerith_Concept1") << std::endl;
}