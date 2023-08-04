#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>
#include <type_traits>

#define ASSERT(STATEMENT, STR) \
  if (!(STATEMENT)) throw std::logic_error(STR)

#define EMIT_ERROR(STR) throw std::logic_error(STR)

// INFO is a string to stress where are the outputs from
#define DEBUG_VISIT(INFO, DECL) \
  llvm::errs() << "======" << #INFO << "=====\n"; \
  llvm::errs() << "DECL's name is " << DECL->getQualifiedNameAsString() << "\n"; \
  SourceRange sr = DECL->getSourceRange(); \
  SourceLocation begloc = sr.getBegin(); \
  SourceLocation endloc = sr.getEnd(); \
  begloc.print(llvm::errs(), ctx->getSourceManager()); \
  llvm::errs() << "\n"; \
  endloc.print(llvm::errs(), ctx->getSourceManager()); \
  llvm::errs() << "\n"; \
  llvm::errs() << "begloc's encoding: " << begloc.getRawEncoding() << "\n"; \
  llvm::errs() << "endloc's encoding: " << endloc.getRawEncoding() << "\n"; 

// Read source code given the path to the file
std::string read(std::string codePath);
// Split string by delimiter
std::vector<std::string> split(const std::string& str, char delimiter);
// Splice a vector of strings into a long string
std::string splice(const std::vector<std::string>& strvec);
// get Concept ID if the concept starts with some prefix
std::optional<std::string> getConceptID(const std::string& conceptName, const std::string& prefix);
