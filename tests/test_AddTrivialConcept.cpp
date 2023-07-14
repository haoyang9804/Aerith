#include <iostream>

#include "../include/Mutators.hpp"
#include "../include/Utils.hpp"
#include "../include/Config.hpp"
#include "../include/Analyzer.hpp"

int main() {
  std::string code = read("../pool/1.cpp");
  std::cout << AnalyzeConceptName(code) << std::endl;
  // code = Add1stConcept(code);
  // std::cout << code << std::endl;
  // std::cout << "==========" << std::endl;
  // std::cout << AddTrivialConcept(code, Config::getInstance().techName + "_Concept1") << std::endl;
}