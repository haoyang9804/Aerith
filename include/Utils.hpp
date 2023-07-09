#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <stdexcept>
#include <string>
#include <vector>

#define ASSERT(STATEMENT, STR) \
  if (!(STATEMENT)) throw std::logic_error(STR)

#define ASSERT_FALSE(STR) throw std::logic_error(STR)

std::string read(std::string path);
std::vector<std::string> split(const std::string& str, char delimiter);
std::string splice(const std::vector<std::string>&);

#endif