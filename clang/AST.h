//
// Created by rick on 18-8-26.
//

#ifndef CLANGPLAYGROUND_ASTCONTEXT_H
#define CLANGPLAYGROUND_ASTCONTEXT_H

#include <string>
#include <map>
#include <algorithm>
#include "helper.h"

#define DECLARE_TYPE_KIND(kind) \
  kind##Type();
#define DECLARE_DECL_KIND(klass, kind) \
  klass();

namespace CppParser {
    namespace AST {
#pragma region Types
        enum class TypeKind {
            Tag,
            Array,
            Function,
            Pointer,
            MemberPointer,
            Typedef,
            Attributed,
            Decayed,
            TemplateSpecialization,
            DependentTemplateSpecialization,
            TemplateParameter,
            TemplateParameterSubstitution,
            InjectedClassName,
            DependentName,
            PackExpansion,
            Builtin,
            UnaryTransform,
            Vector
        };

        class Type {
        public:
            Type(TypeKind kind);

            Type(const Type &);

            TypeKind kind;
            bool isDependent;
        };

        struct TypeQualifiers {
            bool isConst;
            bool isVolatile;
            bool isRestrict;
        };

        struct QualifiedType {
            QualifiedType();

            Type *type;
            TypeQualifiers qualifiers;
        };

        class Declaration;

        class TagType : public Type {
        public:
            DECLARE_TYPE_KIND(Tag)

            Declaration *declaration;
        };

        class ArrayType : public Type {
        public:
            enum class ArraySize {
                Constant,
                Variable,
                Dependent,
                Incomplete
            };

            DECLARE_TYPE_KIND(Array)

            QualifiedType qualifiedType;
            ArraySize sizeType;
            long size;
            long elementSize;
        };

        class Parameter;

        enum class CallingConvention {
            Default,
            C,
            StdCall,
            ThisCall,
            FastCall,
            Unknown
        };

        enum class ExceptionSpecType {
            None,
            DynamicNone,
            Dynamic,
            MSAny,
            BasicNoexcept,
            DependentNoexcept,
            NoexceptFalse,
            NoexceptTrue,
            Unevaluated,
            Uninstantiated,
            Unparsed
        };

        class FunctionType : public Type {
        public:
            DECLARE_TYPE_KIND(Function)

            ~FunctionType();

            QualifiedType returnType;
            CallingConvention callingConvention;
            ExceptionSpecType exceptionSpecType;
            VECTOR(Parameter*, Parameters)
        };

        class PointerType : public Type {
        public:
            enum struct TypeModifier {
                Value,
                Pointer,
                LVReference,
                RVReference
            };

            DECLARE_TYPE_KIND(Pointer)

            QualifiedType qualifiedPointee;
            TypeModifier modifier;
        };

        class MemberPointerType : public Type {
        public:
            DECLARE_TYPE_KIND(MemberPointer)

            QualifiedType pointee;
        };

        class TypedefNameDecl;

        class TypedefType : public Type {
        public:
            TypedefType();

            TypedefNameDecl *declaration;
        };

        class AttributedType : public Type {
        public:
            DECLARE_TYPE_KIND(Attributed)

            QualifiedType modified;
            QualifiedType equivalent;
        };

        class DecayedType : public Type {
        public:
            DECLARE_TYPE_KIND(Decayed)

            QualifiedType decayed;
            QualifiedType original;
            QualifiedType pointee;
        };

        struct TemplateArgument {
            TemplateArgument();

            enum struct ArgumentKind {
                Type,
                Declaration,
                NullPtr,
                Integral,
                Template,
                TemplateExpansion,
                Expression,
                Pack
            };

            ArgumentKind kind;
            QualifiedType type;
            Declaration *declaration;
            long integral;
        };

        class Template;

        class TemplateSpecializationType : public Type {
        public:
            TemplateSpecializationType();

            TemplateSpecializationType(const TemplateSpecializationType &);

            ~TemplateSpecializationType();

            VECTOR(TemplateArgument, Arguments)

            Template *_template;
            QualifiedType desugared;
        };

        class DependentTemplateSpecializationType : public Type {
        public:
            DependentTemplateSpecializationType();

            DependentTemplateSpecializationType(const DependentTemplateSpecializationType &);

            ~DependentTemplateSpecializationType();

            VECTOR(TemplateArgument, Arguments)

            QualifiedType desugared;
        };

        class TypeTemplateParameter;

        class TemplateParameterType : public Type {
        public:
            DECLARE_TYPE_KIND(TemplateParameter)

            ~TemplateParameterType();

            TypeTemplateParameter *parameter;
            unsigned int depth;
            unsigned int index;
            bool isParameterPack;
        };

        class TemplateParameterSubstitutionType : public Type {
        public:
            DECLARE_TYPE_KIND(TemplateParameterSubstitution)

            QualifiedType replacement;
            TemplateParameterType* replacedParameter;
        };

        class Class;

        class InjectedClassNameType : public Type {
        public:
            DECLARE_TYPE_KIND(InjectedClassName)

            QualifiedType injectedSpecializationType;
            Class *_class;
        };

        class DependentNameType : public Type {
        public:
            DECLARE_TYPE_KIND(DependentName)

            ~DependentNameType();

            QualifiedType qualifier;
            std::string identifier;
        };

        class PackExpansionType : public Type {
        public:
            DECLARE_TYPE_KIND(PackExpansion)
        };

        class UnaryTransformType : public Type {
        public:
            DECLARE_TYPE_KIND(UnaryTransform)

            QualifiedType desugared;
            QualifiedType baseType;
        };

        class VectorType : public Type {
        public:
            DECLARE_TYPE_KIND(Vector)

            QualifiedType elementType;
            unsigned numElements;
        };

        enum class PrimitiveType {
            Null,
            Void,
            Bool,
            WideChar,
            Char,
            SChar,
            UChar,
            Char16,
            Char32,
            Short,
            UShort,
            Int,
            UInt,
            Long,
            ULong,
            LongLong,
            ULongLong,
            Int128,
            UInt128,
            Half,
            Float,
            Double,
            LongDouble,
            Float128,
            IntPtr
        };

        class BuiltinType : public Type {
        public:
            DECLARE_TYPE_KIND(Builtin)

            PrimitiveType type;
        };

#pragma endregion

#pragma region ABI
        enum class CppAbi {
            Itanium,
            Microsoft,
            ARM,
            iOS,
            iOS64
        };

        enum class VTableComponentKind {
            VCallOffset,
            VBaseOffset,
            OffsetToTop,
            RTTI,
            FunctionPointer,
            CompleteDtorPointer,
            DeletingDtorPointer,
            UnusedFunctionPointer,
        };

        struct VTableComponent {
            VTableComponent();

            VTableComponentKind kind;
            unsigned offset;
            Declaration *declaration;
        };

        struct VTableLayout {
            VTableLayout();

            VTableLayout(const VTableLayout &);

            ~VTableLayout();

            VECTOR(VTableComponent, Components)
        };

        struct VFTableInfo {
            VFTableInfo();

            VFTableInfo(const VFTableInfo &);

            uint64_t VBTableIndex;
            uint32_t VFPtrOffset;
            uint32_t VFPtrFullOffset;
            VTableLayout layout;
        };

        class LayoutField {
        public:
            LayoutField();

            LayoutField(const LayoutField &other);

            ~LayoutField();

            unsigned offset;
            std::string name;
            QualifiedType qualifiedType;
            void *fieldPtr;
        };

        class LayoutBase {
        public:
            LayoutBase();

            LayoutBase(const LayoutBase &other);

            ~LayoutBase();

            unsigned offset;
            Class *_class;
        };

        struct ClassLayout {
            ClassLayout();

            CppAbi ABI;
            VECTOR(VFTableInfo, VFTables)

            VTableLayout layout;
            bool hasOwnVFPtr;
            long VBPtrOffset;
            int alignment;
            int size;
            int dataSize;
            VECTOR(LayoutField, Fields)

            VECTOR(LayoutBase, Bases)
        };

#pragma endregion

#pragma region Declarations
        enum class DeclarationKind {
            DeclarationContext,
            Typedef,
            TypeAlias,
            Parameter,
            Function,
            Method,
            Enumeration,
            EnumerationItem,
            Variable,
            Field,
            AccessSpecifier,
            Class,
            Template,
            TypeAliasTemplate,
            ClassTemplate,
            ClassTemplateSpecialization,
            ClassTemplatePartialSpecialization,
            FunctionTemple,
            Namespace,
            PreprocessedEntity,
            MacroDefinition,
            MacroExpansion,
            TranslationUnit,
            Friend,
            TemplateTemplateParm,
            TemplateTypeParm,
            NonTypeTemplateParm,
            VarTemplate,
            VarTemplateSpecialization,
            VarTemplatePartialSpecialization
        };

        enum class AccessSpecifier {
            Private,
            Protected,
            Public
        };

        class DeclarationContext;

        class RawComment;

        class PreprocessedEntity;


        class Declaration {
        public:
            Declaration(DeclarationKind kind);

            Declaration(const Declaration &);

            ~Declaration();

            DeclarationKind kind;
            int maxFieldAlignment;
            AccessSpecifier access;
            DeclarationContext *_namespace;
            SourceLocation location;
            int lineNumberStart;
            int lineNumberEnd;
            std::string name;
            std::string USR;
            std::string debugText;
            bool isIncomplete;
            bool isDependent;
            bool isImplicit;
            bool isInvalid;
            Declaration *completeDeclaration;
            unsigned definitionOrder;
            VECTOR(PreprocessedEntity *, PreprocessedEntities)

            VECTOR(Declaration*, Redeclarations)

            void *originalPtr;
            RawComment *comment;
        };

        class Class;

        class Enumeration;

        class Function;

        class TypedefDecl;

        class TypeAlias;

        class Namespace;

        class Template;

        class TypeAliasTemplate;

        class ClassTemplate;

        class FunctionTemplate;

        class Variable;

        class Friend;

        class DeclarationContext : public Declaration {
        public:
            DeclarationContext(DeclarationKind kind);

            Declaration *FindAnonymous(const std::string &USR);

            Namespace *FindNamespace(const std::string &Name);

            Namespace *FindNamespace(const std::vector<std::string> &);

            Namespace *FindCreateNamespace(const std::string &Name);

            Class *CreateClass(const std::string &Name, bool IsComplete);

            Class *FindClass(const void *OriginalPtr, const std::string &Name, bool IsComplete);

            Class *FindClass(const void *OriginalPtr, const std::string &Name, bool IsComplete, bool Create);

            template<typename T>
            T *FindTemplate(const std::string &USR);

            Enumeration *FindEnum(const void *OriginalPtr);

            Enumeration *FindEnum(const std::string &Name, bool Create = false);

            Enumeration *FindEnumWithItem(const std::string &Name);

            Function *FindFunction(const std::string &USR);

            TypedefDecl *FindTypedef(const std::string &Name, bool Create = false);

            TypeAlias *FindTypeAlias(const std::string &Name, bool Create = false);

            Variable *FindVariable(const std::string &USR);

            Friend *FindFriend(const std::string &USR);

            VECTOR(Namespace*, Namespaces)

            VECTOR(Enumeration*, Enums)

            VECTOR(Function*, Functions)

            VECTOR(Class*, Classes)

            VECTOR(Template*, Templates)

            VECTOR(TypedefDecl*, Typedefs)

            VECTOR(TypeAlias*, TypeAliases)

            VECTOR(Variable*, Variables)

            VECTOR(Friend*, Friends)

            std::map<std::string, Declaration *> anonymous;

            bool isAnonymous;
        };

        class TypedefNameDecl : public Declaration {
        public:
            TypedefNameDecl(DeclarationKind kind);

            ~TypedefNameDecl();

            QualifiedType qualifiedType;
        };

        class TypedefDecl : public TypedefNameDecl {
        public:
            DECLARE_DECL_KIND(TypedefDecl, Typedef)

            ~TypedefDecl();
        };

        class TypeAlias : public TypedefNameDecl {
        public:
            TypeAlias();

            ~TypeAlias();

            TypeAliasTemplate *describedAliasTemplate;
        };

        class Friend : public Declaration {
        public:
            DECLARE_DECL_KIND(Friend, Friend)

            ~Friend();

            Declaration *declaration;
        };

        enum class StatementClass {
            Any,
            BinaryOperator,
            CallExprClass,
            DeclRefExprClass,
            CXXConstructExprClass,
            CXXOperatorCallExpr,
            ImplicitCastExpr,
            ExplicitCastExpr,
        };

        class Statement {
        public:
            Statement(const std::string &str, StatementClass Class = StatementClass::Any, Declaration *decl = nullptr);

            StatementClass _class;
            Declaration *decl;
            std::string string;
        };

        class Expression : public Statement {
        public:
            Expression(const std::string &str, StatementClass Class = StatementClass::Any, Declaration *decl = nullptr);
        };

        class BinaryOperator : public Expression {
        public:
            BinaryOperator(const std::string &str, Expression *lhs, Expression *rhs, const std::string &opcodeStr);

            ~BinaryOperator();

            Expression *LHS;
            Expression *RHS;
            std::string opcodeStr;
        };

        class CallExpr : public Expression {
        public:
            CallExpr(const std::string &str, Declaration *decl);

            ~CallExpr();

            VECTOR(Expression*, Arguments)
        };

        class CXXConstructExpr : public Expression {
        public:
            CXXConstructExpr(const std::string &str, Declaration *decl = nullptr);

            ~CXXConstructExpr();

            VECTOR(Expression*, Arguments)
        };

        class Parameter : public Declaration {
        public:
            Parameter();

            ~Parameter();

            QualifiedType qualifiedType;
            bool isIndirect;
            bool hasDefaultValue;
            unsigned int index;
            Expression *defaultArgument;
        };


        enum class CXXMethodKind {
            Normal,
            Constructor,
            Destructor,
            Conversion,
            Operator,
            UsingDirective
        };

        enum class CXXOperatorKind {
            None,
            New,
            Delete,
            Array_New,
            Array_Delete,
            Plus,
            Minus,
            Star,
            Slash,
            Percent,
            Caret,
            Amp,
            Pipe,
            Tilde,
            Exclaim,
            Equal,
            Less,
            Greater,
            PlusEqual,
            MinusEqual,
            StarEqual,
            SlashEqual,
            PercentEqual,
            CaretEqual,
            AmpEqual,
            PipeEqual,
            LessLess,
            GreaterGreater,
            LessLessEqual,
            GreaterGreaterEqual,
            EqualEqual,
            ExclaimEqual,
            LessEqual,
            GreaterEqual,
            Spaceship,
            AmpAmp,
            PipePipe,
            PlusPlus,
            MinusMinus,
            Comma,
            ArrowStar,
            Arrow,
            Call,
            Subscript,
            Conditional,
            Coawait
        };

        class FunctionTemplateSpecialization;

        enum class FriendKind {
            None,
            Declared,
            Undeclared
        };

        class Function : public DeclarationContext {
        public:
            Function();

            ~Function();

            QualifiedType returnType;
            bool isReturnIndirect;
            bool hasThisReturn;

            bool isConstExpr;
            bool isVariadic;
            bool isInline;
            bool isPure;
            bool isDeleted;
            bool isDefauted;

            FriendKind friendKind;
            CXXOperatorKind operatorKind;
            std::string mangled;
            std::string signature;
            std::string body;
            CallingConvention callingConvention;
            VECTOR(Parameter*, Parameters)

            FunctionTemplateSpecialization *specializationInfo;
            Function *instantiatedFrom;
            QualifiedType qualifiedType;
        };

        class AccessSpecifierDecl;

        enum class RefQualifierKind {
            None,
            LValue,
            RValue
        };

        class Method : public Function {
        public:
            Method();

            ~Method();

            bool isVirtual;
            bool isStatic;
            bool isConst;
            bool isExplicit;

            CXXMethodKind methodKind;

            bool isDefaultConstructor;
            bool isCopyConstructor;
            bool isMoveConstructor;

            QualifiedType conversionType;
            RefQualifierKind refQualifier;
            VECTOR(Method*, OverridenMethods)
        };

        class Enumeration : public DeclarationContext {
        public:
            DECLARE_DECL_KIND(Enumeration, Enumeration)

            ~Enumeration();

            class Item : public Declaration {
            public:
                DECLARE_DECL_KIND(Item, EnumerationItem)

                Item(const Item &);

                ~Item();

                std::string expression;
                uint64_t value;
            };

            enum class EnumModifiers {
                Anonymous = 1 << 0,
                Scoped = 1 << 1,
                Flags = 1 << 2,
            };

            EnumModifiers modifiers;
            Type *type;
            BuiltinType *builtinType;
            VECTOR(Item*, Items)

            Item *FindItemByName(const std::string &name);
        };

        class Variable : public Declaration {
        public:
            DECLARE_DECL_KIND(Variable, Variable)

            ~Variable();

            std::string mangled;
            QualifiedType qualifiedType;
        };

        class PreprocessedEntity;

        struct BaseClassSpecifier {
            BaseClassSpecifier();

            AccessSpecifier access;
            bool isVirtual;
            Type *type;
            int offset;
        };

        class Field : public Declaration {
        public:
            DECLARE_DECL_KIND(Field, Field)

            ~Field();

            QualifiedType qualifiedType;
            Class *_class;
            bool isBitField;
            unsigned bitWidth;
        };

        class AccessSpecifierDecl : public Declaration {
        public:
            DECLARE_DECL_KIND(AccessSpecifierDecl, AccessSpecifier)

            ~AccessSpecifierDecl();
        };

        class Class : public DeclarationContext {
        public:
            Class();

            ~Class();

            VECTOR(BaseClassSpecifier*, Bases)

            VECTOR(Field*, Fields)

            VECTOR(Method*, Methods)

            VECTOR(AccessSpecifierDecl*, Specifiers)

            bool isPOD;
            bool isAbstract;
            bool isUnion;
            bool isDynamic;
            bool isPolymorphic;
            bool hasNonTrivialDefaultConstructor;
            bool hasNonTrivialCopyConstructor;
            bool hasNonTrivialDestructor;
            bool isExternCContext;
            bool isInjected;

            ClassLayout *layout;
        };

        class Template : public Declaration {
        public:
            Template(DeclarationKind kind);
            DECLARE_DECL_KIND(Template, Template)

            Declaration *TemplatedDecl;
            VECTOR(Declaration*, Parameters)
        };

        template<typename T>
        T *DeclarationContext::FindTemplate(const std::string &USR) {
          auto foundTemplate = std::find_if(Templates.begin(), Templates.end(),
                                            [&](Template *t) { return t->USR == USR; });

          if (foundTemplate != Templates.end())
            return static_cast<T *>(*foundTemplate);

          return nullptr;
        }

        class TypeAliasTemplate : public Template {
        public:
            TypeAliasTemplate();

            ~TypeAliasTemplate();

        };

        class TemplateParameter : public Declaration {
        public:
            TemplateParameter(DeclarationKind kind);

            ~TemplateParameter();

            unsigned int depth;
            unsigned int index;
            bool isParameterPack;
        };

        class TemplateTemplateParameter : public Template {
        public:
            TemplateTemplateParameter();

            ~TemplateTemplateParameter();

            bool isParameterPack;
            bool isPackExpansion;
            bool isExpandedParameterPack;
        };

        class TypeTemplateParameter : public TemplateParameter {
        public:
            TypeTemplateParameter();

            TypeTemplateParameter(const TypeTemplateParameter &);

            ~TypeTemplateParameter();

            QualifiedType defaultArgument;
        };

        class NonTypeTemplateParameter : public TemplateParameter {
        public:
            NonTypeTemplateParameter();

            NonTypeTemplateParameter(const NonTypeTemplateParameter &);

            ~NonTypeTemplateParameter();

            Expression *defaultArgument;
            unsigned int position;
            bool isPackExpansion;
            bool isExpandedParameterPack;
        };

        class ClassTemplateSpecialization;

        class ClassTemplatePartialSpecialization;

        class ClassTemplate : public Template {
        public:
            ClassTemplate();

            ~ClassTemplate();

            VECTOR(ClassTemplateSpecialization*, Specializations)

            ClassTemplateSpecialization *FindSpecialization(const std::string &usr);

            ClassTemplatePartialSpecialization *FindPartialSpecialization(const std::string &usr);
        };

        enum class TemplateSpecializationKind {
            Undeclared,
            ImplicitInstantiation,
            ExplicitSpecialization,
            ExplicitInstantiationDeclaration,
            ExplicitInstantiationDefinition
        };

        class ClassTemplateSpecialization : public Class {
        public:
            ClassTemplateSpecialization();

            ~ClassTemplateSpecialization();

            ClassTemplate *templateDecl;
            VECTOR(TemplateArgument, Arguments)

            TemplateSpecializationKind specializationKind;
        };

        class ClassTemplatePartialSpecialization : public ClassTemplateSpecialization {
        public:
            ClassTemplatePartialSpecialization();

            ~ClassTemplatePartialSpecialization();
        };

        class FunctionTemplate : public Template {
        public:
            FunctionTemplate();

            ~FunctionTemplate();

            VECTOR(FunctionTemplateSpecialization*, Specializations)

            FunctionTemplateSpecialization *FindSpecialization(const std::string &usr);
        };

        class FunctionTemplateSpecialization {
        public:
            FunctionTemplateSpecialization();

            ~FunctionTemplateSpecialization();

            FunctionTemplate *_template;
            VECTOR(TemplateArgument, Arguments)

            Function *specializedFunction;
            TemplateSpecializationKind specializationKind;
        };

        class VarTemplateSpecialization;

        class VarTemplatePartialSpecialization;

        class VarTemplate : public Template {
        public:
            VarTemplate();

            ~VarTemplate();

            VECTOR(VarTemplateSpecialization*, Specializations)

            VarTemplateSpecialization *FindSpecialization(const std::string &usr);

            VarTemplatePartialSpecialization *FindPartialSpecialization(const std::string &usr);
        };

        class VarTemplateSpecialization : public Variable {
        public:
            VarTemplatePartialSpecialization();

            ~VarTemplateSpecialization();

            VarTemplate *templatedDecl;
            VECTOR(TemplateArgument, Arguments)

            TemplateSpecializationKind specializationKind;
        };

        class VarTemplatePartialSpecialization : public VarTemplatePartialSpecialization {
        public:
            VarTemplatePartialSpecialization();

            ~VarTemplatePartialSpecialization();
        };

        class Namespace : public DeclarationContext {
        public:
            Namespace();

            ~Namespace();

            bool isInline;
        };

        enum class MacroLocation {
            Unknown,
            ClassHead,
            ClassBody,
            FunctionHead,
            FunctionParameters,
            FunctionBody,
        };

        class PreprocessedEntity {
        public:
            PreprocessedEntity();

            MacroLocation macroLocation;
            void *originalPtr;
            DeclarationKind kind;
        };

        class MacroDefinition : public PreprocessedEntity {
        public:
            MacroDefinition();

            ~MacroDefinition();

            std::string name;
            std::string expression;
            int lineNumberStart;
            int lineNumberEnd;
        };

        class MacroExpansion : public PreprocessedEntity {
        public:
            MacroExpansion();

            ~MacroExpansion();

            std::string name;
            std::string text;
            MacroDefinition *definition;
        };

        class TranslationUnit : public Namespace {
        public:
            TranslationUnit();

            ~TranslationUnit();

            std::string fileName;
            bool isSystemHeader;
            VECTOR(MacroDefinition*, Macros)
        };

        enum class ArchType {
            UnknownArch,
            x86,
            x86_64
        };

        class NativeLibrary {
        public:
            NativeLibrary();

            ~NativeLibrary();

            std::string fileName;
            ArchType archType;
            VECTOR_STRING(Symbols)

            VECTOR_STRING(Dependencies)
        };


        class ASTContext {
        public:
            ASTContext();

            ~ASTContext();

            TranslationUnit *FindOrCreateModule(std::string File);

            VECTOR(TranslationUnit*, TranslationUnits)
        };

#pragma endregion

#pragma region Comments
        enum struct CommentKind {
            FullComment,
            BlockContentComment,
            BlockCommandComment,
            ParamCommandComment,
            TParamCommandComment,
            VerbatimBlockComment,
            VerbatimLineComment,
            ParagraphComment,
            HTMLTagComment,
            HTMLStartTagComment,
            HTMLEndTagComment,
            TextComment,
            InlineContentComment,
            InlineCommandComment,
            VerbatimBlockLineComment
        };

        class Comment {
        public:
            Comment(CommentKind kind);

            CommentKind kind;
        };

        class BlockContentComment : public Comment {
        public:
            BlockContentComment();

            BlockContentComment(CommentKind kind);
        };

        class FullComment : public Comment {
        public:
            FullComment();

            ~FullComment();

            VECTOR(BlockContentComment*, Blocks)
        };

        class InlineContentComment : public Comment {
        public:
            InlineContentComment();

            InlineContentComment(CommentKind kind);

            bool hasTrailingNewline;
        };

        class ParagraphComment : public BlockContentComment {
        public:
            ParagraphComment();

            ~ParagraphComment();

            bool isWhitespace;
            VECTOR(InlineContentComment*, Content)
        };

        class BlockCommandComment : public BlockContentComment {
        public:
            class Argument {
            public:
                Argument();

                Argument(const Argument &);

                std::string text;
            };

            BlockContentComment();

            BlockContentComment(CommentKind
            kind);

            ~BlockCommandComment();

            unsigned commandId;
            ParagraphComment *paragraphComment;
            VECTOR(Argument, Arguments)

        };

        class ParamCommandComment : public BlockCommandComment {
        public:
            enum PassDirection {
                In,
                Out,
                InOut,

            };

            ParamCommandComment();

            PassDirection direction;
            unsigned paramIndex;
        };

        class TParamCommandComment : public BlockCommandComment {
        public:
            TParamCommandComment();

            VECTOR(unsigned, Position)
        };

        class VerbatimBlockLineComment : public Comment {
        public:
            VerbatimBlockLineComment();

            std::string text;
        };

        class VerbatimBlockComment : public BlockCommandComment {
        public:
            VerbatimBlockComment();

            ~VerbatimBlockComment();

            VECTOR(VerbatimBlockLineComment*, Lines)
        };

        class VerbatimLineComment : public BlockCommandComment {
        public:
            VerbatimLineComment();

            std::string text;
        };

        class InlineCommandComment : public InlineContentComment {
        public:
            enum RenderKind {
                RenderNormal,
                RenderBold,
                RenderMonospaced,
                RenderEmphasized
            };

            class Argument {
            public:
                Argument();

                Argument(const Argument &);

                std::string text;
            };

            InlineCommandComment();

            unsigned commandId;
            RenderKind commentRenderKind;
            VECTOR(Argument, Arguments)
        };

        class HTMLTagComment : public InlineContentComment {
        public:
            HTMLTagComment();

            HTMLTagComment(CommentKind kind);
        };

        class HTMLStartTagComment : public HTMLTagComment {
        public:
            class Attribute {
            public:
                Attribute();

                Attribute(const Attribute &);

                std::string name;
                std::string value;
            };

            HTMLStartTagComment();

            std::string tagName;
            VECTOR(Attribute, Attributes)
        };

        class HTMLEndTagComment : public HTMLTagComment {
        public:
            HTMLEndTagComment();

            std::string tagName;
        };

        class TextComment : public InlineContentComment {
        public:
            TextComment();

            std::string text;
        };

        enum class RawCommentKind {
            Invalid,
            OrdinaryBCPL,
            OrdinaryC,
            BCPLSlash,
            BCPLExcl,
            JavaDoc,
            Qt,
            Merged
        };

        class RawComment {
        public:
            RawComment();

            ~RawComment();

            RawCommentKind kind;
            std::string text;
            std::string briefText;
            FullComment *fullCommentBlock;
        };

#pragma region Commands
#pragma endregion
#pragma endregion


    }
}


#endif //CLANGPLAYGROUND_ASTCONTEXT_H
