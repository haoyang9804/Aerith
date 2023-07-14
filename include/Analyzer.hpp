#ifndef ANALYZER_HPP_
#define ANALYZER_HPP_

#include <string>

/**
 * @brief 
 * 
 * @param code The C++ code that is ready to be analyzed. 
 * update the configuration conceptID
 * @return the name of the concept that is last defined (could be nullopt)
 */
std::optional<std::string> AnalyzeConceptName(std::string code);

#endif