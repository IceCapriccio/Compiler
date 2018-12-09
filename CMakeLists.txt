cmake_minimum_required(VERSION 3.12)
project(Compiler)

set(CMAKE_CXX_STANDARD 14)

#add_executable(Compiler main.cpp LexAnalyse.cpp LexAnalyse.h SyntaxAnalyse.cpp SyntaxAnalyse.h SemanticAnalyse.cpp SemanticAnalyse.h)
add_executable(Compiler main.cpp LexAnalyse.cpp LexAnalyse.h SyntaxAnalyse.cpp SyntaxAnalyse.h SemanticAnalyse.cpp SemanticAnalyse.h IntermediateCodeGenerate.h IntermediateCodeGenerate.cpp)