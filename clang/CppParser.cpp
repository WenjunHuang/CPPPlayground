//
// Created by xxzyjy on 2018/8/27.
//

#include "CppParser.h"

namespace CppParser {
    CppParserOptions::CppParserOptions()
      : ASTContext(nullptr),
        toolSetToUse(0),
        abi(CppAbi::Itanium),
        noStandardIncludes(false),
        noBuiltinIncludes(false),
        microsoftMode(false),
        verbose(false),
        unityBuild(false) {}

    CppParserOptions::~CppParserOptions() {}

    DEF_VECTOR_STRING(CppParserOptions, Arguments)

    DEF_VECTOR_STRING(CppParserOptions, SourceFiles)

    DEF_VECTOR_STRING(CppParserOptions, IncludeDirs)

    DEF_VECTOR_STRING(CppParserOptions, SystemIncludeDirs)

    DEF_VECTOR_STRING(CppParserOptions, Defines)

    DEF_VECTOR_STRING(CppParserOptions, Undefines)

    DEF_VECTOR_STRING(CppParserOptions, LibraryDirs)

    DEF_VECTOR_STRING(CppParserOptions, SupportedStdTypes)

    ParserResult::ParserResult()
      : library(nullptr),
        targetInfo(nullptr),
        codeParser(nullptr) {}

    ParserResult::ParserResult(const ParserResult &rhs)
      : kind(rhs.kind),
        Diagnostics(rhs.Diagnostics),
        library(rhs.library),
        targetInfo(rhs.targetInfo),
        codeParser(rhs.codeParser) {}

    ParserResult::~ParserResult() {
      if (codeParser)
        delete codeParser;
      if (library)
        delete library;
    }

    ParserDiagnostic::ParserDiagnostic() {}

    ParserDiagnostic::ParserDiagnostic(const ParserDiagnostic &rhs)
      : fileName(rhs.fileName),
        message(rhs.message),
        level(rhs.level),
        lineNumber(rhs.lineNumber),
        columnNumber(rhs.columnNumber) {}

    DEF_VECTOR(ParserResult, ParserDiagnostic, Diagnostics)
}