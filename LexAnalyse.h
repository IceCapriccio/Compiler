//
// Created by IceCapriccio on 2018/11/18.
//

#ifndef COMPILER_LEXCOMPILER_H
#define COMPILER_LEXCOMPILER_H

#include <string>

enum Type {
    //标识符
    Identifier,
    //数字
    Number,
    //关键字
    MainKey, IfKey, WhileKey, ForKey, ScanfKey, PrintfKey, ElseKey, IntKey,
    //界符
    LeftParentheses, RightParentheses, LeftBrace, RightBrace, Comma, Semicolon, Quotation,
    //运算符
    Equal, Unequal, Assignment, LessThan, GreaterThan, NoLessThan, NoGreaterThan, Add, Subtract, Multiply, Divide,
    //条件转移
    JEqual,JUnequal,JLessThan,JGreaterThan,JNoLessThan,JNoGreaterThan,
    //无条件转移
    Goto,
    //定义标签
    DefLabel,
    //空
    None
};

struct Node {
    Type type;
    std::string value;
};


class LexAnalyse {
public:
    void panduan(std::string s);
    struct Node* LexAnalysis();
};


#endif //COMPILER_LEXCOMPILER_H
