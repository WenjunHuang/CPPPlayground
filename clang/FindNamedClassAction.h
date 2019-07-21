//
// Created by rick on 18-8-18.
//

#ifndef CLANGPLAYGROUND_FINDNAMEDCLASSACTION_H
#define CLANGPLAYGROUND_FINDNAMEDCLASSACTION_H
#include <clang/Frontend/FrontendActions.h>
#include <clang/Frontend/CompilerInstance.h>
#include <llvm/ADT/StringRef.h>
#include <memory>
#include "FindNamedClassConsumer.h"


class FindNamedClassAction: public clang::ASTFrontendAction {
public:
    virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &Compiler,llvm::StringRef InFile){
      return std::make_unique<FindNamedClassConsumer>(&Compiler.getASTContext());
    }
};


#endif //CLANGPLAYGROUND_FINDNAMEDCLASSACTION_H
