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


class varASTConsumer : public clang::ASTConsumer {
    public: 
    varASTConsumer() : clang::ASTConsumer() {}
    virtual ~varASTConsumer() {}

    virtual bool HandleTopLevelDecl(clang::DeclGroupRef d) {
        static int count = 0;
        clang::DeclGroupRef::iterator it;

        for (it = d.begin(); it != d.end(); it++) {
            count++;
            clang::VarDecl *vardec = llvm::dyn_cast<clang::VarDecl>(*it);
            if (!vardec) {
                continue;
            }
            if (vardec->isFileVarDecl() && !vardec->hasExternalStorage()) {
                std::cerr << "top level variable declaration:"
                          << vardec->getDeclName().getAsString()
                          << std::endl;
            }
        }
        return true;
    }
};


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
    

    const clang::FileEntry* ptrFile = ci.getFileManager().getFile("sample3.c");
    ci.getSourceManager().setMainFileID( ci.getSourceManager().createFileID(ptrFile, clang::SourceLocation(), 
                                            clang::SrcMgr::C_User));
    ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(), &ci.getPreprocessor());

                            
    clang::ParseAST(ci.getPreprocessor(), &ci.getASTConsumer(), ci.getASTContext());
    ci.getDiagnosticClient().EndSourceFile();


    return 0;
}