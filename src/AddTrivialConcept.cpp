#include <iostream>
#include <string>

#include "../include/Config.hpp"
#include "../include/Mutators.hpp"
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

bool _find = false;

class AddTrivialConceptVisitor : public RecursiveASTVisitor<AddTrivialConceptVisitor> {
 public:
  explicit AddTrivialConceptVisitor(ASTContext* context, Rewriter& rewriter,
                                    const std::string& conceptName)
      : context(context), rewriter(rewriter), conceptName(conceptName) {}

  bool VisitConceptDecl(ConceptDecl* decl) {
    if (decl->getQualifiedNameAsString() == conceptName) {
      _find = true;  // Emit the message that we found the concept definition.

      /*
        The name of a concept is 'C?' where '?' is a number
        '?' is assigned in order, starting from 1, which is assigned by `Add1stConcept`
      */
      auto conceptNameSplitter = [](const std::string& name) {
        // concept name starts with a common prefix "techName_Concept", which is 15 characters long.
        std::string prefix = Config::getInstance().techName + "_Concept";
        std::string idstr = name.substr(prefix.size());
        std::string newid = "";
        try {
          int id = std::stoi(idstr) + 1;
          newid = std::to_string(id);
        } catch (...) {
          EMIT_ERROR(idstr + " cannot be converted into a number");
        }
        return Config::getInstance().techName + "_Concept" + newid;
      };
      std::string newConceptName = conceptNameSplitter(conceptName);
      SourceLocation endsl = decl->getSourceRange().getEnd();
      /*
        endsl is the beginning location of the last token.
        For instance, in the code
          template<typename>
          concept C2 = C1 && true;
        , endsl points the the position of 't' in the token `true`.
        In another example,
          template<typename>
          concept C2 = true && C1;
        endsl points to the location of the semicolon.
        The location of endsl is not good for insertion because 1) it
        may points to the the location before the last token, and 2) it
        must points to somewhere before the semicolon.
        That's why we need `Lexer::findLocationAfterToken` to find the location
        after the simicolon. `SkipTrailingWhitespaceAndNewLine = true` can help
        start a new line after the location of the semicolon.
      */
      SourceLocation endslAfterSemi = Lexer::findLocationAfterToken(
          endsl, clang::tok::TokenKind::semi, rewriter.getSourceMgr(), rewriter.getLangOpts(),
          /*SkipTrailingWhitespaceAndNewLine=*/true);
      bool rewriteResult = rewriter.InsertTextAfter(
          endslAfterSemi,
          (Twine("template<typename T>\nconcept ") + Twine(newConceptName) + Twine(" = true;\n"))
              .str());
      /*
        rewriteResult = true -> rewriter fails at rewriting.
      */
      ASSERT(!rewriteResult, "Rewrite fails");
    }
    return true;
  }

 private:
  ASTContext* context;
  Rewriter& rewriter;
  const std::string& conceptName;
};

class AddTrivialConceptASTConsumer : public clang::ASTConsumer {
 public:
  AddTrivialConceptASTConsumer(ASTContext* context, Rewriter& rewriter,
                               const std::string& conceptName)
      : visitor(context, rewriter, conceptName) {}
  void HandleTranslationUnit(ASTContext& Ctx) override {
    visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
  }

 private:
  AddTrivialConceptVisitor visitor;
};

class AddTrivialConceptAction : public ASTFrontendAction {
 public:
  explicit AddTrivialConceptAction(const std::string& conceptName, std::string& newcode)
      : conceptName(conceptName), newcode(newcode) {}
  std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(CompilerInstance& Compiler,
                                                        llvm::StringRef InFile) override {
    rewriter.setSourceMgr(Compiler.getSourceManager(), Compiler.getLangOpts());
    return std::unique_ptr<ASTConsumer>(std::make_unique<AddTrivialConceptASTConsumer>(
        &Compiler.getASTContext(), rewriter, conceptName));
  }

  void EndSourceFileAction() override {
    SourceManager& SM = rewriter.getSourceMgr();
    FileID fid = SM.getMainFileID();
    SourceLocation fStartLoc = SM.getLocForStartOfFile(fid);
    SourceLocation fEndLoc = SM.getLocForEndOfFile(fid);
    SourceRange fSR(fStartLoc, fEndLoc);
    newcode = rewriter.getRewrittenText(fSR);
  }

 private:
  Rewriter rewriter;
  const std::string& conceptName;
  std::string& newcode;
};

std::string AddTrivialConcept(std::string code, const std::string& conceptName) {
  _find = false;
  std::string newcode;
  tooling::runToolOnCodeWithArgs(std::make_unique<AddTrivialConceptAction>(conceptName, newcode),
                                 code,
                                 {
                                     Config::getInstance().cppStandard,
                                     "-I" + Config::getInstance().includePath,
                                 });
  ASSERT(_find, "Cannot find the concept definition whose name is " + conceptName);
  return newcode;
}