#ifndef MUTATORS_HPP_
#define MUTATORS_HPP_

#include <string>

std::string Add1stTrivialConcept(std::string code);
std::string AddTrivialConcept(std::string code, const std::string& conceptName);
std::string AddTrivialTemplate(std::string code);

#endif