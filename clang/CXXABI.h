#pragma once

namespace clang {
    class ASTContext;
    class MemberPointerType;

    class CXXABI {
    public:
        virtual ~CXXABI();

        virtual unsigned getMemberPointerSize(const MemberPointerType* MPT) const = 0;

        virtual CallingConv getDefaultMethodCallConv() const  = 0;

        virtual bool isNearlyEmpty(const CXXRecordDecl *RD) const = 0;
    };

    CXXABI* CreateARMCXXABI(ASTContext &Ctx);
    CXXABI* CreateItaniumCXXABI(ASTContext &Ctx);
    CXXABI* CreateMicrosoftCXXABI(ASTContext &Ctx);
}