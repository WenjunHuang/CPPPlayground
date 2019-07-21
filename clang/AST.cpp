//
// Created by rick on 18-8-26.
//

#include "AST.h"

#include <string>
#include <vector>
#include <llvm/ADT/SmallString.h>
#include <llvm/Support/Path.h>

static std::string normalizePath(const std::string &File) {
  llvm::SmallString<2 * 128> Result;
  for (auto I = llvm::sys::path::begin(File), E = llvm::sys::path::end(File);
       I != E;
       ++I) {
    if (I->size() == 1 && *I == ".")
      continue;
    if (I->size() == 2 && *I == "..")
      llvm::sys::path::remove_filename(Result);
    else
      llvm::sys::path::append(Result, *I);
  }

#ifdef _WIN32
  std::replace(Result.begin(),Result.end(),'/','\\');
#endif
  return Result.c_str();
}

namespace CppParser {
    namespace AST {
        Type::Type(TypeKind kind) : kind(kind) {}

        Type::Type(const Type &rhs) : kind(rhs.kind), isDependent(rhs.isDependent) {}

        QualifiedType::QualifiedType() : type(nullptr) {}

        TagType::TagType() : Type(TypeKind::Tag) {}

        ArrayType::ArrayType() : Type(TypeKind::Array), size(0), elementSize(0) {}

        FunctionType::FunctionType()
          : Type(TypeKind::Function),
            callingConvention(CallingConvention::Default),
            exceptionSpecType(ExceptionSpecType::None) {}

        Function::~Function() {}

        DEF_VECTOR(FunctionType, Parameter*, Parameters)

        PointerType::PointerType() : Type(TypeKind::Pointer) {}

        MemberPointerType::MemberPointerType() : Type(TypeKind::MemberPointer) {}

        TypedefType::TypedefType() : Type(TypeKind::Typedef), declaration(nullptr) {}

        AttributedType::AttributedType() : Type(TypeKind::Attributed) {}

        DecayedType::DecayedType() : Type(TypeKind::Decayed) {}

        TemplateParameter::TemplateParameter(DeclarationKind kind)
          : Declaration(kind),
            depth(0),
            index(0),
            isParameterPack(false) {}

        TemplateParameter::~TemplateParameter() {}

        TemplateTemplateParameter::TemplateTemplateParameter()
          : Template(DeclarationKind::TemplateTemplateParm),
            isParameterPack(false),
            isPackExpansion(false),
            isExpandedParameterPack(false) {}

        TemplateTemplateParameter::~TemplateTemplateParameter() {}

        TypeTemplateParameter::TypeTemplateParameter() : TemplateParameter(DeclarationKind::TemplateTypeParm) {}

        TypeTemplateParameter::TypeTemplateParameter(const TypeTemplateParameter &rhs)
          : TemplateParameter(rhs.kind),
            defaultArgument(rhs.defaultArgument) {}

        TypeTemplateParameter::~TypeTemplateParameter() {}

        NonTypeTemplateParameter::NonTypeTemplateParameter()
          : TemplateParameter(DeclarationKind::NonTypeTemplateParm),
            defaultArgument(0),
            position(0),
            isPackExpansion(false),
            isExpandedParameterPack(false) {}

        NonTypeTemplateParameter::NonTypeTemplateParameter(const NonTypeTemplateParameter &rhs)
          : TemplateParameter(rhs.kind),
            defaultArgument(rhs.defaultArgument),
            position(rhs.position),
            isPackExpansion(rhs.isPackExpansion),
            isExpandedParameterPack(rhs.isExpandedParameterPack) {}

        NonTypeTemplateParameter::~NonTypeTemplateParameter() {}

        TemplateArgument::TemplateArgument() : declaration(0), integral(0) {}

        TemplateSpecializationType::TemplateSpecializationType()
          : Type(TypeKind::TemplateSpecialization), _template(0) {}

        TemplateSpecializationType::TemplateSpecializationType(const TemplateSpecializationType &rhs) : Type(rhs),
                                                                                                        Arguments(
                                                                                                          rhs.Arguments),
                                                                                                        _template(
                                                                                                          rhs._template),
                                                                                                        desugared(
                                                                                                          rhs.desugared) {}

        TemplateSpecializationType::~TemplateSpecializationType() {}

        DEF_VECTOR(TemplateSpecializationType, TemplateArgument, Arguments)

        DependentTemplateSpecializationType::DependentTemplateSpecializationType() : Type(
          TypeKind::DependentTemplateSpecialization) {}

        DependentTemplateSpecializationType::DependentTemplateSpecializationType(
          const DependentTemplateSpecializationType &rhs) : Type(rhs), Arguments(rhs.Arguments),
                                                            desugared(rhs.desugared) {}

        DependentTemplateSpecializationType::~DependentTemplateSpecializationType() {}

        DEF_VECTOR(DependentTemplateSpecializationType, TemplateArgument, Arguments)

        TemplateParameterType::TemplateParameterType() : Type(TypeKind::TemplateParameter), parameter(nullptr) {}

        TemplateParameterType::~TemplateParameterType() {}

        TemplateParameterSubstitutionType::TemplateParameterSubstitutionType() : Type(
          TypeKind::TemplateParameterSubstitution), replacedParameter(nullptr) {}

        InjectedClassNameType::InjectedClassNameType() : Type(TypeKind::InjectedClassName), _class(nullptr) {}

        DependentNameType::DependentNameType() : Type(TypeKind::DependentName) {}

        DependentNameType::~DependentNameType() {}

        PackExpansionType::PackExpansionType() : Type(TypeKind::PackExpansion) {}

        UnaryTransformType::UnaryTransformType() : Type(TypeKind::UnaryTransform) {}

        VectorType::VectorType() : Type(TypeKind::Vector), numElements(0) {}

        BuiltinType::BuiltinType() : Type(TypeKind::Builtin) {}

        VTableComponent::VTableComponent() : offset(0), declaration(0) {}

        // VTableLayout
        VTableLayout::VTableLayout() {}

        VTableLayout::VTableLayout(const VTableLayout &rhs) : Components(rhs.Components) {}

        VTableLayout::~VTableLayout() {}

        DEF_VECTOR(VTableLayout, VTableComponent, Components)

        VFTableInfo::VFTableInfo() : VBTableIndex(0), VFPtrOffset(0), VFPtrFullOffset(0) {}

        VFTableInfo::VFTableInfo(const VFTableInfo &rhs)
          : VBTableIndex(rhs.VBTableIndex),
            VFPtrOffset(rhs.VFPtrOffset),
            VFPtrFullOffset(rhs.VFPtrFullOffset),
            layout(rhs.layout) {}

        LayoutField::LayoutField() : offset(0), fieldPtr(0) {}

        LayoutField::LayoutField(const LayoutField &other)
          : offset(other.offset),
            name(other.name),
            qualifiedType(other.qualifiedType),
            fieldPtr(other.fieldPtr) {}

        LayoutField::~LayoutField() {}

        LayoutBase::LayoutBase():offset(0),_class(nullptr) {}

        LayoutBase::LayoutBase(const LayoutBase& other):offset(other.offset),_class(other._class){}

        LayoutBase::~LayoutBase() {}

    }
}