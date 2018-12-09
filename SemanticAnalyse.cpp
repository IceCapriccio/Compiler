//
// Created by IceCapriccio on 2018/11/30.
//

#include <iostream>
#include <cstdio>
#include "SemanticAnalyse.h"
#include "IntermediateCodeGenerate.h"

AbstractSyntaxTreeNode::AbstractSyntaxTreeNode() {
    info.type = None;
    childNum = 0;
    for (auto &i : child)
        i = nullptr;
}

Symbol::Symbol(string name, int depth, int offset):depth(depth), offset(offset) {
    this->name = name;
}

void Symbol::show() {
    printf("%5s %5d\n", name.c_str(), offset);
}

void SymbolTable::del(int depth) {
    for (int i = table.size() - 1; i >= 0; i--) {
        if (table[i].depth == depth)
            table.erase(table.begin() + i);
    }
}

int SymbolTable::query(string name) {
    for (auto it = table.rbegin(); it != table.rend(); it++) {
        if ((*it).name == name)
            return (*it).offset;
    }
    return -1;
}

void SymbolTable::insert(string name,int depth) {
    Symbol symbol(name,depth, table.size() * 4+10000);
    table.push_back(symbol);
}

void SymbolTable::show() {
    printf("%5s %s\n", "add", "val");
    for (auto &i : table) {
        i.show();
    }
}

