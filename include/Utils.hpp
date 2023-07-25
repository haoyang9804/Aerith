#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#define ASSERT(STATEMENT, STR) \
  if (!(STATEMENT)) throw std::logic_error(STR)

#define EMIT_ERROR(STR) throw std::logic_error(STR)

// Read source code given the path to the file
std::string read(std::string codePath);
// Split string by delimiter
std::vector<std::string> split(const std::string& str, char delimiter);
// Splice a vector of strings into a long string
std::string splice(const std::vector<std::string>& strvec);
// get Concept ID if the concept starts with some prefix
std::optional<std::string> getConceptID(const std::string& conceptName, const std::string& prefix);

#endif