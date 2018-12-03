//
// Created by IceCapriccio on 2018/11/28.
//

#include "SyntaxAnalyse.h"


// 匹配一个终结符
AbstractSyntaxTreeNode* SyntaxAnalyse::MatchToken(Type expect,bool flag) {
    Type lexType = lex[cur].type;
    if (lexType != expect) {
        SyntaxError();
    }
    if(flag)
    {
        if(symbolTable.query(lex[cur].value))
        {
            SyntaxError();
        }
        else
        {
            symbolTable.insert(lex[cur].value,depth);
        }
    }
    else
    {
        if(!symbolTable.query(lex[cur].value))
        {
            SyntaxError();
        }
    }
    if(lexType == RightBrace)
    {
        symbolTable.del(depth);
        depth--;
    }
    if(lexType == LeftBrace)
    {
        depth++;
    }
    auto *node = new AbstractSyntaxTreeNode;
    node->node = lex[cur];
    cur++;
    return node;
}

// 语法分析错误
void SyntaxAnalyse::SyntaxError() {
    printf("syntax error!\n");
    exit(1);
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseProgram() {
    auto root = new AbstractSyntaxTreeNode;
    root->node.value = "Program";
    switch (lex[cur].type) {
        case MainKey:
            root->child[root->num++] = MatchToken(MainKey, false);
            root->child[root->num++] = MatchToken(LeftParentheses, false);
            root->child[root->num++] = MatchToken(RightParentheses, false);
            root->child[root->num++] = MatchToken(LeftBrace, false);
            root->child[root->num++] = ParseDeclarationSequence();
            root->child[root->num++] = ParseStatementSequence();
            root->child[root->num++] = MatchToken(RightBrace, false);
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseDeclarationSequence() {
    auto root = new AbstractSyntaxTreeNode;
    root->node.value = "DeclarationSequence";
    switch (lex[cur].type) {
        case IntKey:
            root->child[root->num++] = ParseDeclarationStatement();
            root->child[root->num++] = ParseDeclarationSequence();
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
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseDeclarationStatement() {
    auto root = new AbstractSyntaxTreeNode;
    root->node.value = "DeclarationStatement";
    switch (lex[cur].type) {
        case IntKey:
            root->child[root->num++] = MatchToken(IntKey, false);
            root->child[root->num++] = ParseIdentifierTable();
            root->child[root->num++] = MatchToken(Semicolon, false);
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseIdentifierTable() {
    auto root = new AbstractSyntaxTreeNode;
    root->node.value = "IdentifierTable";
    switch (lex[cur].type) {
        case Identifier:
            root->child[root->num++] = MatchToken(Identifier, true);
            root->child[root->num++] = ParseIdentifierTableS();
            break;
        default:
            SyntaxError();
            break;
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseIdentifierTableS() {
    auto root = new AbstractSyntaxTreeNode;
    root->node.value = "IdentifierTableS";
    switch (lex[cur].type) {
        case Comma:
            root->child[root->num++] = MatchToken(Comma, false);
            root->child[root->num++] = ParseIdentifierTable();
            break;
        case Semicolon:
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseStatementSequence() {
    auto root = new AbstractSyntaxTreeNode;
    root->node.value = "StatementSequence";
    switch (lex[cur].type) {
        case IfKey:
        case WhileKey:
        case ForKey:
        case ScanfKey:
        case PrintfKey:
        case LeftBrace:
        case Identifier:
            root->child[root->num++] = ParseSentence();
            root->child[root->num++] = ParseStatementSequence();
            break;
        case RightBrace:
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseSentence() {
    auto root = new AbstractSyntaxTreeNode;
    root->node.value = "Sentence";
    switch (lex[cur].type) {
        case IfKey:
            root->child[root->num++] = ParseIfSentence();
            break;
        case WhileKey:
            root->child[root->num++] = ParseWhileSentence();
            break;
        case ForKey:
            root->child[root->num++] = ParseForSentence();
            break;
        case ScanfKey:
            root->child[root->num++] = ParseScanfSentence();
            break;
        case PrintfKey:
            root->child[root->num++] = ParsePrintfSentence();
            break;
        case LeftBrace:
            root->child[root->num++] = ParseCompoundStatement();
            break;
        case Identifier:
        case Number:
        case LeftParentheses:
            root->child[root->num++] = ParseAssignmentStatement();
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseIfSentence() {
    auto root = new AbstractSyntaxTreeNode;
    root->node.value = "IfSentence";
    switch (lex[cur].type) {
        case IfKey:
            root->child[root->num++] = MatchToken(IfKey, false);
            root->child[root->num++] = MatchToken(LeftParentheses, false);
            root->child[root->num++] = ParseExpression();
            root->child[root->num++] = MatchToken(RightParentheses, false);
            root->child[root->num++] = ParseCompoundStatement();
            root->child[root->num++] = ParseElseSentence();
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseElseSentence() {
    switch (lex[cur].type) {
        case ElseKey:
            MatchToken(ElseKey, false);
            ParseCompoundStatement();
            break;
        case ForKey:
        case WhileKey:
        case Identifier:
        case IfKey:
        case PrintfKey:
        case ScanfKey:
        case LeftBrace:
        case RightBrace:
            break;
        default:
            SyntaxError();
    }
}


AbstractSyntaxTreeNode* SyntaxAnalyse::ParseWhileSentence() {
    auto root = new AbstractSyntaxTreeNode;
    root->node.value = "WhileSentence";
    switch (lex[cur].type) {
        case WhileKey:
            root->child[root->num++] = MatchToken(WhileKey, false);
            root->child[root->num++] = MatchToken(LeftParentheses, false);
            root->child[root->num++] = ParseExpression();
            root->child[root->num++] = MatchToken(RightParentheses, false);
            root->child[root->num++] = ParseCompoundStatement();
            break;
        default:
            SyntaxError();
    }
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseForSentence() {
    auto root = new AbstractSyntaxTreeNode;
    switch (lex[cur].type) {
        case ForKey:
            root->child[root->num++] = MatchToken(ForKey, false);
            root->child[root->num++] = MatchToken(LeftParentheses, false);
            root->child[root->num++] = ParseExpression();
            root->child[root->num++] = MatchToken(Semicolon, false);
            root->child[root->num++] = ParseExpression();
            root->child[root->num++] = MatchToken(Semicolon, false);
            root->child[root->num++] = ParseExpression();
            root->child[root->num++] = MatchToken(RightParentheses, false);
            root->child[root->num++] = ParseCompoundStatement();
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseCompoundStatement() {
    auto root = new AbstractSyntaxTreeNode;
    root->node.value = "CompoundStatement";
    switch (lex[cur].type) {
        case LeftBrace:
            root->child[root->num++] = MatchToken(LeftBrace, false);
            root->child[root->num++] = ParseStatementSequence();
            root->child[root->num++] = MatchToken(RightBrace, false);
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseAssignmentStatement() {
    auto root = new AbstractSyntaxTreeNode;
    root->node.value = "AssignmentStatement";
    switch (lex[cur].type) {
        case Identifier:
        case Number:
        case LeftParentheses:
            root->child[root->num++] = ParseExpression();
            root->child[root->num++] = MatchToken(Semicolon, false);
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseExpression() {
    AbstractSyntaxTreeNode *root = new AbstractSyntaxTreeNode;
    root -> node.value = "Expression";
    switch (lex[cur].type) {
        case Identifier:
            root ->child[root ->num++] =  MatchToken(Identifier, false);
            root -> child[root -> num++] =  ParseExpressionS();
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseExpressionS() {
    AbstractSyntaxTreeNode *root = new AbstractSyntaxTreeNode;
    root -> node.value = "ExpressionS";
    switch (lex[cur].type) {
        case Assignment:
            root -> child[root -> num++] = MatchToken(Assignment, false);
            root -> child[root -> num++] = ParseArithmeticExpression();
            break;
        case GreaterThan:
        case LessThan:
        case NoGreaterThan:
        case NoLessThan:
        case Equal:
        case Unequal:
        case Semicolon:
        case RightParentheses:
            root -> child[root -> num++] = ParseRel();
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseRel() {
    AbstractSyntaxTreeNode* root = new AbstractSyntaxTreeNode;
    root -> node.value = "Rel";
    switch (lex[cur].type) {
        case Add:
        case Subtract:
        case Multiply:
        case Divide:
        case LessThan:
        case GreaterThan:
        case NoGreaterThan:
        case NoLessThan:
        case Equal:break;
        case Unequal:
            root -> child[root -> num++] = ParseOpt();
            root -> child[root -> num++] = ParseRelationOperator();
            root -> child[root -> num++] = ParseArithmeticExpression();
            break;
        case Semicolon:
        case RightParentheses:
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseOpt() {
    AbstractSyntaxTreeNode* root = new AbstractSyntaxTreeNode;
    root -> node.value = "Opt";
    switch (lex[cur].type) {
        case Add:
            root ->child[root -> num++] = MatchToken(Add, false);
            root ->child[root -> num++] = ParseArithmeticExpression();
            break;
        case Subtract:
            root -> child[root -> num++] = MatchToken(Subtract, false);
            root -> child[root -> num++] = ParseArithmeticExpression();
            break;
        case Multiply:
            root ->child[root -> num++] = MatchToken(Multiply, false);
            root ->child[root -> num++] = ParseArithmeticExpression();
            break;
        case Divide:
            root -> child[root -> num++] = MatchToken(Divide, false);
            root -> child[root -> num++] = ParseArithmeticExpression();
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
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseRelationOperator()
{
    AbstractSyntaxTreeNode *root = new AbstractSyntaxTreeNode;
    root -> node.value = "RelationOperator";
    switch (lex[cur].type)
    {
        case GreaterThan:
            root -> child[root -> num++] = MatchToken(GreaterThan, false);
            break;
        case LessThan:
            root -> child[root -> num++] = MatchToken(LessThan, false);
            break;
        case NoLessThan:
            root -> child[root -> num++] = MatchToken(NoLessThan, false);
            break;
        case NoGreaterThan:
            root -> child[root -> num++] = MatchToken(NoGreaterThan, false);
            break;
        case Unequal:
            root -> child[root -> num++] = MatchToken(Unequal, false);
            break;
        case Equal:
            root -> child[root -> num++] = MatchToken(Equal, false);
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseArithmeticExpression()
{
    AbstractSyntaxTreeNode *root = new AbstractSyntaxTreeNode;
    root -> node.value = "ArithmeticExpression";
    switch (lex[cur].type)
    {
        case Identifier:
            root -> child[root -> num++] = ParseTerm();
            root -> child[root -> num++] = ParseOparit();
            break;
        case Number:
            root -> child[root -> num++] = ParseTerm();
            root -> child[root -> num++] = ParseOparit();
            break;
        case LeftParentheses:
            root -> child[root -> num++] = ParseTerm();
            root -> child[root -> num++] = ParseOparit();
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseOparit()
{
    AbstractSyntaxTreeNode* root = new AbstractSyntaxTreeNode;
    root -> node.value= "Oparit";
    switch (lex[cur].type)
    {
        case Add:
            root -> child[root -> num++] = MatchToken(Add, false);
            root -> child[root -> num++] = ParseArithmeticExpression();
            break;
        case Subtract:
            root -> child[root -> num++] = MatchToken(Subtract, false);
            root -> child[root -> num++] = ParseArithmeticExpression();
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
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseTerm()
{
    AbstractSyntaxTreeNode* root = new AbstractSyntaxTreeNode;
    root -> node.value = "Term";
    switch (lex[cur].type)
    {
        case Identifier:
            root -> child[root -> num++] = ParseFactor();
            root -> child[root -> num++] = ParseOpterm();
            break;
        case Number:
            root -> child[root -> num++] = ParseFactor();
            root -> child[root -> num++] = ParseOpterm();
            break;
        case LeftParentheses:
            root -> child[root -> num++] = ParseFactor();
            root -> child[root -> num++] = ParseOpterm();
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseOpterm()
{
    AbstractSyntaxTreeNode* root = new AbstractSyntaxTreeNode;
    root -> node.value = "Opterm";
    switch (lex[cur].type)
    {
        case Multiply:
            root -> child[root -> num++] = MatchToken(Multiply, false);
            root -> child[root -> num++] = ParseTerm();
            break;
        case Divide:
            root -> child[root -> num++] = MatchToken(Divide, false);
            root -> child[root -> num++] = ParseTerm();
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
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseFactor()
{
    AbstractSyntaxTreeNode* root = new AbstractSyntaxTreeNode;
    root -> node.value = "Factor";
    switch (lex[cur].type)
    {
        case Identifier:
            root -> child[root -> num++] = MatchToken(Identifier, false);
            break;
        case Number:
            root -> child[root -> num++] = MatchToken(Number, false);
            break;
        case LeftParentheses:
            root -> child[root -> num++] = MatchToken(LeftParentheses, false);
            root -> child[root -> num++] = ParseArithmeticExpression();
            root -> child[root -> num++] = MatchToken(RightParentheses, false);
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseScanfSentence()
{
    auto root = new AbstractSyntaxTreeNode;
    root->node.value = "ScanfSentence";
    switch (lex[cur].type)
    {
        case ScanfKey:
            root -> child[root -> num++] = MatchToken(ScanfKey, false);
            root -> child[root -> num++] = MatchToken(LeftParentheses, false);
            root -> child[root -> num++] = MatchToken(Quotation, false);
            root -> child[root -> num++] = ParseScan();
            root -> child[root -> num++] = MatchToken(RightParentheses, false);
            root -> child[root -> num++] = MatchToken(Semicolon, false);
            break;
        default:SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseScan()
{
    AbstractSyntaxTreeNode* root = new AbstractSyntaxTreeNode;
    root -> node.value = "Scan";
    switch (lex[cur].type)
    {
        case Format:
            root -> child[root -> num++] = MatchToken(Format, false);
            root -> child[root -> num++] = ParseScans();
            root -> child[root -> num++] = MatchToken(Address, false);
            root -> child[root -> num++] = MatchToken(Identifier, false);
            break;
        default:SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseScans()
{
    AbstractSyntaxTreeNode* root = new AbstractSyntaxTreeNode;
    root -> node.value = "Scans";
    switch (lex[cur].type)
    {
        case Format:
            root -> child[root -> num++] = MatchToken(Format, false);
            root -> child[root -> num++] = ParseScans();
            root -> child[root -> num++] = MatchToken(Address, false);
            root -> child[root -> num++] = MatchToken(Identifier, false);
            break;
        case Quotation:
            root -> child[root -> num++] = MatchToken(Quotation, false);
            break;
        default:SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParsePrintfSentence()
{
    AbstractSyntaxTreeNode *root = new AbstractSyntaxTreeNode;
    root -> node.value = "PrintfSentence";
    switch (lex[cur].type)
    {
        case PrintfKey:
            root -> child[root -> num++] = MatchToken(PrintfKey, false);
            root -> child[root -> num++] = MatchToken(LeftParentheses, false);
            root -> child[root -> num++] = MatchToken(Quotation, false);
            root -> child[root -> num++] = ParsePrints();
            root -> child[root -> num++] = MatchToken(RightParentheses, false);
            root -> child[root -> num++] = MatchToken(Semicolon, false);
            break;
        default:SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParsePrints()
{
    AbstractSyntaxTreeNode *root = new AbstractSyntaxTreeNode;
    root -> node.value = "Prints";
    switch (lex[cur].type)
    {
        case String:
            root -> child[root -> num++] = MatchToken(String, false);
            root -> child[root -> num++] = ParsePrints();
            break;
        case Format:
            root -> child[root -> num++] = MatchToken(Format, false);
            root -> child[root -> num++] = ParsePrin();
            root -> child[root -> num++] = MatchToken(Identifier, false);
            break;
        case Quotation:
            root -> child[root -> num++] = MatchToken(Quotation, false);
            break;
        default:SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParsePrin()
{
    AbstractSyntaxTreeNode *root = new AbstractSyntaxTreeNode;
    root -> node.value = "Prin";
    switch (lex[cur].type)
    {
        case Format:
            root -> child[root -> num++] = MatchToken(Format, false);
            root -> child[root -> num++] = ParsePrin();
            root -> child[root -> num++] = MatchToken(Identifier, false);
            break;
        case Quotation:
            root -> child[root -> num++] = MatchToken(Quotation, false);
            break;
        default:SyntaxError();
    }
    return root;
}


void SyntaxAnalyse::Parse(Node *node) {
    this->lex = node;
    cur = 0;
    depth = 0;
    ParseProgram();
}

void SyntaxAnalyse::ShowTree(AbstractSyntaxTreeNode *root) {

}




