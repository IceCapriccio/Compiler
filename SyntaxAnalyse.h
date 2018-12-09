//
// Created by IceCapriccio on 2018/11/28.
//

#ifndef COMPILER_SYNTAXANALYSE_H
#define COMPILER_SYNTAXANALYSE_H

#include <string>
#include <stdio.h>
#include <cstdlib>
#include "LexAnalyse.h"
#include "SemanticAnalyse.h"

using namespace std;

class SyntaxAnalyse {
private:
    Node *lex;
    int cur, depth;

    void SyntaxError();
    AbstractSyntaxTreeNode* MatchToken(Type expect, bool flag);
    AbstractSyntaxTreeNode* ParseProgram();
    AbstractSyntaxTreeNode* ParseDeclarationSequence();
    AbstractSyntaxTreeNode* ParseDeclarationStatement();
    AbstractSyntaxTreeNode* ParseIdentifierTable();
    AbstractSyntaxTreeNode* ParseIdentifierTableS();
    AbstractSyntaxTreeNode* ParseStatementSequence();
    AbstractSyntaxTreeNode* ParseSentence();
    AbstractSyntaxTreeNode* ParseIfSentence();
    AbstractSyntaxTreeNode* ParseElseSentence();
    AbstractSyntaxTreeNode* ParseWhileSentence();
    AbstractSyntaxTreeNode* ParseForSentence();
    AbstractSyntaxTreeNode* ParseCompoundStatement();
    AbstractSyntaxTreeNode* ParseAssignmentStatement();
    AbstractSyntaxTreeNode* ParseExpression();
    AbstractSyntaxTreeNode* ParseExpressionS();
    AbstractSyntaxTreeNode* ParseRel();
    AbstractSyntaxTreeNode* ParseOpt();
    AbstractSyntaxTreeNode* ParseRelationOperator();
    AbstractSyntaxTreeNode* ParseArithmeticExpression();
    AbstractSyntaxTreeNode* ParseOparit();
    AbstractSyntaxTreeNode* ParseTerm();
    AbstractSyntaxTreeNode* ParseOpterm();
    AbstractSyntaxTreeNode* ParseFactor();
    AbstractSyntaxTreeNode* ParseScanfSentence();
    AbstractSyntaxTreeNode* ParsePrintfSentence();

public:
    static SymbolTable symbolTable;

    SyntaxAnalyse();
    AbstractSyntaxTreeNode* Parse(Node node[]);
    void ShowTree(AbstractSyntaxTreeNode *root, int depth);
};


#endif //COMPILER_SYNTAXANALYSE_H