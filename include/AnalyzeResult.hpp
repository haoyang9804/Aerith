#pragma once

#include <string>
#include <vector>
#include <utility>

#include "clang/Basic/SourceLocation.h"

class AnalyzeResult {
 private:
  AnalyzeResult() = default;

 public:
  AnalyzeResult(const AnalyzeResult&) = delete;
  AnalyzeResult(AnalyzeResult&&) = delete;
  AnalyzeResult& operator=(const AnalyzeResult&) = delete;
  AnalyzeResult& operator=(AnalyzeResult&&) = delete;

  int conceptID = 0;
  std::vector<std::string> conceptNames;

  struct F {
    int ID;
    std::string f_name;
    std::vector<std::pair<std::string, clang::SourceLocation>> parameter_types_info;
    F() {}
    F(int ID, std::string f_name) : ID(ID), f_name(f_name) {}
  };

  std::vector<F> funcs;

  static AnalyzeResult& getInstance() {
    static AnalyzeResult r = AnalyzeResult();
    return r;
  }
};