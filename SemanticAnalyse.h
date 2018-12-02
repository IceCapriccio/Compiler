//
// Created by IceCapriccio on 2018/11/30.
//

#ifndef COMPILER_SEMANTICANALYSE_H
#define COMPILER_SEMANTICANALYSE_H

#include <string>
#include "LexAnalyse.h"

/*
 * 叶子节点：node.type 有对应的值，value 为源程序的字符串
 * 非叶子节点： node.type 为 None，value 为空
 * <exp> 以下有 value 和 op 值
 */
class AbstractSyntaxTreeNode{
    Node node;
    AbstractSyntaxTreeNode *child[10];
    int value;
    Type op;
};


#endif //COMPILER_SEMANTICANALYSE_H
