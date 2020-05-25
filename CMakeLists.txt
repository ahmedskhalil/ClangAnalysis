cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++" 
    CACHE string "clang++ compiler" FORCE)
    
project(clangpg 
        VERSION 0.1.0 
        DESCRIPTION "clang preprocessing" 
        LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++0x ")


set(LLVM_PATH /usr/bin/llvm-config --src-root)

# set(CMAKE_VERBOSE_MAKEFILE on)

link_directories(${LLVM_PATH}/../lib)
include_directories(${LLVM_PATH}/../include)

add_definitions(
  -D__STDC_LIMIT_MACROS
  -D__STDC_CONSTANT_MACROS
)

set(CLANGLIBS 
clangFrontend
clangSerialization
clangDriver
clangParse
clangSema
clangAnalysis
clangAST
clangBasic
clangEdit
clangLex
clangTooling
clangFrontendTool			
clangDriver
clangCodeGen
clangStaticAnalyzerFrontend
clangStaticAnalyzerCheckers
clangStaticAnalyzerCore			
clangARCMigrate			
clangRewrite
clangRewriteFrontend

)

set(LLVMLIBS
LLVMX86AsmParser # MC, MCParser, Support, X86Desc, X86Info
LLVMX86Desc # MC, Support, X86AsmPrinter, X86Info
LLVMX86AsmPrinter # MC, Support, X86Utils
LLVMX86Info # MC, Support, Target
LLVMX86Utils # Core, Support
LLVMipo
LLVMScalarOpts
LLVMInstCombine
LLVMTransformUtils
#   LLVMipa
LLVMAnalysis
LLVMTarget
LLVMOption # Support
LLVMMCParser # MC, Support
LLVMMC # Object, Support
LLVMObject # BitReader, Core, Support
LLVMBitReader # Core, Support
LLVMCore # Support
LLVMSupport
)

set(AUXLIBS
pthread
z
dl
${CURSES_LIBRARIES}
)


find_package(LLVM REQUIRED CONFIG)
include(FindCurses)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

##
add_executable(preprocessorobj  preprocessorobj.cpp)
target_link_libraries(preprocessorobj ${CLANGLIBS})
target_link_libraries(preprocessorobj ${LLVMLIBS})
target_link_libraries(preprocessorobj ${AUXLIBS})

add_executable(filepreprocessor filepreprocessor.cpp)
target_link_libraries(filepreprocessor ${CLANGLIBS})
target_link_libraries(filepreprocessor ${LLVMLIBS})
target_link_libraries(filepreprocessor ${AUXLIBS})

add_executable(includeheader includeheaders.cpp)
target_link_libraries(includeheader ${CLANGLIBS})
target_link_libraries(includeheader ${LLVMLIBS})
target_link_libraries(includeheader ${AUXLIBS})

add_executable(parser parser.cpp)
target_link_libraries(parser ${CLANGLIBS})
target_link_libraries(parser ${LLVMLIBS})
target_link_libraries(parser ${AUXLIBS})




