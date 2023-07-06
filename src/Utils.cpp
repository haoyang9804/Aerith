#include "../include/Utils.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

std::string read(std::string path) {
  ASSERT(std::filesystem::exists(path), "Path " + path + " does not exists");
  std::ifstream infile(path);
  std::stringstream buffer;
  buffer << infile.rdbuf();
  std::string contents = buffer.str();
  return contents;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string splice(const std::vector<std::string>& tokens) {
  std::string str = "";
  for (auto token : tokens) {
    str += token + "\n";
  }
  return str;
}