#include "gtest/gtest.h"

#include "../include/Mutators.hpp"
#include "../include/Utils.hpp"


TEST(MutatorTest, AddTrivialTemplate_1) {
  std::string code = read("code_pool/2.cpp");
  AddTrivialTemplate(code);
}