/*
 * -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: 4; -*-
 * Author        : Ahmed Khalil
 * Created       : 20.05.20
 * 
 */

#include "llvm/Support/Host.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"

#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/Utils.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"

#include "clang/Basic/LangOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"

#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"

#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/PreprocessorOptions.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"

#include "clang/Parse/Parser.h"
#include "clang/Parse/ParseAST.h"

#include <iostream>


int main()
{
    clang::CompilerInstance ci;
    clang::DiagnosticOptions dops;

    ci.createDiagnostics();

    std::shared_ptr<clang::TargetOptions> ptrto = std::make_shared<clang::TargetOptions>();
    ptrto->Triple = llvm::sys::getDefaultTargetTriple();

    clang::TargetInfo* ptrti = clang::TargetInfo::CreateTargetInfo(ci.getDiagnostics(), ptrto);
    ci.setTarget(ptrti);

    ci.createFileManager();
    ci.createSourceManager(ci.getFileManager());
    ci.createPreprocessor(clang::TU_Complete);
    ci.getPreprocessorOpts().UsePredefines = false;

    ci.setASTConsumer(llvm::make_unique<clang::ASTConsumer>());

    ci.createASTContext();
    ci.createSema(clang::TU_Complete, NULL);

    const clang::FileEntry* ptrFile = ci.getFileManager().getFile("sample2.c");
    ci.getSourceManager().setMainFileID( ci.getSourceManager().createFileID(ptrFile, clang::SourceLocation(), 
                                            clang::SrcMgr::C_User) );
                            
    clang::ParseAST(ci.getSema());
    ci.getASTContext().Idents.PrintStats();
    
    return 0;
}