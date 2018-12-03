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
    SymbolTable symbolTable;

    void SyntaxError();
    void ShowTree(AbstractSyntaxTreeNode *root);
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
    AbstractSyntaxTreeNode* ParseScan();
    AbstractSyntaxTreeNode* ParseScans();
    AbstractSyntaxTreeNode* ParsePrintfSentence();
    AbstractSyntaxTreeNode* ParsePrints();
    AbstractSyntaxTreeNode* ParsePrin();

public:
    void Parse(Node node[]);
};


#endif //COMPILER_SYNTAXANALYSE_H