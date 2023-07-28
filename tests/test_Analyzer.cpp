#include <gtest/gtest.h>

#include <string>

#include "../include/Analyzers.hpp"
#include "../include/Utils.hpp"

TEST(AnalyzerTest, AnalyzeConceptNameTest_1) {
  std::string code = read("code_pool/1.cpp");
  auto lastdefined_concept_name = AnalyzeConceptName(code);
  EXPECT_TRUE(lastdefined_concept_name.has_value());
  EXPECT_EQ(std::string(lastdefined_concept_name.value()), std::string("Arthur_Concept100"));
}