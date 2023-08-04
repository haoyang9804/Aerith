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
#include "llvm/ADT/DenseSet.h"

using namespace clang;

/**
 * The implementation of AddTrivialTemplateVisitor takes requires both
 * VisitFunctionTemplateDecl and VisitFunctionDecl.
 * 
 * Perform VisitFunctionTemplateDecl, recording the name of the templated function.
 * Since for any templated function, VisitFunctionDecl is always performed before
 * VisitFunctionDecl, the recording will be known when performing VisitingFunctionDecl on 
 * this templated function.
 * 
 * The implementation also considers three special scenarios:
 * 1) forward definition 
 * 2) template specialization
 * 3) function overloading
 */

// TODO: how to handle pack expansion in a template

// static llvm::DenseSet<std::string> 

class AddTrivialTemplateVisitor : public RecursiveASTVisitor<AddTrivialTemplateVisitor> {
public:
  explicit AddTrivialTemplateVisitor(ASTContext* ctx, bool all_at_once) : ctx(ctx), all_at_once(all_at_once) {}

  bool VisitFunctionDecl(FunctionDecl* decl) {
     
    return true;
  }

  // bool VisitFunctionTemplateDecl(FunctionTemplateDecl* decl) {
  //   DEBUG_VISIT(VisitFunctionTemplateDecl);
  //   // llvm::errs() << "canonical decl's address: " << static_cast<const void*>(decl->getCanonicalDecl()) << "\n";

  //   TemplateParameterList* params = decl->getTemplateParameters();
  //   for(auto** it = params->begin(); it != params->end(); it++) {
  //     llvm::errs() << (*it)->getQualifiedNameAsString() << "\n";
  //   } 
  //   llvm::errs() << ">>> param type: \n";
    
  //   // if(isa<FunctionTemplateDecl>(decl)) {
  //   //   llvm::errs() << "inside if\n";
  //   //   const FunctionProtoType *FPT = dyn_cast<FunctionProtoType>(decl->getFunctionType());
  //   //   llvm::errs() << "after dyn_cast\n";
  //   //   for (unsigned i = 0, e = FPT->getNumParams(); i < e; ++i) {
  //   //         llvm::errs() << "Parameter " << i << ": " << FPT->getParamType(i).getAsString() << "\n";
  //   //     }
  //   //   llvm::errs() << ">>> return type: " << decl->getFunctionType()->getReturnType().getAsString() << "\n";
  //   // } 
    
  //   return true;
  // }
private:
  ASTContext* ctx;
  bool all_at_once;
};

// bool AddTrivialTemplateVisitor::VisitClassTemplateSpecializationDecl(ClassTemplateSpecializationDecl* decl) {

// }

// bool AddTrivialTemplateVisitor::VisitVarTemplateDecl(VarTemplateDecl* decl) {
// llvm::errs() << "decl's name is " << decl->getQualifiedNameAsString() << "\n";
//     SourceRange sr = decl->getSourceRange();
//     SourceLocation begloc = sr.getBegin();
//     SourceLocation endloc = sr.getEnd();
//     // llvm::errs() << "=====visit templated function decl=====\n";
//     begloc.print(llvm::errs(), ctx->getSourceManager());
//     llvm::errs() << "\n";
//     endloc.print(llvm::errs(), ctx->getSourceManager());
//     llvm::errs() << "\n";
//     return true;
// }

class AddTrivialTemplateASTConsumer : public ASTConsumer {
public:
  explicit AddTrivialTemplateASTConsumer(ASTContext* ctx, bool all_at_once) : visitor(ctx, all_at_once) {}
  void HandleTranslationUnit(ASTContext& ctx) override {
    visitor.TraverseDecl(ctx.getTranslationUnitDecl());
  }
private:
  AddTrivialTemplateVisitor visitor;
};

class AddTrivialTemplateAction : public ASTFrontendAction {
public:
  explicit AddTrivialTemplateAction(bool all_at_once) : all_at_once(all_at_once) {};
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(CompilerInstance& Compiler,
                                                        llvm::StringRef InFile) override {
    rewriter.setSourceMgr(Compiler.getSourceManager(), Compiler.getLangOpts());
    return std::unique_ptr<ASTConsumer>(
        std::make_unique<AddTrivialTemplateASTConsumer>(&Compiler.getASTContext(), all_at_once));
  }
private:
  bool all_at_once;
  Rewriter rewriter;
};

std::pair<std::string, bool> AddTrivialTemplate(std::string code, bool all_at_once) {
  std::string newcode = code;
  tooling::runToolOnCodeWithArgs(std::make_unique<AddTrivialTemplateAction>(all_at_once),
                                 code,
                                 {
                                     Config::getInstance().cppStandard,
                                     "-I" + Config::getInstance().includePath,
                                 });
  bool alldone = false;
  if (all_at_once) alldone = true;
  return make_pair(newcode, alldone);
}