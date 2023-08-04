#include <gtest/gtest.h>

#include <string>

#include "../include/Analyzers.hpp"
#include "../include/Utils.hpp"
#include "../DEV/OPs.hpp"

TEST(TestDEV, test_analyzeTemplateSpecialization4Func) {
  std::string code = read("code_pool/2.cpp");
  analyzeTemplateSpecialization4Func(code);
}