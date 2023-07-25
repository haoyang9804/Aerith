#include "../include/Utils.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

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

std::optional<std::string> getConceptID(const std::string& conceptName, const std::string& prefix) {
  if (conceptName.starts_with(prefix)) {
    std::string possibleID = conceptName.substr(prefix.size(), conceptName.size() - prefix.size());
    try {
      std::stoi(possibleID);
    } catch (...) {
      return std::nullopt;
    }
    return possibleID;
  }
  return std::nullopt;
}