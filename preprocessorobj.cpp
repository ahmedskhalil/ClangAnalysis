/*
 * -*- mode: c++; c-basic-offset: 2; indent-tabs-mode: 4; -*-
 * Author        : Ahmed Khalil
 * Created       : 15.05.20
 * 
 */


#include "llvm/Support/Host.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"

#include "clang/Basic/DiagnosticOptions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"

#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"


int main()
{
    clang::CompilerInstance ci;
    ci.createDiagnostics();

    std::shared_ptr<clang::TargetOptions> ptrto = std::make_shared<clang::TargetOptions>();
    ptrto->Triple = llvm::sys::getDefaultTargetTriple();

    clang::TargetInfo* ptrti = clang::TargetInfo::CreateTargetInfo(ci.getDiagnostics(), ptrto);
    ci.setTarget(ptrti);

    ci.createFileManager();
    ci.createSourceManager(ci.getFileManager());
    ci.createPreprocessor(clang::TU_Complete);

    return 0;
}