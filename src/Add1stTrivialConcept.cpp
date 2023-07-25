#include "../include/Mutators.hpp"
#include "../include/Analyzer.hpp"
#include "../include/Utils.hpp"
#include "../include/Config.hpp"
#include "../include/AnalyzeResult.hpp"


std::string Add1stTrivialConcept(std::string code) {
  auto lastdefined_concept_name = AnalyzeConceptName(code);
  if (lastdefined_concept_name.has_value()) {
    // There are concept definition(s) in the source code
    // we will insert the new concept definitions after the 
    // position of the last defined concept
    return AddTrivialConcept(code, lastdefined_concept_name.value()); 
  }
  else {
    std::vector<std::string> lines = split(code, '\n');
    size_t siz = lines.size();
    size_t offset = 0;
    // Otherwise, insert concept definitions after the last #include
    for (; offset < siz; offset++) {
      if (lines[offset][0] != '#') {
        break;
      }
    }
    lines.insert(std::next(lines.begin(), offset), "template<typename>\nconcept " + Config::getInstance().techName + std::to_string(AnalyzeResult::getInstance().conceptID) + " = true;");
    return splice(lines);
  }
  
}
