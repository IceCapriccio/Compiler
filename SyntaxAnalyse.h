//
// Created by IceCapriccio on 2018/11/28.
//

#ifndef COMPILER_SYNTAXANALYSE_H
#define COMPILER_SYNTAXANALYSE_H

#include <string>
#include <stdio.h>
#include <cstdlib>
#include "LexAnalyse.h"

using namespace std;

class SyntaxAnalyse {
private:
    void MatchToken(Type expect);
    void SyntaxError();
    void ParseProgram();
    void ParseDeclarationSequence();
    void ParseDeclarationStatement();
    void ParseIdentifierTable();
    void ParseIdentifierTableS();
    void ParseStatementSequence();
    void ParseSentence();
    void ParseIfSentence();
    void ParseElseSentence();
    void ParseWhileSentence();
    void ParseForSentence();
    void ParseCompoundStatement();
    void ParseAssignmentStatement();
    void ParseExpression();
    void ParseExpressionS();
    void ParseRel();
    void ParseOpt();
    void ParseRelationOperator();
    void ParseArithmeticExpression();
    void ParseOparit();
    void ParseTerm();
    void ParseOpterm();
    void ParseFactor();
public:
//    void Parse(Node node[]);
};


#endif //COMPILER_SYNTAXANALYSE_H