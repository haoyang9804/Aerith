#pragma once

#include <optional>
#include <string>

/**
 * @brief This function has three responsibilities:
 * 1) Update conceptID in the AnalyzeResult class, this variable represents
 * the ID of the first inserted concept definition;
 * 2) Update conceptNames in the AnalyzeResult class, this variable represents
 * a vector of concept names in the source code;
 * 3) Return the name of the last concept definition. This name is a landmark
 * after where we can insert the first concept.
 *
 * @param code The C++ code that is ready to be analyzed.
 * update the configuration conceptID
 * @return the name of the concept that is last defined (could be nullopt)
 */
std::optional<std::string> AnalyzeConceptName(std::string code);

/**
 * @brief This function analyzes the existing functions in
 * the given code, extracting their names and parameter informations.
 * This function is called initially before [AddTrivialTemplate].
 * 
 * @param code 
 */

void AnalyzeFunctions(std::string code);