#include <algorithm>
#include <optional>
#include <utility>

#include "../../include/AnalyzeResult.hpp"
#include "../../include/Analyzers.hpp"
#include "../../include/Config.hpp"
#include "../../include/Utils.hpp"
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

class AnalyzeExistingFunctionsVisitor : public RecursiveASTVisitor<AnalyzeExistingFunctionsVisitor> {
public:
  explicit AnalyzeExistingFunctionsVisitor(ASTContext* ctx) : ctx(ctx) {}

  // bool VisitFunctionDecl(FunctionDecl* decl) {

  //   return true;
  // }

  bool VisitFunctionTemplateDecl(FunctionTemplateDecl* decl) {
    FunctionDecl* fdecl = decl->getTemplatedDecl();
    for (FunctionDecl::param_const_iterator it = fdecl->param_begin(); it != fdecl->param_end(); it++) {
      
    }
    return true;
  }


private:
  ASTContext* ctx;
};

class AnalyzeExistingFunctionsASTConsumer : public ASTConsumer {
public:
  explicit AnalyzeExistingFunctionsASTConsumer(ASTContext* ctx) : visitor(ctx) {}
  void HandleTranslationUnit(ASTContext& ctx) override {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
  }
private:
  AnalyzeExistingFunctionsVisitor visitor;
};

class AnalyzeExistingFunctionsAction : public ASTFrontendAction {
public:
  explicit AnalyzeExistingFunctionsAction() = default;
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(CompilerInstance& Compiler,
                                                        llvm::StringRef InFile) override {
    return std::unique_ptr<ASTConsumer>(
        std::make_unique<AnalyzeExistingFunctionsASTConsumer>(&Compiler.getASTContext()));
  }
private:
};

void AnalyzeFunctions(std::string code) {

}