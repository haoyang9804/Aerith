#include "../include/Mutators.hpp"
#include "../include/Utils.hpp"
#include "../include/Config.hpp"

std::string Add1stConcept(std::string code) {
  std::vector<std::string> lines = split(code, '\n');
  size_t siz = lines.size();
  size_t offset = 0;
  for (; offset < siz; offset++) {
    if (lines[offset][0] != '#') {
      break;
    }
  }
  lines.insert(std::next(lines.begin(), offset), "template<typename>\nconcept " + Config::getInstance().techName + "_Concept1 = true;");
  return splice(lines);
}
