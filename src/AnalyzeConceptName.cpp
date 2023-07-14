/*
TODO:
  Analyze the concept names of the given code.
  If there exists concept names with prefix "techName_Concept",
  then return the biggest following ID (`id`) after this prefix.
  This is important for the name initialization of our inserted concepts.
  e.g., if `id` is 78, then `Add1stConcept` will starts with the first concept with id 79.
  For each concept inserted, the id of our concept will be increased by 1.
*/

#include <algorithm>

#include "../include/Analyzer.hpp"
#include "../include/Utils.hpp"
#include "../include/Config.hpp"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/FileManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Lex/Lexer.h"

using namespace clang;

int _maxid = 0;

class AnalyzeConceptNameVisitor : public RecursiveASTVisitor<AnalyzeConceptNameVisitor> {
public:
  explicit AnalyzeConceptNameVisitor() = default;
  bool VisitConceptDecl(ConceptDecl* decl) {
    std::string name = decl->getQualifiedNameAsString();
    std::optional<std::string> ID = getConceptID(name, Config::getInstance().techName + "_Concept");
    if (ID.has_value()) _maxid = std::max(_maxid, std::stoi(ID.value()) + 1);
    return true;
  }
};

class AnalyzeConceptNameASTConsumer : public ASTConsumer {
public:
  AnalyzeConceptNameASTConsumer() : visitor() {}
  void HandleTranslationUnit(ASTContext& Ctx) override {
    visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
  }
private:
  AnalyzeConceptNameVisitor visitor;
};

class AnalyzeConceptNameAction : public ASTFrontendAction {
public:
  explicit AnalyzeConceptNameAction() = default;
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(CompilerInstance& Compiler, llvm::StringRef InFile) override {
    return std::unique_ptr<ASTConsumer>(std::make_unique<AnalyzeConceptNameASTConsumer>());
  }
};

/**
 * @brief 
 * 
 * @param code: The C++ code that is ready to be analyzed. 
 * @return a string representation of a integer, which is the ID number
 * of the first concept definition.
 * For instance, if `AnalyzeConceptName` finds there is a concept Named
 * "techName_Concept" followed by an ID number, such as 5.
 * Then this function return str(5), and the first inserted concept will be
 * assigned with the name "techName_Concept6".
 */
std::string AnalyzeConceptName(std::string code) {
  tooling::runToolOnCodeWithArgs(std::make_unique<AnalyzeConceptNameAction>(),
                                 code,
                                 {
                                     Config::getInstance().cppStandard,
                                     "-I" + Config::getInstance().includePath,
                                 }); 
  return std::to_string(_maxid);
}