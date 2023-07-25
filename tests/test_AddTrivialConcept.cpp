#include <iostream>

#include "../include/Mutators.hpp"
#include "../include/Utils.hpp"
#include "../include/Config.hpp"
#include "../include/AnalyzeResult.hpp"
#include "../include/Analyzer.hpp"

int main() {
  std::string code = read("../pool/1.cpp");
  // std::cout << AnalyzeConceptName(code) << std::endl;
  code = Add1stTrivialConcept(code);
  std::cout << code << std::endl;
  std::cout << "==========" << std::endl;
  std::cout << AddTrivialConcept(code, Config::getInstance().techName + "_Concept" + std::to_string(AnalyzeResult::getInstance().conceptID)) << std::endl;
}