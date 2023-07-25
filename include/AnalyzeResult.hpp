#pragma once

#include <vector>
#include <string>

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

  static AnalyzeResult& getInstance() {
    static AnalyzeResult r = AnalyzeResult();
    return r;
  }
};