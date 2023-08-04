#include <iostream>
#include <string>

#include "OPs.hpp"
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

class AnalyzeTemplateSpecialization4FuncVisitor : public RecursiveASTVisitor<AnalyzeTemplateSpecialization4FuncVisitor> {
public:
  explicit AnalyzeTemplateSpecialization4FuncVisitor(ASTContext* ctx) : ctx(ctx) {}

  bool VisitFunctionTemplateDecl(FunctionTemplateDecl* decl) {
    DEBUG_VISIT(AnalyzeTemplateSpecialization4FuncVisitor, decl);
    auto spec_beg = decl->spec_begin();
    auto spec_end = decl->spec_end();
    for (; spec_beg != spec_end; spec_beg++) {
      DEBUG_VISIT(spec, spec_beg);
    }

    return true;
  }


private:
  ASTContext* ctx;
};

class AnalyzeTemplateSpecialization4FuncASTConsumer : public ASTConsumer {
public:
  explicit AnalyzeTemplateSpecialization4FuncASTConsumer(ASTContext* ctx) : visitor(ctx) {}
  void HandleTranslationUnit(ASTContext& ctx) override {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
  }
private:
  AnalyzeTemplateSpecialization4FuncVisitor visitor;
};

class AnalyzeTemplateSpecialization4FuncAction : public ASTFrontendAction {
public:
  explicit AnalyzeTemplateSpecialization4FuncAction() = default;
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(CompilerInstance& Compiler,
                                                        llvm::StringRef InFile) override {
    return std::unique_ptr<ASTConsumer>(
        std::make_unique<AnalyzeTemplateSpecialization4FuncASTConsumer>(&Compiler.getASTContext()));
  }
private:
};

void analyzeTemplateSpecialization4Func(std::string code) {
  tooling::runToolOnCodeWithArgs(std::make_unique<AnalyzeTemplateSpecialization4FuncAction>(),
                                 code,
                                 {
                                     Config::getInstance().cppStandard,
                                     "-I" + Config::getInstance().includePath,
                                 });
}