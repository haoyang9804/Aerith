#pragma once
#include <string>
#include <utility>

std::string Add1stTrivialConcept(std::string code);
/**
 * @brief Add a trivial concept definition like this:
 * template<typename T>
 * concept CONCEPTNAME = true;
 * 
 * @param code 
 * @param conceptName 
 * @return std::string 
 */
std::string AddTrivialConcept(std::string code, const std::string& conceptName);
/**
 * @brief Traverse all AST nodes, when finding a FunctionDecl which contains 
 * at least one argument whose type is not parametric. Then rewrite it
 * by adding a parametric type in template and replace the concrete type
 * with the parametric one.
 * 
 * @param code 
 * @param all_at_once whether or not all non-templated/incompletely-templated
 * functions will be rewritten in one AST tree traverse. If this argument
 * is false, then only rewrite one such function in one traverse (this
 * default setting can possibly help with program reduction).
 * @return a pair of the string representation of the code and a boolean
 * indicating whether or not all non-templated/incompletely-templated functions 
 * have been rewritten.
 */
std::pair<std::string, bool> AddTrivialTemplate(std::string code, bool all_at_once=false);
