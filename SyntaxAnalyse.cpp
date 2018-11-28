//
// Created by IceCapriccio on 2018/11/28.
//

#include "SyntaxAnalyse.h"

Node lex[100]; // 语法分析传来的结构体数组
int cur = 0; // 当前分析到哪个单词

void SyntaxAnalyse::MatchToken(Type expect) {
    Type lexType = lex[cur].type;
    if (lexType != expect) {
        printf("expect %s, syntax error!", expect.value.c_str());
        exit(0);
    } else {
        cur++;
    }
}

void SyntaxAnalyse::ParseProgram() {
    ParseMainkey()
    MatchToken(LeftParentheses);
    MatchToken(RightParentheses);
    MatchToken(LeftBrace);
    ParseDeclarativeSequence();
    ParseStatmentSequence();
    MatchToken(RightBrace);
}
