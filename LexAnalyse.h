//
// Created by IceCapriccio on 2018/11/18.
//

#ifndef COMPILER_LEXCOMPILER_H
#define COMPILER_LEXCOMPILER_H

//TODO 语法分析体现多态 https://www.cnblogs.com/gaofei-1/p/6435442.html

#include <string>

enum Type {
    Identifier,
    Number,
    MainKey, IfKey, WhileKey, ForKey, ScanfKey, PrintfKey, ElseKey, IntKey,
    LeftParentheses, RightParentheses, LeftBrace, RightBrace, Comma, Semicolon, Quotation,
    Equal, Unequal, Assignment, LessThan, GreaterThan, NoLessThan, NoGreaterThan, Add, Subtract, Multiply, Divide
};

struct Node {
    Type type;
    std::string value;
};


class LexAnalyse {

};


#endif //COMPILER_LEXCOMPILER_H
