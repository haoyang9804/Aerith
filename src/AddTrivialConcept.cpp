#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/FileManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Tooling.h"

using namespace clang;

class AddTrivialConceptVisitor : public RecursiveASTVisitor<AddTrivialConceptVisitor> {
public:
  explicit AddTrivialConceptVisitor(ASTContext *context, Rewriter& rewriter, const std::string& conceptName) :
    context(context), rewriter(rewriter), conceptName(conceptName) {}
  
  bool visitConceptDecl(ConceptDecl* decl) {
    llvm::errs() << decl->getQualifiedNameAsString() << "\n";
    return true;  
  }

private:
  ASTContext *context;
  Rewriter& rewriter;
  const std::string& conceptName;
};

class AddTrivialConceptASTConsumer : public clang::ASTConsumer {
public:
  AddTrivialConceptASTConsumer(ASTContext *context, Rewriter& rewriter, const std::string& conceptName) :
    visitor(context, rewriter, conceptName) {}
  void HandleTranslationUnit(ASTContext &Ctx) override {
    visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
  }

private:
  AddTrivialConceptVisitor visitor;
};

class AddTrivialConceptAction : public ASTFrontendAction {
public:
  explicit AddTrivialConceptAction(Rewriter& rewriter, const std::string& conceptName) :
     rewriter(rewriter), conceptName(conceptName) {}
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
                    CompilerInstance &Compiler,
                    llvm::StringRef InFile) override {
    return std::unique_ptr<ASTConsumer>(
      std::make_unique<AddTrivialConceptASTConsumer>(&Compiler.getASTContext(), rewriter, conceptName)
    );
  }
private:
  Rewriter& rewriter;
  const std::string& conceptName;
  
};

std::string AddTrivialConcept(std::string code, const std::string& conceptName) {
  Rewriter rewriter;
  tooling::runToolOnCodeWithArgs(std::make_unique<AddTrivialConceptAction>(rewriter, conceptName), code, 
    {
      "-std=c++20",
      "-I/opt/homebrew/Cellar/llvm/16.0.6/lib/clang/16/include",
      });
  return "";
}