//
// Created by rick on 18-8-18.
//

#ifndef CLANGPLAYGROUND_FINDNAMEDCLASSCONSUMER_H
#define CLANGPLAYGROUND_FINDNAMEDCLASSCONSUMER_H

#include <clang/AST/ASTConsumer.h>
#include <clang/AST/ASTContext.h>
#include "FindNamedClassVisitor.h"

class FindNamedClassConsumer : public clang::ASTConsumer {
public:
    FindNamedClassConsumer(clang::ASTContext *context):Visitor(context) {
    }

    virtual void HandleTranslationUnit(clang::ASTContext &Context) {
      Visitor.TraverseDecl(Context.getTranslationUnitDecl());
    }

private:
    FindNamedClassVisitor Visitor;
};


#endif //CLANGPLAYGROUND_FINDNAMEDCLASSCONSUMER_H
