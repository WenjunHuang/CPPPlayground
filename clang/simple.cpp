
#include <memory>
#include <iostream>

#include <llvm/ADT/IntrusiveRefCntPtr.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Host.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/Basic/MemoryBufferCache.h>
#include <clang/Basic/Diagnostic.h>
#include <clang/Basic/DiagnosticOptions.h>
#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Basic/LangOptions.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Basic/TargetOptions.h>
#include <clang/Lex/PreprocessorOptions.h>
#include <clang/Frontend/PreprocessorOutputOptions.h>
#include <clang/Frontend/PCHContainerOperations.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Frontend/ASTConsumers.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Parse/Parser.h>
#include <clang/Parse/ParseAST.h>


using namespace llvm;
using namespace clang;

static cl::opt<std::string>
    FileName(cl::Positional,cl::desc("Input file"),cl::Required);

int simple(int argc, char **argv){
  cl::ParseCommandLineOptions(argc,argv,"My simple front end\n");
  CompilerInstance CI;
  DiagnosticOptions diagnosticOptions;
  CI.createDiagnostics();

  std::shared_ptr<clang::TargetOptions> PTO(new clang::TargetOptions());
  PTO->Triple = sys::getDefaultTargetTriple();
  TargetInfo *PTI = TargetInfo::CreateTargetInfo(CI.getDiagnostics(),PTO);
  CI.setTarget(PTI);

  CI.createFileManager();
  CI.createSourceManager(CI.getFileManager());
  CI.createPreprocessor(TranslationUnitKind::TU_Complete);
  CI.getPreprocessorOpts().UsePredefines = false;

  auto astConsumer = CreateASTPrinter(nullptr,"");
  CI.setASTConsumer(std::move(astConsumer));

  CI.createASTContext();
  CI.createSema(TranslationUnitKind::TU_Complete,nullptr);

  auto pFile = CI.getFileManager().getFile(FileName);
  if (!pFile) {
    std::cerr << "File not found: "<<FileName<<std::endl;
    return 1;
  }

  auto fileId = CI.getSourceManager().createFileID(pFile,SourceLocation(),SrcMgr::C_User);
  CI.getSourceManager().setMainFileID(fileId);
  CI.getDiagnosticClient().BeginSourceFile(CI.getLangOpts());
  ParseAST(CI.getSema());

  // Print AST statistics
  CI.getASTContext().PrintStats();
  CI.getASTContext().Idents.PrintStats();

  return 0;
}
