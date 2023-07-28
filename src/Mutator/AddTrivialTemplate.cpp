#include "../../include/AnalyzeResult.hpp"
#include "../../include/Analyzers.hpp"
#include "../../include/Config.hpp"
#include "../../include/Mutators.hpp"
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

class AddTrivialTemplateVisitor : public RecursiveASTVisitor<AddTrivialTemplateVisitor> {
public:
  explicit AddTrivialTemplateVisitor(ASTContext* ctx) : ctx(ctx) {}
  bool VisitFunctionDecl(FunctionDecl* decl) {
    SourceRange sr = decl->getSourceRange();
    SourceLocation begloc = sr.getBegin();
    SourceLocation endloc = sr.getEnd();
    llvm::errs() << "=====visit function decl=====\n";
    begloc.print(llvm::errs(), ctx->getSourceManager());
    llvm::errs() << "\n";
    endloc.print(llvm::errs(), ctx->getSourceManager());
  }
private:
  ASTContext* ctx;
};

// bool AddTrivialTemplateVisitor::VisitFunctionDecl

class AddTrivialTemplateASTConsumer : public ASTConsumer {
public:
  explicit AddTrivialTemplateASTConsumer(ASTContext* ctx) : visitor(ctx) {}
  void HandleTranslationUnit(ASTContext& ctx) override {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
  }
private:
  AddTrivialTemplateVisitor visitor;
};

class AddTrivialTemplateAction : public ASTFrontendAction {
public:
  explicit AddTrivialTemplateAction() = default;
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(CompilerInstance& Compiler,
                                                        llvm::StringRef InFile) override {
    return std::unique_ptr<ASTConsumer>(
        std::make_unique<AddTrivialTemplateASTConsumer>(&Compiler.getASTContext()));
  }
};

std::string AddTrivialTemplate(std::string code) {
  std::string newcode = code;
  tooling::runToolOnCodeWithArgs(std::make_unique<AddTrivialTemplateAction>(),
                                 code,
                                 {
                                     Config::getInstance().cppStandard,
                                     "-I" + Config::getInstance().includePath,
                                 });
  return newcode;
}