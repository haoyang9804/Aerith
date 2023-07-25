/*! \brief
  Analyze the concept names of the given code. It only accomplishes two missions:

  First, it finds the suitable concept names to be inserted.
  Specifically, if there exists concept names with prefix "techName_Concept",
  then return the biggest following ID (`id`) after this prefix.
  This is important for the name initialization of our inserted concepts.
  e.g., if `id` is 78, then `Add1stConcept` will starts with the first concept with id 79.
  For each concept inserted, the id of our concept will be increased by 1.

  Second, it records the main feature of each existing concept,
  which is how to make it correct.

  This Analysis is only done once at the very beginning of this tech, before all the mutations
  specifically.
*/

#include <algorithm>
#include <optional>
#include <utility>

#include "../include/AnalyzeResult.hpp"
#include "../include/Analyzer.hpp"
#include "../include/Config.hpp"
#include "../include/Utils.hpp"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/FileManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Lex/Lexer.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

std::pair<unsigned, unsigned> _lastdefined_pos = {0, 0};
std::optional<std::string> _lastdefined_conceptName = std::nullopt;

class AnalyzeConceptNameVisitor : public RecursiveASTVisitor<AnalyzeConceptNameVisitor> {
 public:
  explicit AnalyzeConceptNameVisitor(ASTContext* ctx) : ctx(ctx) {}
  bool VisitConceptDecl(ConceptDecl* decl) {
    // First analyze if the concept name has the expected form
    // If it has, record the concept ID in its name
    std::string name = decl->getQualifiedNameAsString();
    // Update conceptNames
    AnalyzeResult::getInstance().conceptNames.push_back(name);
    std::optional<std::string> ID = getConceptID(name, Config::getInstance().techName + "_Concept");
    // Update conceptID
    if (ID.has_value())
      AnalyzeResult::getInstance().conceptID =
          std::max(AnalyzeResult::getInstance().conceptID, std::stoi(ID.value()) + 1);
    // Then record the line number of this concept in the source code
    // Update the biggest line number
    FullSourceLoc FullLocation = ctx->getFullLoc(decl->getBeginLoc());
    unsigned lineNum = FullLocation.getSpellingLineNumber();
    unsigned columnNum = FullLocation.getSpellingColumnNumber();
    std::pair<unsigned, unsigned> pos = {lineNum, columnNum};
    if (_lastdefined_pos < pos) {
      _lastdefined_pos = pos;
      _lastdefined_conceptName = decl->getQualifiedNameAsString();
    }
    return true;
  }

 private:
  ASTContext* ctx;
};

class AnalyzeConceptNameASTConsumer : public ASTConsumer {
 public:
  AnalyzeConceptNameASTConsumer(ASTContext* ctx) : visitor(ctx) {}
  void HandleTranslationUnit(ASTContext& Ctx) override {
    visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
  }

 private:
  AnalyzeConceptNameVisitor visitor;
};

class AnalyzeConceptNameAction : public ASTFrontendAction {
 public:
  explicit AnalyzeConceptNameAction() = default;
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(CompilerInstance& Compiler,
                                                        llvm::StringRef InFile) override {
    return std::unique_ptr<ASTConsumer>(
        std::make_unique<AnalyzeConceptNameASTConsumer>(&Compiler.getASTContext()));
  }
};

std::optional<std::string> AnalyzeConceptName(std::string code) {
  tooling::runToolOnCodeWithArgs(std::make_unique<AnalyzeConceptNameAction>(), code,
                                 {
                                     Config::getInstance().cppStandard,
                                     "-I" + Config::getInstance().includePath,
                                 });
  return _lastdefined_conceptName;
}