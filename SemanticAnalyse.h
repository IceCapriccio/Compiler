//
// Created by IceCapriccio on 2018/11/30.
//

#ifndef COMPILER_SEMANTICANALYSE_H
#define COMPILER_SEMANTICANALYSE_H

#include <string>
#include "LexAnalyse.h"
#include <vector>

using namespace std;

/*
 * 叶子节点：node.type 有对应的值，value 为源程序的字符串
 * 非叶子节点： node.type 为 None，value 为空
 */
class AbstractSyntaxTreeNode{ // 树节点
public:
    Node info;
    AbstractSyntaxTreeNode *child[10];
    int childNum;
    AbstractSyntaxTreeNode();
};

class Symbol // 符号表项
{
public:
    string name;
    int depth, offset;
    Symbol(string name,int depth, int offset);
    void show();
};

struct SymbolTable { // 符号表
private:
    vector <Symbol> table;
public:
    void insert(string name, int depth);
    void del(int depth);
    int query(string name);
    void show();
};


#endif //COMPILER_SEMANTICANALYSE_H
