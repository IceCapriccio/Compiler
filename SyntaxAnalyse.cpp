//
// Created by IceCapriccio on 2018/11/28.
//

#include "SyntaxAnalyse.h"

Node lex[100]; // 语法分析传来的结构体数组
int cur = 0; // 当前分析到哪个单词

// 匹配一个终结符
void SyntaxAnalyse::MatchToken(Type expect) {
    Type lexType = lex[cur].type;
    if (lexType != expect) {
        SyntaxError();
    } else {
        cur++;
    }
}

// 语法分析错误
void SyntaxAnalyse::SyntaxError() {
    printf("syntax error!\n");
    exit(1);
}

void SyntaxAnalyse::ParseProgram() {
    switch (lex[cur].type) {
        case MainKey:
            MatchToken(MainKey);
            MatchToken(LeftParentheses);
            MatchToken(RightParentheses);
            MatchToken(LeftBrace);
            ParseDeclarationSequence();
            ParseStatementSequence();
            MatchToken(RightBrace);
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseDeclarationSequence() {
    switch (lex[cur].type) {
        case IntKey:
            ParseDeclarationStatement();
            ParseDeclarationSequence();
            break;
        case RightBrace:
        case IfKey:
        case WhileKey:
        case ForKey:
        case ScanfKey:
        case PrintfKey:
        case LeftBrace:
        case Identifier:
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseDeclarationStatement() {
    switch (lex[cur].type) {
        case IntKey:
            MatchToken(IntKey);
            ParseIdentifierTable();
            MatchToken(Semicolon);
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseIdentifierTable() {
    switch (lex[cur].type) {
        case Identifier:
            MatchToken(Identifier);
            ParseIdentifierTableS();
            break;
        default:
            SyntaxError();
            break;
    }
}

void SyntaxAnalyse::ParseIdentifierTableS() {
    switch (lex[cur].type) {
        case Comma:
            MatchToken(Comma);
            ParseIdentifierTable();
            break;
        case Semicolon:
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseStatementSequence() {
    switch (lex[cur].type) {
        case IfKey:
        case WhileKey:
        case ForKey:
        case ScanfKey:
        case PrintfKey:
        case LeftBrace:
        case Identifier:
            ParseSentence();
            ParseStatementSequence();
            break;
        case RightBrace:
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseSentence() {
    switch (lex[cur].type) {
        case IfKey:
            ParseIfSentence();
            break;
        case WhileKey:
            ParseWhileSentence();
            break;
        case ForKey:
            ParseForSentence();
            break;
        case ScanfKey:
//            ParseScanfSentence();
            break;
        case PrintfKey:
//            ParsePrintfSentence();
            break;
        case LeftBrace:
            ParseCompoundStatement();
            break;
        case Identifier:
        case Number:
        case LeftParentheses:
            ParseAssignmentStatement();
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseIfSentence() {
    switch (lex[cur].type) {
        case IfKey:
            MatchToken(IfKey);
            MatchToken(LeftParentheses);
            ParseExpression();
            MatchToken(RightParentheses);
            ParseCompoundStatement();
            ParseElseSentence();
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseElseSentence() {
    switch (lex[cur].type) {
        case ElseKey:
            MatchToken(ElseKey);
            ParseCompoundStatement();
            break;
        case Semicolon:
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseWhileSentence() {
    switch (lex[cur].type) {
        case WhileKey:
            MatchToken(WhileKey);
            MatchToken(LeftParentheses);
            ParseExpression();
            MatchToken(RightParentheses);
            ParseCompoundStatement();
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseForSentence() {
    switch (lex[cur].type) {
        case ForKey:
            MatchToken(ForKey);
            MatchToken(LeftParentheses);
            ParseExpression();
            MatchToken(Semicolon);
            ParseExpression();
            MatchToken(Semicolon);
            ParseExpression();
            MatchToken(RightParentheses);
            ParseCompoundStatement();
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseCompoundStatement() {
    switch (lex[cur].type) {
        case LeftBrace:
            MatchToken(LeftBrace);
            ParseStatementSequence();
            MatchToken(RightBrace);
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseAssignmentStatement() {
    switch (lex[cur].type) {
        case Identifier:
        case Number:
        case LeftParentheses:
            ParseExpression();
            MatchToken(Semicolon);
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseExpression() {
    switch (lex[cur].type) {
        case Identifier:
            MatchToken(Identifier);
            ParseExpressionS();
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseExpressionS() {
    switch (lex[cur].type) {
        case Assignment:
            MatchToken(Assignment);
            ParseArithmeticExpression();
            break;
        case GreaterThan:
        case LessThan:
        case NoGreaterThan:
        case NoLessThan:
        case Equal:
        case Unequal:
        case Semicolon:
        case RightParentheses:
            ParseRel();
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseRel() {
    switch (lex[cur].type) {
        case Add:
        case Subtract:
        case Multiply:
        case Divide:
        case LessThan:
        case GreaterThan:
        case NoGreaterThan:
        case NoLessThan:
        case Equal:
        case Unequal:
            ParseOpt();
            ParseRelationOperator();
            ParseArithmeticExpression();
            break;
        case Semicolon:
        case RightParentheses:
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseOpt() {
    switch (lex[cur].type) {
        case Add:
            MatchToken(Add);
            ParseArithmeticExpression();
            break;
        case Subtract:
            MatchToken(Subtract);
            ParseArithmeticExpression();
            break;
        case Multiply:
            MatchToken(Multiply);
            ParseArithmeticExpression();
            break;
        case Divide:
            MatchToken(Divide);
            ParseArithmeticExpression();
            break;
        case GreaterThan:
        case LessThan:
        case NoGreaterThan:
        case NoLessThan:
        case Equal:
        case Unequal:
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseRelationOperator()
{
    switch (lex[cur].type)
    {
        case GreaterThan:
            MatchToken(GreaterThan);
            break;
        case LessThan:
            MatchToken(LessThan);
            break;
        case NoLessThan:
            MatchToken(NoLessThan);
            break;
        case NoGreaterThan:
            MatchToken(NoGreaterThan);
            break;
        case Unequal:
            MatchToken(Unequal);
            break;
        case Equal:
            MatchToken(Equal);
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseArithmeticExpression()
{
    switch (lex[cur].type)
    {
        case Identifier:
            ParseTerm();
            ParseOparit();
            break;
        case Number:
            ParseTerm();
            ParseOparit();
            break;
        case LeftParentheses:
            ParseTerm();
            ParseOparit();
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseOparit()
{
    switch (lex[cur].type)
    {
        case Add:
            MatchToken(Add);
            ParseArithmeticExpression();
            break;
        case Subtract:
            MatchToken(Subtract);
            ParseArithmeticExpression();
            break;
        case LessThan:break;
        case GreaterThan:break;
        case NoGreaterThan:break;
        case NoLessThan:break;
        case Unequal:break;
        case Equal:break;
        case RightParentheses:break;
        case Semicolon:break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseTerm()
{
    switch (lex[cur].type)
    {
        case Identifier:
            ParseFactor();
            ParseOpterm();
            break;
        case Number:
            ParseFactor();
            ParseOpterm();
            break;
        case LeftParentheses:
            ParseFactor();
            ParseOpterm();
            break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseOpterm()
{
    switch (lex[cur].type)
    {
        case Multiply:
            MatchToken(Multiply);
            ParseTerm();
            break;
        case Divide:
            MatchToken(Divide);
            ParseTerm();
            break;
        case Add:break;
        case Subtract:break;
        case LessThan:break;
        case GreaterThan:break;
        case NoLessThan:break;
        case NoGreaterThan:break;
        case Equal:break;
        case Unequal:break;
        case Semicolon:break;
        case RightParentheses:break;
        default:
            SyntaxError();
    }
}

void SyntaxAnalyse::ParseFactor()
{
    switch (lex[cur].type)
    {
        case Identifier:
            MatchToken(Identifier);
            break;
        case Number:
            MatchToken(Number);
            break;
        case LeftParentheses:
            MatchToken(LeftParentheses);
            ParseArithmeticExpression();
            MatchToken(RightParentheses);
            break;
        default:
            SyntaxError();
    }
}




