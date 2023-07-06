#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/FileManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Rewrite/Core/Rewriter.h"

using namespace clang;

class AddTrivialConceptVisitor : public RecursiveASTVisitor<AddTrivialConceptVisitor> {
public:
  explicit AddTrivialConceptVisitor(ASTContext *context, Rewriter& rewriter) :
    context(context), rewriter(rewriter) {}
  
private:
  ASTContext *context;
  Rewriter rewriter;

};

class AddTrivialConceptASTConsumer : public clang::ASTConsumer {
public:
  AddTrivialConceptASTConsumer(Rewriter& rewriter) :
    rewriter(rewriter) {}
  void HandleTranslationUnit(ASTContext &Ctx) override {
    
  }

private:
  Rewriter rewriter;
};



std::string AddTrivialConcept(std::string code) {
  
}