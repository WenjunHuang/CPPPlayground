cmake_minimum_required(VERSION 3.12)
project(ClangPlayground)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(LLVM REQUIRED)
find_package(LibClang REQUIRED)

message("${LIBCLANG_LIBRARIES}")
list(APPEND CMAKE_CXX_FLAGS ${LIBCLANG_CXXFLAGS})
#list(APPEND CMAKE_LIBRARY_PATH ${LIBCLANG_LIBDIR})
set(CLANG_LIBS  clang
        clangFrontend
        clangDriver
        clangSerialization
        clangParse
        clangSema
        clangAST
        clangEdit
        clangBasic
        clangAnalysis
        clangLex
        clangTooling)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include ${LLVM_INCLUDE_DIRS})
add_executable(simple simple.cpp include/catch.h main.cpp FindNamedClassAction.cpp FindNamedClassAction.h FindNamedClassConsumer.cpp FindNamedClassConsumer.h FindNamedClassVisitor.cpp FindNamedClassVisitor.h Parser.cpp Parser.h AST.cpp AST.h helper.h)
target_link_libraries(simple ${CLANG_LIBS} ${LIBCLANG_LIBRARIES} ${LLVM_LIBS} )

