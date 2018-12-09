#include <iostream>
#include "LexAnalyse.h"
#include "SyntaxAnalyse.h"
#include "SemanticAnalyse.h"
#include "IntermediateCodeGenerate.h"

using namespace std;

int main() {
    extern int x;
    LexAnalyse lexAnalyse;
    SyntaxAnalyse syntaxAnalyse;
    Visitor visitor;
    Node *lex = lexAnalyse.LexAnalysis();
    AbstractSyntaxTreeNode* root = syntaxAnalyse.Parse(lex);
//    freopen("../SyntaxTree.txt", "w", stdout);
//    syntaxAnalyse.ShowTree(root, 0);
//    freopen("CON", "w", stdout);
    CodeSequence codeSequence = visitor.Visit(root);
    codeSequence.show();
    return 0;
}