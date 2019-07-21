//
// Created by rick on 18-8-26.
//

#ifndef CLANGPLAYGROUND_PARSER_H
#define CLANGPLAYGROUND_PARSER_H
#include "helper.h"
#include <llvm/Object/Archive.h>
#include <llvm/Object/ObjectFile.h>
#include <llvm/Object/SymbolicFile.h>

#include <clang/AST/ASTFwd.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/Type.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Sema/Scope.h>

#include "CXXABI.h"
#include "CppParser.h"
#include <string>
#include <vector>

namespace clang {
    namespace CodeGen {
        class CodeGenTypes;
    }

    struct ASTTemplateArgumentListInfo;
    class FunctionTemplateSpecialization;
    class FunctionTemplateSpecializationInfo;
    class PreprocessingRecord;
    class PreprocessedEntity;
    class RawComment;
    class TemplateSpecializationTypeLoc;
    class TemplateArgumentList;
    class VTableLayout;
    class VTableComponent;
}
namespace CppParser {
    struct CppParserOptions{
        CppParserOptions();
        ~CppParserOptions();

        VECTOR_STRING(Arguments)
        std::string libraryFile;

        // C/C++ header file names.
        VECTOR_STRING(SourceFiles)

        // Include directories
        VECTOR_STRING(IncludeDirs)
        VECTOR_STRING(SystemIncludeDirs)
        VECTOR_STRING(Defines)
        VECTOR_STRING(Undefines)
        VECTOR_STRING(LibraryDirs)
        VECTOR_STRING(SupportedStdTypes)


    };
    class Parser {
    public:
        Parser(CppParserOptions* Opts);

        void Setup();
        ParserResult* ParseHeader(const std::vector<std::string>& SourceFiles);
        ParserResult* ParseLibrary(const std::string& File);

    private:
        bool IsSupported(const clang::NamedDecl* ND);
        bool IsSupported(const clang::CXXMethodDecl* MD);

        // AST traversers
        void WalkAST();
        Declaration* WalkDeclaration(const clang::Decl* D);
        Declaration* WalkDeclarationDef(clang::Decl* D);
        Enumeration* WalkEnum(const clang::EnumDecl* ED);
        Enumeration::Item* WalkEnumItem(clang::EnumConstantDecl* ECD);
        Function* WalkFunction(const clang::FunctionDecl* FD, bool IsDependent = false,bool AddToNamespace = true);
        void EnsureCompleteRecord(const clang::RecordDecl* Record,DeclarationContext* NS,Class* RC);
        Class* GetRecord(const clang::RecordDecl* Record,bool& IsComplete);
        Class* WalkRecord(const clang::RecordDecl* Record);
        void WalkRecord(const clang::RecordDecl* Record,Class* RC);
        Class* WalkRecordCXX(const clang::CXXRecordDecl* Record);
        void WalkRecordCXX(const clang::CXXRecordDecl* Record,Class* RC);
        ClassTemplateSpecialization* WalkClassTemplateSpecialization(const clang::ClassTemplateSpecializationDecl* CTS);
        ClassTemplatePartialSpecialization*
        WalkClassTemplatePartialSpecialization(const clang::ClassTemplatePartialSpecializationDecl* CTS);
        Method* WalkMethodCXX(const clang::CXXMethodDecl* MD);
        Field* WalkFieldCXX(const clang::FieldDecl* FD,Class* Class);
        FunctionTemplateSpecialization* WalkFunctionTemplateSpec(clang::FunctionTemplateSpecializationInfo* FTS,Function* Function);
        Variable* WalkVariable(const clang::VarDecl* VD);
        void WalkVariable(const clang::VarDecl* VD,Variable* Var);
        Friend* WalkFriend(const clang::FriendDecl* FD);
        RawComment* WalkRawComment(const clang::RawComment* RC);
        Type* WalkType(clang::QualType QualType,const clang::TypeLoc* TL = nullptr,bool DesugarType = false);
        TemplateArgument WalkTemplateArgument(const clang::TemplateArgument& TA,clang::TemplateArgumentLoc* ArgLoc);
        TemplateTemplateParameter* WalkTemplateTemplateParameter(const clang::TemplateTemplateParmDecl* TTP);
        TypeTemplateParameter* WalkTypeTemplateParameter(const clang::TemplateTypeParmDecl* TTPD);
        NonTypeTemplateParameter* WalkNonTypeTemplateParameter(const clang::NonTypeTemplateParmDecl* TTPD);
        std::vector<Declaration*> WalkTemplateParameterList(const clang::TemplateParameterList* TPL);
        TypeAliasTemplate* WalkTypeAliasTemplate(const clang::TypeAliasTemplateDecl* TD);
        ClassTemplate* WalkClassTemplate(const clang::ClassTemplateDecl* TD);
        FunctionTemplate* WalkFunctionTemplate(const clang::FunctionTemplateDecl* TD);
        VarTemplate* WalkVarTemplate(const clang::VarTemplateDecl* VT);
        VarTemplateSpecialization*
        WalkVarTemplateSpecialization(const clang::VarTemplateSpecializationDecl* VTS);
        VarTemplatePartialSpecialization*
        WalkVarTemplatePartialSpecialization(const clang::VarTemplatePartialSpecializationDecl* VTS);
        template<typename TypeLoc>
        std::vector<TemplateArgument> WalkTemplateArgumentList(const clang::TemplateArgumentList* TAL,TypeLoc* TSTL);
        std::vector<TemplateArgument> WalkTemplateArgumentList(const clang::TemplateArgumentList* TAL,const clang::ASTTemplateArgumentListInfo* TSTL);
        void WalkVTable(const clang::CXXRecordDecl* RD,Class* C);
        QualifiedType GetQualifiedType(const clang::QualType& qual,const clang::TypeLoc* TL = nullptr);
        void ReadClassLayout(Class* Class,const clang::RecordDecl* RD,clang::CharUnits Offset,bool IncludeVirtualBases);
        LayoutField WalkVTableLayout(const clang::VTableComponent& Component);
        VTableLayout WalkVTableLayout(const clang::VTableLayout& VTLayout);
        VTableComponent WalkVTableComponent(const clang::VTableComponent& Component);



    };
}


#endif //CLANGPLAYGROUND_PARSER_H
