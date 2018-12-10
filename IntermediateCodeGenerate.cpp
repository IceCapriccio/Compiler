//
// Created by IceCapriccio on 2018/12/3.
//

#include "IntermediateCodeGenerate.h"

SymbolTable SyntaxAnalyse::symbolTable;
SupportTable supportTable;
int total = 0; // 当前共有多少四元式
string name[] = {"Identifier","Number","MainKey","IfKey","WhileKey","ForKey","ScanfKey","PrintfKey","ElseKey","IntKey","LeftParentheses","RightParentheses","LeftBrace","RightBrace","Comma","Semicolon","Quotation","Equal","Unequal","Assignment","LessThan","GreaterThan","NoLessThan","NoGreaterThan","Add","Subtract","Multiply","Divide","JEqual","JUnequal","JLessThan","JGreaterThan","JNoLessThan","JNoGreaterThan","Goto","DefLabel","None"};


CodeTerm::CodeTerm(Type op, int param1, int param2, int dest)
    : op(op), param1(param1), param2(param2), dest(dest) {
}

Type CodeTerm::getOp()
{
    return op;
}

int CodeTerm::getParam1()
{
    return param1;
}

int CodeTerm::getParam2()
{
    return param2;
}

int CodeTerm::getDest()
{
    return dest;
}

void CodeTerm::show() {
    int idx = op;
    cout << '(' << name[idx] << ',' << param1 << ',' << param2 << ',' << dest << ')' << endl;
//    //cout << '(' << name[idx] << ',' << supportTable.Query(param1) << ','
//        << supportTable.Query(param2) << ',' << supportTable.Query(dest) << ')' << endl;
}

CodeSequence::CodeSequence()
{
    op = None;
    op1 = None;
    param1 = -1;
    param2 = -1;
    param3 = -1;
}

void CodeSequence::setParam1(int param1)
{
    this->param1 = param1;
}

void CodeSequence::setParam2(int param2)
{
    this->param2 = param2;
}

void CodeSequence::setParam3(int param3)
{
    this->param3 = param3;
}

void CodeSequence::setOp(Type op)
{
    this->op = op;
}

void CodeSequence::setOp1(Type op1)
{
    this->op1 = op1;
}

int CodeSequence::getParam1()
{
    return param1;
}

int CodeSequence::getParam2()
{
    return param2;
}

int CodeSequence::getParam3()
{
    return param3;
}

Type CodeSequence::getOp()
{
    return op;
}

Type CodeSequence::getOp1()
{
    return op1;
}

CodeSequence Visitor::VisitProgram(AbstractSyntaxTreeNode *root) {
    // child[0] = main   child[1] = (   child[2] = )   child[3] = {
//    CodeSequence declarationSqeCode = VisitDeclaration(root->child[4]);
    CodeSequence sentenceSeqCode = VisitStatementSequence(root->child[5]);
    // child[6] = }
    CodeSequence code;
//    code.push_back(declarationSqeCode);
    code.push_back(sentenceSeqCode);
    return code;
}

CodeSequence Visitor::VisitStatementSequence(AbstractSyntaxTreeNode *root) {
    CodeSequence SentenceCode, SentenceSeqCode;
    if (root->child[0] != 0) { // SentenceSequence -> Sentence SentenceSequence
        SentenceCode = VisitSentence(root->child[0]);
        SentenceSeqCode = VisitStatementSequence(root->child[1]);
    } else if (root->child[0] == 0) { // SentenceSequence -> null

    }
    CodeSequence code;
    code.push_back(SentenceCode);
    code.push_back(SentenceSeqCode);
    return code;
}

CodeSequence Visitor::VisitIfSentence(AbstractSyntaxTreeNode *root, int IfSentence_next) {
    CodeSequence code;
    // node->child[0] = IfKey
    int Expression_true = supportTable.NewLabel(); // {Expression.true = newtemp}
    int Expression_false = IfSentence_next; // {Expression.false = newtemp}
    // node->child[1] = (
    CodeSequence ExpCode = VisitExpression(root->child[2], Expression_true, Expression_false);
    code.push_back(ExpCode);
    CodeTerm codeTerm = CodeTerm(DefLabel, -1, -1, Expression_true);
    code.push_back(codeTerm);
    supportTable.LabelAssign(Expression_true, total); // {expression.true :}
    total++;
    // node->child[3] = )
    CodeSequence CompCode = VisitCompoundStatement(root->child[4]);
    code.push_back(CompCode);
    CodeSequence ElseCode = VisitElseSentence(root->child[5]);
    if (ElseCode.GetSize() != 0) { // ElseSentence -> else CompoundSentence
        supportTable.LabelAssign(Expression_false, total);
        codeTerm = CodeTerm(DefLabel, -1, -1, Expression_false);
        code.push_back(codeTerm); // {expression.false :}
        total++;
        code.push_back(ElseCode);
    } else { // ElseSentence -> null
        supportTable.LabelAssign(Expression_false, total);
    }
    // IfSentence.code = Expression.code || "Expression.true :" || CompoundStatement.code ||
    //                   "Expression.false: " || ElseSentence.code
    return code;
}

CodeSequence Visitor::VisitElseSentence(AbstractSyntaxTreeNode *root) {
    CodeSequence code, CpdStmtCode;
    if (root->childNum == 2) { // ElseSentence -> else CompoundSentence
        // child[0] = else
        CpdStmtCode = VisitCompoundStatement(root->child[1]);
    }  else if (root->childNum == 0) { // ElseSentence -> null

    }
    code.push_back(CpdStmtCode);
    return code;
}

CodeSequence Visitor::VisitSentence(AbstractSyntaxTreeNode *root) {
    CodeSequence code, IfCode, WhileCode, ForCode, ScanfCode, PrintfCode, CpdStmtCode, AssignCode;
    int next = supportTable.NewLabel();
    if (root->child[0]->info.value == "IfSentence") { // Sentence -> IfSentence
        IfCode = VisitIfSentence(root->child[0], next);
        code.push_back(IfCode);
        supportTable.LabelAssign(next, total);
        CodeTerm codeTerm(DefLabel, -1, -1, next);
        code.push_back(codeTerm);
        total++;
    } else if (root->child[0]->info.value == "WhileSentence") { // Sentence -> WhileSentence
        WhileCode = VisitWhileSentence(root->child[0], next);
        code.push_back(WhileCode);
        supportTable.LabelAssign(next, total);
        CodeTerm codeTerm(DefLabel, -1, -1, next);
        code.push_back(codeTerm);
        total++;
    } else if (root->child[0]->info.value == "ForSentence") { // Sentence -> ForSentence
        ForCode = VisitForSentence(root->child[0], next);
        code.push_back(ForCode);
        supportTable.LabelAssign(next, total);
        CodeTerm codeTerm(DefLabel, -1, -1, next);
        code.push_back(codeTerm);
        total++;
    } else if (root->child[0]->info.value == "ScanfSentence") { // Sentence -> ScanfSentence
        ScanfCode = VisitScanfSentence(root->child[0]);
        code.push_back(ScanfCode);
    } else if (root->child[0]->info.value == "PrintfSentence") { // Sentence -> PrintfSentence
        PrintfCode = VisitPrintSentence(root->child[0]);
        code.push_back(PrintfCode);
    } else if (root->child[0]->info.type == LeftBrace) { // Sentence -> { CompoundStatement }
        // child[0] = {
        CpdStmtCode = VisitCompoundStatement(root->child[1]);
        // child[2] = }
        code.push_back(CpdStmtCode);
    } else if (root->child[0]->info.value == "AssignmentStatement") { // Sentence -> AssignmentStatement
        AssignCode = VisitAssignmentStatement(root->child[0], -1, -1);
        code.push_back(AssignCode);
    }
    return code;
}

CodeSequence Visitor::VisitCompoundStatement(AbstractSyntaxTreeNode *root) {
    CodeSequence code, StmtSeqCode;
    // child[0] = {
    StmtSeqCode = VisitStatementSequence(root->child[1]);
    // child[2] = }
    code.push_back(StmtSeqCode);
    return code;
}

CodeSequence Visitor::VisitWhileSentence(AbstractSyntaxTreeNode *root, int WhileSentence_next) {
    CodeSequence ExpCode, CpdStmtCode, code;
    // child[0] = while
    // child[1] = (
    int Expression_true = supportTable.NewLabel();
    int Expression_false = WhileSentence_next;
    ExpCode = VisitExpression(root->child[2], Expression_true, Expression_false);
    code.push_back(ExpCode);
    // child[3] = )
    supportTable.LabelAssign(Expression_true, total); // {expression.true :}
    CodeTerm codeTerm = CodeTerm(DefLabel, -1, -1, Expression_true);
    code.push_back(codeTerm);
    total++;
    CpdStmtCode = VisitCompoundStatement(root->child[4]);
    code.push_back(CpdStmtCode);
    return code;
}

CodeSequence Visitor::VisitForSentence(AbstractSyntaxTreeNode *root, int ForSentence_next) {
    CodeSequence code, Exp1Code, Exp2Code, Exp3Code, CpdStmtCode;
    // child[0] = for
    // child[1] = (
    Exp1Code = VisitExpression(root->child[2], -1, -1);
    code.push_back(Exp1Code);
    if(Exp1Code.getOp() == Assignment && Exp1Code.getParam1() && Exp1Code.getParam2())
    {
        CodeTerm codeTerm1(Exp1Code.getOp(),Exp1Code.getParam2(),-1,Exp1Code.getParam1());
        code.push_back(codeTerm1);
        total++;
    }
    int expression2_true = supportTable.NewLabel();
    int expression2_false = ForSentence_next;
    int expression2_begin = supportTable.NewLabel();
    supportTable.LabelAssign(expression2_begin, total);
    CodeTerm codeTerm(DefLabel, -1, -1, expression2_begin);
    code.push_back(codeTerm);
    total++;
    // child[3] = ;
    Exp2Code = VisitExpression(root->child[4], expression2_true, expression2_false);
    code.push_back(Exp2Code);
    codeTerm = CodeTerm(DefLabel, -1, -1, expression2_true);
    supportTable.LabelAssign(expression2_true, total);
    code.push_back(codeTerm);
    total++;
    // child[5] = ;
    // child[6] = Exp3
    CpdStmtCode = VisitCompoundStatement(root->child[8]);
    code.push_back(CpdStmtCode);
    // child[7] = )
    // child[8] = CompoundStatement
    Exp3Code = VisitExpression(root->child[6], -1, -1);
    code.push_back(Exp3Code);
    if(Exp3Code.getOp() == Assignment && Exp3Code.getParam1() && Exp3Code.getParam2())
    {
        CodeTerm codeTerm1(Exp3Code.getOp(),Exp3Code.getParam2(),-1,Exp3Code.getParam1());
        code.push_back(codeTerm1);
        total++;
    }
    codeTerm = CodeTerm(Goto, -1, -1, expression2_begin);
    code.push_back(codeTerm);
    total++;
    return code;
}

CodeSequence Visitor::VisitAssignmentStatement(AbstractSyntaxTreeNode *node,int Expression_true, int Expression_false)
{
    CodeSequence Expression, code;
    if(node -> child[0] != 0)
    {
        Expression = VisitExpression(node->child[0], Expression_true, Expression_false);
        int tmp = Expression.getParam2();
        code.setParam2(tmp);
        code.push_back(Expression);
        if (Expression.getParam1() != -1 && Expression.getParam2() != -1 && Expression.getOp() != None) {
            if (Expression.getOp() == Assignment) {
                CodeTerm codeTerm(Expression.getOp(), Expression.getParam2(), -1, Expression.getParam1());
                total++;
                code.push_back(codeTerm);
            } else {
                int dest = supportTable.NewLabel();
                CodeTerm codeTerm(Expression.getOp(), Expression.getParam1(), Expression.getParam2(), dest);
                total++;
                code.push_back(codeTerm);
                code.setParam2(dest);
                /*if (Expression.getParam3() != -1 && Expression.getOp1() != None) {
                    int dest1 = supportTable.NewLabel();
                    CodeTerm codeTerm1(Expression.getOp1(), dest, Expression.getParam3(), dest1);
                    total++;
                    code.push_back(codeTerm1);
                }*/
            }
        }
    }
    //cout<<"AssignmentStatement:"<<code.getParam1()<<" "<<name[code.getOp()]<<" "<<code.getParam2()<<endl;
    return code;
}

CodeSequence Visitor::VisitExpression(AbstractSyntaxTreeNode *node,int Expression_true, int Expression_false)
{
    CodeSequence ExpressionS, code;
    if(node->child[1] != 0)
    {
        ExpressionS = VisitExpressionS(node->child[1]);
        code.push_back(ExpressionS);
    }
    /*if(ExpressionS.getParam1() != -1 && ExpressionS.getParam2() != -1 && ExpressionS.getOp() != None)
    {
        int dest = supportTable.NewLabel();
        CodeTerm codeTerm(ExpressionS.getOp(),ExpressionS.getParam1(),ExpressionS.getParam2(),dest);
        total++;
        code.push_back(codeTerm);
        code.setParam2(dest);
    }*/
    /*else if(ExpressionS.getParam2() == -1)
    {
        ExpressionS.setParam2(SyntaxAnalyse::symbolTable.query(node->child[0]->info.value));
        code.setParam2(ExpressionS.getParam2());
    }*/
    //else if(ExpressionS.getParam1() == -1)
    //{
    code.setParam1(SyntaxAnalyse::symbolTable.query(node->child[0]->info.value));
    code.setParam2(ExpressionS.getParam2());
    code.setOp(ExpressionS.getOp());
    //}
    if(code.getParam1() != -1 && code.getParam2() != -1 && code.getOp() != Number)
    {
        if(code.getOp() != Assignment)
        {
            int dest = supportTable.NewLabel();
            CodeTerm codeTerm(code.getOp(),code.getParam1(),code.getParam2(),dest);
            code.setParam1(dest);
            total++;
            code.push_back(codeTerm);
            code.setParam2(-1);
        }
    }
    if(ExpressionS.getParam3() != -1 && ExpressionS.getOp1() != None)
    {
        int dest = supportTable.NewLabel();
        CodeTerm codeTerm(ExpressionS.getOp1(),code.getParam1(),ExpressionS.getParam3(),dest);
        code.setParam1(dest);
        code.push_back(codeTerm);
        total++;
        if(Expression_true != -1)
        {
            CodeTerm codeGoto(JEqual,dest,-2,Expression_true);
            total++;
            code.push_back(codeGoto);
            CodeTerm codeFalse(Goto,-1,-1,Expression_false);
            total++;
            code.push_back(codeFalse);
        }
    }
    code.setParam3(ExpressionS.getParam3());
    code.setOp1(ExpressionS.getOp1());
    //cout<<"Expression:"<<code.getParam1()<<" "<<name[code.getOp()]<<" "<<code.getParam2()<<endl;
    return code;
}

CodeSequence Visitor::VisitExpressionS(AbstractSyntaxTreeNode *node)
{
    CodeSequence ArithmeticExpression, Rel;
    CodeSequence code;
    if(node->child[0] != 0)
    {
        if(node -> child[0]->info.type == Assignment)
        {
            code.setOp(Assignment);
            if(node -> child[1] != 0)
            {
                ArithmeticExpression = VisitArithmeticExpression(node->child[1]);
                code.setParam2(ArithmeticExpression.getParam2());
            }
            code.push_back(ArithmeticExpression);
        }
        else
        {
            Rel = VisitRel(node->child[0]);
            code.push_back(Rel);
            if(Rel.getParam2() != -1 && Rel.getParam1() != -1 && Rel.getOp() != None)
            {
                int dest = supportTable.NewLabel();
                CodeTerm codeTerm(Rel.getOp(),Rel.getParam1(),Rel.getParam2(),dest);
                code.setParam2(dest);
                total++;
                code.push_back(codeTerm);
            }
            else
            {
                code.setParam2(Rel.getParam2());
                code.setOp(Rel.getOp());
            }
            code.setParam3(Rel.getParam3());
            code.setOp1(Rel.getOp1());
        }
    }
    //cout<<"ExpressionS:"<<code.getParam1()<<" "<<name[code.getOp()]<<" "<<code.getParam2()<<endl;
    return code;
}

CodeSequence Visitor::VisitRel(AbstractSyntaxTreeNode *node)
{
    CodeSequence Opt, RelationOperator, ArithmeticExpression;
    CodeSequence code;
    if (node->child[0] != 0)
    {
        Opt = VisitOpt(node->child[0]);
        code.push_back(Opt);
        code.setOp(Opt.getOp());
        code.setParam2(Opt.getParam2());
    }
    if(node -> child[1] != 0)
    {
        RelationOperator = VisitRelationOperator(node->child[1]);
        code.push_back(RelationOperator);
        code.setOp1(RelationOperator.getOp1());
    }
    if(node->child[2] != 0)
    {
        ArithmeticExpression = VisitArithmeticExpression(node->child[2]);
        code.push_back(ArithmeticExpression);
        if(ArithmeticExpression.getParam1() != -1 && ArithmeticExpression.getParam2() != -1 && ArithmeticExpression.getOp() != None)
        {
            int dest = supportTable.NewLabel();
            CodeTerm codeTerm(ArithmeticExpression.getOp(),ArithmeticExpression.getParam1(),ArithmeticExpression.getParam2(),dest);
            total++;
            code.push_back(codeTerm);
            code.setParam3(dest);
        }
        else
        {
            code.setParam3(ArithmeticExpression.getParam2());
        }
    }
    //cout<<"Rel:"<<code.getParam1()<<" "<<name[code.getOp()]<<" "<<code.getParam2()<<endl;
    return code;
}

CodeSequence Visitor::VisitOpt(AbstractSyntaxTreeNode *node)
{
    CodeSequence ArithmeticExpression, code;
    if(node->child[0] != 0)
    {
        code.setOp(node->child[0]->info.type);
    }
    if(node->child[1] != 0)
    {
        ArithmeticExpression = VisitArithmeticExpression(node->child[1]);
        code.push_back(ArithmeticExpression);
        code.setParam2(ArithmeticExpression.getParam2());
    }
    //cout<<"Opt:"<<code.getParam1()<<" "<<name[code.getOp()]<<" "<<code.getParam2()<<endl;
    return code;
}

CodeSequence Visitor::VisitRelationOperator(AbstractSyntaxTreeNode *node)
{
    CodeSequence code;
    if(node->child[0] != 0)
    {
        code.setOp1(node->child[0]->info.type);
    }
    //cout<<"ORelationOperator:"<<code.getParam1()<<" "<<name[code.getOp()]<<" "<<code.getParam2()<<endl;
    return code;
}

CodeSequence Visitor::VisitArithmeticExpression(AbstractSyntaxTreeNode *node)
{
    CodeSequence Term,Oparit,code;
    if(node->child[0] != 0)
    {
        Term = VisitTerm(node->child[0]);
        code.push_back(Term);
        if(Term.getParam1() != -1 && Term.getParam2() != -1 && Term.getOp() != None)
        {
            int dest = supportTable.NewLabel();
            CodeTerm codeTerm(Term.getOp(),Term.getParam1(),Term.getParam2(),dest);
            total++;
            code.push_back(codeTerm);
            code.setParam2(dest);
        }
        else
        {
            code.setParam2(Term.getParam2());
            code.setOp(Term.getOp());
        }
    }
    if(node->child[1] != 0)
    {
        Oparit = VisitOparit(node->child[1]);
        code.push_back(Oparit);
        if(Oparit.getOp() != None && code.getParam2() != -1)
        {
            int dest = supportTable.NewLabel();
            CodeTerm codeTerm(Oparit.getOp(),code.getParam2(),Oparit.getParam2(),dest);
            code.setParam2(dest);
            total++;
            code.push_back(codeTerm);
        }
    }
    //cout<<"ArithmeticExpression:"<<code.getParam1()<<" "<<name[code.getOp()]<<" "<<code.getParam2()<<endl;
    return code;
}

CodeSequence Visitor::VisitOparit(AbstractSyntaxTreeNode *node)
{
    CodeSequence ArithmeticExpression, code;
    if(node->child[0] != 0)
    {
        code.setOp(node->child[0]->info.type);
        ArithmeticExpression = VisitArithmeticExpression(node->child[1]);
        code.push_back(ArithmeticExpression);
        code.setParam2(ArithmeticExpression.getParam2());
    }
    //cout<<"Oparit:"<<code.getParam1()<<" "<<name[code.getOp()]<<" "<<code.getParam2()<<endl;
    return code;
}

CodeSequence Visitor::VisitTerm(AbstractSyntaxTreeNode *node)
{
    CodeSequence Factor, Opterm, code;
    if(node->child[0] != 0)
    {
        Factor = VisitFactor(node->child[0]);
        Opterm = VisitOpterm(node->child[1]);
        code.push_back(Factor);
        code.push_back(Opterm);
        if(Factor.getParam1() != -1 && Factor.getParam2() != -1 && Factor.getOp() != None)
        {
            int dest = supportTable.NewLabel();
            CodeTerm codeTerm(Factor.getOp(),Factor.getParam1(),Factor.getParam2(),dest);
            total++;
            code.push_back(codeTerm);
            code.setParam2(dest);
        }
        else
        {
            code.setParam2(Factor.getParam2());
            code.setOp(Factor.getOp());
        }
        if(code.getParam2() != -1 && Opterm.getParam2() != -1 && Opterm.getOp() != None)
        {
            int dest = supportTable.NewLabel();
            CodeTerm codeTerm(Opterm.getOp(),code.getParam2(),Opterm.getParam2(),dest);
            total++;
            code.push_back(codeTerm);
            code.setParam2(dest);
        }
    }
    //cout<<"Term:"<<code.getParam1()<<" "<<name[code.getOp()]<<" "<<code.getParam2()<<endl;
    return code;
}

CodeSequence Visitor::VisitOpterm(AbstractSyntaxTreeNode *node)
{
    CodeSequence Term, code;
    if(node->child[0] != 0)
    {
        Term = VisitTerm(node->child[1]);
        code.push_back(Term);
        code.setParam2(Term.getParam2());
        code.setOp(node->child[0]->info.type);
    }
    //cout<<"Opterm:"<<code.getParam1()<<" "<<name[code.getOp()]<<" "<<code.getParam2()<<endl;
    return code;
}

CodeSequence Visitor::VisitFactor(AbstractSyntaxTreeNode *node)
{
    CodeSequence ArithmeticExpression, code;
    if(node->child[0]->info.type == Identifier)
    {
        code.setParam2(SyntaxAnalyse::symbolTable.query(node->child[0]->info.value));
    }
    else if(node->child[0]->info.type==Number)
    {
        int dest = supportTable.NewLabel();
        supportTable.LabelAssign(dest,atoi(node->child[0]->info.value.c_str()));
        code.setParam2(dest);
    }
    else
    {
        ArithmeticExpression = VisitArithmeticExpression(node->child[1]);
        code.push_back(ArithmeticExpression);
        code.setParam2(ArithmeticExpression.getParam2());
        code.setOp(ArithmeticExpression.getOp());
    }
    //cout<<"Factor:"<<code.getParam1()<<" "<<name[code.getOp()]<<" "<<code.getParam2()<<endl;
    return code;
}

CodeSequence Visitor::VisitScanfSentence(AbstractSyntaxTreeNode *node)
{
    CodeSequence code;
    int addr = SyntaxAnalyse::symbolTable.query(node->child[2]->info.value);
    CodeTerm codeTerm(ScanfKey,addr,-1,-1);
    total++;
    code.push_back(codeTerm);
    return code;
}

CodeSequence Visitor::VisitPrintSentence(AbstractSyntaxTreeNode *node)
{
    CodeSequence code;
    int addr = SyntaxAnalyse::symbolTable.query(node->child[2]->info.value);
    CodeTerm codeTerm(PrintfKey,addr,-1,-1);
    total++;
    code.push_back(codeTerm);
    return code;
}

CodeSequence Visitor::Visit(AbstractSyntaxTreeNode *root) {
    return VisitProgram(root);
}

int SupportTable::NewLabel() {
    int size = table.size();
    table[size] = -1;
    return size;
}

void SupportTable::LabelAssign(int addr, int value) {
    table[addr] = value;
}

int SupportTable::Query(int addr) {
    return table[addr];
}

SupportTable::SupportTable() {
    table[-2] = true;
    table[-1] = -1;
}

void SupportTable::show() {
    printf("add   val\n");
    for (auto it = table.begin(); it != table.end(); it++)
        printf("%5d %5d\n", it->first, it->second);
}

int CodeSequence::GetSize() {
    return code.size();
}

void CodeSequence::push_back(CodeTerm codeTerm) {
    code.push_back(codeTerm);
}

void CodeSequence::push_back(CodeSequence codeSequence) {
    for (int i = 0; i < codeSequence.GetCode().size(); i++) {
        code.push_back(codeSequence.GetCode()[i]);
    }
}

vector<CodeTerm> CodeSequence::GetCode() {
    return code;
}

void CodeSequence::show() {
    cout << "supportTable:" << endl;
    supportTable.show();
    cout << "SymbolTable:" << endl;
    SyntaxAnalyse::symbolTable.show();
    cout << "TAC:" << endl;
    for (int i = 0; i < code.size(); i++) {
        printf("%2d ", i);
        code[i].show();
    }
}


