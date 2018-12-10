//
// Created by IceCapriccio on 2018/12/3.
//

#ifndef COMPILER_INTERMEDITECODEGENERATE_H
#define COMPILER_INTERMEDITECODEGENERATE_H

#include <iostream>
#include "SyntaxAnalyse.h"
#include <map>
#include "SemanticAnalyse.h"

class CodeTerm {
private:
    Type op;
    int param1, param2, dest;
public:
    Type type;
    CodeTerm(Type op, int param1, int param2, int dest);
    Type getOp();
    int getParam1();
    int getParam2();
    int getDest();
    void show();
};

class CodeSequence { // 综合属性 code
private:
    vector <CodeTerm> code;

    // 因为生成的树不是抽象语法树，所以遍历时不方便产生完整的四元式，用以下三个变量暂存一个不完整的四元式
    int param1, param2,param3;
    Type op, op1;
public:
    CodeSequence();
    void push_back(CodeTerm codeTerm);
    void push_back(CodeSequence codeSequence);
    int GetSize();
    vector <CodeTerm> GetCode();
    void setParam1(int param1);
    void setParam2(int param2);
    void setParam3(int param3);
    void setOp(Type op);
    void setOp1(Type op1);
    int getParam1();
    int getParam2();
    int getParam3();
    Type getOp1();
    Type getOp();
    void show();
};

class SupportTable {
private:
    /*
     * 此表用来存储中间变量的值
     * 每一次 "newtemp" 向 LabelTable 申请一行空间
     * 每一次 "Label :" 向对应的表项写入当前的行数，以供 goto 语句跳转
     */
    map <int, int> table;
public:
    SupportTable();
    int NewLabel();
    void LabelAssign(int addr, int value);
    int Query(int addr);
    void show();
};

class Visitor { // 遍历树生成四元式的方法类
private:
    CodeSequence VisitProgram(AbstractSyntaxTreeNode* root);
    CodeSequence VisitStatementSequence(AbstractSyntaxTreeNode *root);
    CodeSequence VisitCompoundStatement(AbstractSyntaxTreeNode *root);
    CodeSequence VisitSentence(AbstractSyntaxTreeNode *root);
    CodeSequence VisitIfSentence(AbstractSyntaxTreeNode* root, int IfSentence_next);
    CodeSequence VisitElseSentence(AbstractSyntaxTreeNode *root);
    CodeSequence VisitWhileSentence(AbstractSyntaxTreeNode *root, int WhileSentence_next);
    CodeSequence VisitForSentence(AbstractSyntaxTreeNode *root, int ForSentence_next);
    CodeSequence VisitAssignmentStatement(AbstractSyntaxTreeNode* node,int Expression_true, int Expression_false);
    CodeSequence VisitExpression(AbstractSyntaxTreeNode* node,int Expression_true, int Expression_false);
    CodeSequence VisitExpressionS(AbstractSyntaxTreeNode* node);
    CodeSequence VisitRel(AbstractSyntaxTreeNode* node);
    CodeSequence VisitOpt(AbstractSyntaxTreeNode* node);
    CodeSequence VisitRelationOperator(AbstractSyntaxTreeNode* node);
    CodeSequence VisitArithmeticExpression(AbstractSyntaxTreeNode* node);
    CodeSequence VisitOparit(AbstractSyntaxTreeNode* node);
    CodeSequence VisitTerm(AbstractSyntaxTreeNode* node);
    CodeSequence VisitOpterm(AbstractSyntaxTreeNode* node);
    CodeSequence VisitFactor(AbstractSyntaxTreeNode* node);
    CodeSequence VisitScanfSentence(AbstractSyntaxTreeNode *node);
    CodeSequence VisitPrintSentence(AbstractSyntaxTreeNode *node);

public:
    CodeSequence Visit(AbstractSyntaxTreeNode *root);
};

#endif //COMPILER_INTERMEDITECODEGENERATE_H
