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

#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"

#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"

#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/PreprocessorOptions.h"

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
    ci.getPreprocessorOpts().UsePredefines = true;

    clang::HeaderSearchOptions* ptrhso = new clang::HeaderSearchOptions();
    std::shared_ptr<clang::HeaderSearchOptions> hso(ptrhso);
    clang::HeaderSearch headerSearch(hso, ci.getSourceManager(), ci.getDiagnostics(), ci.getLangOpts(),
                                ptrti);

    hso->AddPath("/usr/include", clang::frontend::Angled, false, false);
    hso->AddPath("/usr/lib/gcc/x86_64-linux-gnu/7.5.0/include", clang::frontend::Angled, false, false);

    clang::InitializePreprocessor(ci.getPreprocessor(), ci.getPreprocessorOpts(), 
                                    ci.getPCHContainerReader(), ci.getFrontendOpts());

    const clang::FileEntry* ptrFile = ci.getFileManager().getFile("sample2.c");
    ci.getSourceManager().setMainFileID( ci.getSourceManager().createFileID(ptrFile, clang::SourceLocation(), 
                                            clang::SrcMgr::C_User) );
    ci.getPreprocessor().EnterMainSourceFile();
    ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(), &ci.getPreprocessor());

    clang::Token token;
    do {
        ci.getPreprocessor().Lex(token);
        if(ci.getDiagnostics().hasErrorOccurred()){
            break;
        }
        ci.getPreprocessor().DumpToken(token);
        std::cerr << std::endl;
    } while ( token.isNot(clang::tok::eof) );
    ci.getDiagnosticClient().EndSourceFile();

    return 0;
}