//
// Created by IceCapriccio on 2018/11/28.
//

#ifndef COMPILER_SYNTAXANALYSE_H
#define COMPILER_SYNTAXANALYSE_H

#include <string>
#include <stdio.h>
#include <cstdlib>

using namespace std;

enum Type {
    Identifier,
    Number,
    MainKey, IfKey, WhileKey, ForKey, ScanfKey, PrintfKey, ElseKey, IntKey,
    LeftParentheses, RightParentheses, LeftBrace, RightBrace, Comma, Semicolon, Quotation,
    Equal, Unequal, assignment, LessThan, GreaterThan, NoLessThan, NoGreaterThan, Add, Subtract, Multiply, divide
};

struct Node {
    Type type;
    string value;
};

class SyntaxAnalyse {
    void ParseProgram();
    void MatchToken(Type expect);
};


#endif //COMPILER_SYNTAXANALYSE_H