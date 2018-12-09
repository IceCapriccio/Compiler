//
// Created by IceCapriccio on 2018/11/28.
//

#include "SyntaxAnalyse.h"

extern string name[];
// 匹配一个终结符
AbstractSyntaxTreeNode* SyntaxAnalyse::MatchToken(Type expect,bool flag) {
    // flag = true：声明一个标识符
    // flag = false：调用一个标识符
    Type lexType = lex[cur].type;
    if (lexType != expect) {
        SyntaxError();
    }
    if (lexType == Identifier) {
        if (flag) {
            if (SyntaxAnalyse::symbolTable.query(lex[cur].value) != -1) {
                SyntaxError();
            } else {
                SyntaxAnalyse::symbolTable.insert(lex[cur].value, depth);
            }
        } else {
            if (SyntaxAnalyse::symbolTable.query(lex[cur].value) == -1) {
                SyntaxError();
            }
        }
    }
    if(lexType == RightBrace) {
//        SyntaxAnalyse::symbolTable.del(depth);
        depth--;
    }
    if(lexType == LeftBrace) {
        depth++;
    }
    auto *node = new AbstractSyntaxTreeNode;
    node->info = lex[cur];
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
    root->info.value = "Program";
    switch (lex[cur].type) {
        case MainKey:
            root->child[root->childNum++] = MatchToken(MainKey, false);
            root->child[root->childNum++] = MatchToken(LeftParentheses, false);
            root->child[root->childNum++] = MatchToken(RightParentheses, false);
            root->child[root->childNum++] = MatchToken(LeftBrace, false);
            root->child[root->childNum++] = ParseDeclarationSequence();
            root->child[root->childNum++] = ParseStatementSequence();
            root->child[root->childNum++] = MatchToken(RightBrace, false);
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseDeclarationSequence() {
    auto root = new AbstractSyntaxTreeNode;
    root->info.value = "DeclarationSequence";
    switch (lex[cur].type) {
        case IntKey:
            root->child[root->childNum++] = ParseDeclarationStatement();
            root->child[root->childNum++] = ParseDeclarationSequence();
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
    root->info.value = "DeclarationStatement";
    switch (lex[cur].type) {
        case IntKey:
            root->child[root->childNum++] = MatchToken(IntKey, false);
            root->child[root->childNum++] = ParseIdentifierTable();
            root->child[root->childNum++] = MatchToken(Semicolon, false);
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseIdentifierTable() {
    auto root = new AbstractSyntaxTreeNode;
    root->info.value = "IdentifierTable";
    switch (lex[cur].type) {
        case Identifier:
            root->child[root->childNum++] = MatchToken(Identifier, true);
            root->child[root->childNum++] = ParseIdentifierTableS();
            break;
        default:
            SyntaxError();
            break;
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseIdentifierTableS() {
    auto root = new AbstractSyntaxTreeNode;
    root->info.value = "IdentifierTableS";
    switch (lex[cur].type) {
        case Comma:
            root->child[root->childNum++] = MatchToken(Comma, false);
            root->child[root->childNum++] = ParseIdentifierTable();
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
    root->info.value = "StatementSequence";
    switch (lex[cur].type) {
        case IfKey:
        case WhileKey:
        case ForKey:
        case ScanfKey:
        case PrintfKey:
        case LeftBrace:
        case Identifier:
            root->child[root->childNum++] = ParseSentence();
            root->child[root->childNum++] = ParseStatementSequence();
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
    root->info.value = "Sentence";
    switch (lex[cur].type) {
        case IfKey:
            root->child[root->childNum++] = ParseIfSentence();
            break;
        case WhileKey:
            root->child[root->childNum++] = ParseWhileSentence();
            break;
        case ForKey:
            root->child[root->childNum++] = ParseForSentence();
            break;
        case ScanfKey:
            root->child[root->childNum++] = ParseScanfSentence();
            break;
        case PrintfKey:
            root->child[root->childNum++] = ParsePrintfSentence();
            break;
        case LeftBrace:
            root->child[root->childNum++] = ParseCompoundStatement();
            break;
        case Identifier:
        case Number:
        case LeftParentheses:
            root->child[root->childNum++] = ParseAssignmentStatement();
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseIfSentence() {
    auto root = new AbstractSyntaxTreeNode;
    root->info.value = "IfSentence";
    switch (lex[cur].type) {
        case IfKey:
            root->child[root->childNum++] = MatchToken(IfKey, false);
            root->child[root->childNum++] = MatchToken(LeftParentheses, false);
            root->child[root->childNum++] = ParseExpression();
            root->child[root->childNum++] = MatchToken(RightParentheses, false);
            root->child[root->childNum++] = ParseCompoundStatement();
            root->child[root->childNum++] = ParseElseSentence();
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseElseSentence() {
    auto root = new AbstractSyntaxTreeNode;
    root->info.value = "ElseSentence";
    switch (lex[cur].type) {
        case ElseKey:
            MatchToken(ElseKey, false);
            root->child[root->childNum++] = ParseCompoundStatement();
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
    return root;
}


AbstractSyntaxTreeNode* SyntaxAnalyse::ParseWhileSentence() {
    auto root = new AbstractSyntaxTreeNode;
    root->info.value = "WhileSentence";
    switch (lex[cur].type) {
        case WhileKey:
            root->child[root->childNum++] = MatchToken(WhileKey, false);
            root->child[root->childNum++] = MatchToken(LeftParentheses, false);
            root->child[root->childNum++] = ParseExpression();
            root->child[root->childNum++] = MatchToken(RightParentheses, false);
            root->child[root->childNum++] = ParseCompoundStatement();
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseForSentence() {
    auto root = new AbstractSyntaxTreeNode;
    root->info.value = "ForSentence";
    switch (lex[cur].type) {
        case ForKey:
            root->child[root->childNum++] = MatchToken(ForKey, false);
            root->child[root->childNum++] = MatchToken(LeftParentheses, false);
            root->child[root->childNum++] = ParseExpression();
            root->child[root->childNum++] = MatchToken(Semicolon, false);
            root->child[root->childNum++] = ParseExpression();
            root->child[root->childNum++] = MatchToken(Semicolon, false);
            root->child[root->childNum++] = ParseExpression();
            root->child[root->childNum++] = MatchToken(RightParentheses, false);
            root->child[root->childNum++] = ParseCompoundStatement();
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseCompoundStatement() {
    auto root = new AbstractSyntaxTreeNode;
    root->info.value = "CompoundStatement";
    switch (lex[cur].type) {
        case LeftBrace:
            root->child[root->childNum++] = MatchToken(LeftBrace, false);
            root->child[root->childNum++] = ParseStatementSequence();
            root->child[root->childNum++] = MatchToken(RightBrace, false);
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseAssignmentStatement() {
    auto root = new AbstractSyntaxTreeNode;
    root->info.value = "AssignmentStatement";
    switch (lex[cur].type) {
        case Identifier:
        case Number:
        case LeftParentheses:
            root->child[root->childNum++] = ParseExpression();
            root->child[root->childNum++] = MatchToken(Semicolon, false);
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseExpression() {
    AbstractSyntaxTreeNode *root = new AbstractSyntaxTreeNode;
    root -> info.value = "Expression";
    switch (lex[cur].type) {
        case Identifier:
            root ->child[root ->childNum++] =  MatchToken(Identifier, false);
            root -> child[root -> childNum++] =  ParseExpressionS();
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseExpressionS() {
    AbstractSyntaxTreeNode *root = new AbstractSyntaxTreeNode;
    root -> info.value = "ExpressionS";
    switch (lex[cur].type) {
        case Assignment:
            root -> child[root -> childNum++] = MatchToken(Assignment, false);
            root -> child[root -> childNum++] = ParseArithmeticExpression();
            break;
        case Add:
        case Subtract:
        case Multiply:
        case Divide:
        case GreaterThan:
        case LessThan:
        case NoGreaterThan:
        case NoLessThan:
        case Equal:
        case Unequal:
        case Semicolon:
        case RightParentheses:
            root -> child[root -> childNum++] = ParseRel();
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseRel() {
    AbstractSyntaxTreeNode* root = new AbstractSyntaxTreeNode;
    root -> info.value = "Rel";
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
            root -> child[root -> childNum++] = ParseOpt();
            root -> child[root -> childNum++] = ParseRelationOperator();
            root -> child[root -> childNum++] = ParseArithmeticExpression();
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
    root -> info.value = "Opt";
    switch (lex[cur].type) {
        case Add:
            root ->child[root -> childNum++] = MatchToken(Add, false);
            root ->child[root -> childNum++] = ParseArithmeticExpression();
            break;
        case Subtract:
            root -> child[root -> childNum++] = MatchToken(Subtract, false);
            root -> child[root -> childNum++] = ParseArithmeticExpression();
            break;
        case Multiply:
            root ->child[root -> childNum++] = MatchToken(Multiply, false);
            root ->child[root -> childNum++] = ParseArithmeticExpression();
            break;
        case Divide:
            root -> child[root -> childNum++] = MatchToken(Divide, false);
            root -> child[root -> childNum++] = ParseArithmeticExpression();
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
    root -> info.value = "RelationOperator";
    switch (lex[cur].type)
    {
        case GreaterThan:
            root -> child[root -> childNum++] = MatchToken(GreaterThan, false);
            break;
        case LessThan:
            root -> child[root -> childNum++] = MatchToken(LessThan, false);
            break;
        case NoLessThan:
            root -> child[root -> childNum++] = MatchToken(NoLessThan, false);
            break;
        case NoGreaterThan:
            root -> child[root -> childNum++] = MatchToken(NoGreaterThan, false);
            break;
        case Unequal:
            root -> child[root -> childNum++] = MatchToken(Unequal, false);
            break;
        case Equal:
            root -> child[root -> childNum++] = MatchToken(Equal, false);
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseArithmeticExpression()
{
    AbstractSyntaxTreeNode *root = new AbstractSyntaxTreeNode;
    root -> info.value = "ArithmeticExpression";
    switch (lex[cur].type)
    {
        case Identifier:
            root -> child[root -> childNum++] = ParseTerm();
            root -> child[root -> childNum++] = ParseOparit();
            break;
        case Number:
            root -> child[root -> childNum++] = ParseTerm();
            root -> child[root -> childNum++] = ParseOparit();
            break;
        case LeftParentheses:
            root -> child[root -> childNum++] = ParseTerm();
            root -> child[root -> childNum++] = ParseOparit();
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseOparit()
{
    AbstractSyntaxTreeNode* root = new AbstractSyntaxTreeNode;
    root -> info.value= "Oparit";
    switch (lex[cur].type)
    {
        case Add:
            root -> child[root -> childNum++] = MatchToken(Add, false);
            root -> child[root -> childNum++] = ParseArithmeticExpression();
            break;
        case Subtract:
            root -> child[root -> childNum++] = MatchToken(Subtract, false);
            root -> child[root -> childNum++] = ParseArithmeticExpression();
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
    root -> info.value = "Term";
    switch (lex[cur].type)
    {
        case Identifier:
            root -> child[root -> childNum++] = ParseFactor();
            root -> child[root -> childNum++] = ParseOpterm();
            break;
        case Number:
            root -> child[root -> childNum++] = ParseFactor();
            root -> child[root -> childNum++] = ParseOpterm();
            break;
        case LeftParentheses:
            root -> child[root -> childNum++] = ParseFactor();
            root -> child[root -> childNum++] = ParseOpterm();
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseOpterm()
{
    AbstractSyntaxTreeNode* root = new AbstractSyntaxTreeNode;
    root -> info.value = "Opterm";
    switch (lex[cur].type)
    {
        case Multiply:
            root -> child[root -> childNum++] = MatchToken(Multiply, false);
            root -> child[root -> childNum++] = ParseTerm();
            break;
        case Divide:
            root -> child[root -> childNum++] = MatchToken(Divide, false);
            root -> child[root -> childNum++] = ParseTerm();
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
    root -> info.value = "Factor";
    switch (lex[cur].type)
    {
        case Identifier:
            root -> child[root -> childNum++] = MatchToken(Identifier, false);
            break;
        case Number:
            root -> child[root -> childNum++] = MatchToken(Number, false);
            break;
        case LeftParentheses:
            root -> child[root -> childNum++] = MatchToken(LeftParentheses, false);
            root -> child[root -> childNum++] = ParseArithmeticExpression();
            root -> child[root -> childNum++] = MatchToken(RightParentheses, false);
            break;
        default:
            SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParseScanfSentence()
{
    auto root = new AbstractSyntaxTreeNode;
    root->info.value = "ScanfSentence";
    switch (lex[cur].type)
    {
        case ScanfKey:
            root -> child[root -> childNum++] = MatchToken(ScanfKey, false);
            root -> child[root -> childNum++] = MatchToken(LeftParentheses, false);
            root -> child[root -> childNum++] = MatchToken(Identifier,false);
            root -> child[root -> childNum++] = MatchToken(RightParentheses, false);
            root -> child[root -> childNum++] = MatchToken(Semicolon, false);
            break;
        default:SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::ParsePrintfSentence()
{
    AbstractSyntaxTreeNode *root = new AbstractSyntaxTreeNode;
    root -> info.value = "PrintfSentence";
    switch (lex[cur].type)
    {
        case PrintfKey:
            root -> child[root -> childNum++] = MatchToken(PrintfKey, false);
            root -> child[root -> childNum++] = MatchToken(LeftParentheses, false);
            root -> child[root -> childNum++] = MatchToken(Identifier, false);
            root -> child[root -> childNum++] = MatchToken(RightParentheses, false);
            root -> child[root -> childNum++] = MatchToken(Semicolon, false);
            break;
        default:SyntaxError();
    }
    return root;
}

AbstractSyntaxTreeNode* SyntaxAnalyse::Parse(Node *node) {
    this->lex = node;
    cur = 0;
    depth = 0;
    return ParseProgram();
}

void SyntaxAnalyse::ShowTree(AbstractSyntaxTreeNode *root, int depth) {
    for (int i = 0; i < depth; i++)
        printf("-- ");
    int idx = root->info.type;
    printf("%s %s\n", name[idx].c_str(), root->info.value.c_str());
    for (int i = 0; i < root->childNum; i++)
        ShowTree(root->child[i], depth + 1);
}

SyntaxAnalyse::SyntaxAnalyse() {
    cur = 0;
    depth = 0;
    lex = nullptr;
}




