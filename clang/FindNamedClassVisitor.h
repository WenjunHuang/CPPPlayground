//
// Created by rick on 18-8-18.
//

#ifndef CLANGPLAYGROUND_FINDNAMEDCLASSVISITOR_H
#define CLANGPLAYGROUND_FINDNAMEDCLASSVISITOR_H
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/ASTContext.h>
#include <clang/Tooling/Tooling.h>
#include <iostream>


class FindNamedClassVisitor : public clang::RecursiveASTVisitor<FindNamedClassVisitor>{
public:
    FindNamedClassVisitor(clang::ASTContext *context) {
      Context = context;
    }
    bool VisitCXXRecordDecl(clang::CXXRecordDecl *Declaration) {
      if (Declaration->getQualifiedNameAsString() == "n::m::C") {
        clang::FullSourceLoc FullLocation = Context->getFullLoc(Declaration->getLocStart());
        if (FullLocation.isValid())
          std::cout << "Found declaration at "
          << FullLocation.getSpellingLineNumber() << ":"
          << FullLocation.getSpellingColumnNumber() << std::endl;
      }
      return true;
    }
private:
    clang::ASTContext *Context;

};


#endif //CLANGPLAYGROUND_FINDNAMEDCLASSVISITOR_H
