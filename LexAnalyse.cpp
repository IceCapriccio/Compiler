//
// Created by IceCapriccio on 2018/11/18.
//
#include<stdlib.h>
#include <iostream>
#include <cstdio>
#include "LexAnalyse.h"

using namespace std;

struct Node p[100];
int x = 0;
string T[8]= {"main","if","else","for","while","int","scanf","printf"};


struct Node *LexAnalyse::LexAnalysis()
{
    freopen("../code.txt", "r", stdin);
    string s3;
    string s;
    while(cin>>s)
    {
        int len=s.length();
        string temp="";
        for(int i=0; i<len; i++)
        {
            //操作符
            if (s[i] == '=' || s[i] == '+' || s[i] == '-'||s[i] == '*'|| s[i] == '/' || s[i] == '<' || s[i] == '>' || s[i] == '!')
            {
                if(temp.length())
                {
                    panduan(temp);
                }
                temp="";
                if(i+1<len&&s[i+1]=='=')
                {
                    s3[0] = s[i];
                    s3[1] = s[i+1];
                    s3[2] = '\0';
                    if(s3[0] == '=' && s3[1] == '=')
                        p[x].type = Equal;
                    if(s3[0] == '!' && s3[1] == '=')
                        p[x].type = Unequal;
                    if(s3[0] == '<' && s3[1] == '=')
                        p[x].type = NoGreaterThan;
                    if(s3[0] == '>' && s3[1] == '=')
                        p[x].type = NoLessThan;
                    p[x].value=s3;
                    x++;
                    i++;
                }
                else
                {
                    if(s[i]=='=')
                        p[x].type = Assignment;
                    if(s[i]=='<')
                        p[x].type = LessThan;
                    if(s[i]=='>')
                        p[x].type = GreaterThan;
                    if(s[i]=='+')
                        p[x].type = Add;
                    if(s[i]=='-')
                        p[x].type = Subtract;
                    if(s[i]=='*')
                        p[x].type = Multiply;
                    if(s[i]=='/')
                        p[x].type = Divide;
                    p[x].value=s[i];
                    x++;
                }
            }
            //界符
            else if(s[i] == '(' || s[i] == ')' || s[i] == '{'||s[i] == '}'|| s[i] == ',' || s[i] ==';'||s[i]=='\"')
            {
                if(temp.length())
                {
                    panduan(temp);
                }
                temp="";
                if(s[i]=='(')
                    p[x].type = LeftParentheses;
                if(s[i]==')')
                    p[x].type = RightParentheses;
                if(s[i]=='{')
                    p[x].type = LeftBrace;
                if(s[i]=='}')
                    p[x].type = RightBrace;
                if(s[i]==',')
                    p[x].type = Comma;
                if(s[i]==';')
                    p[x].type = Semicolon;
                if(s[i]=='\"')
                    p[x].type = Quotation;
                p[x].value = s[i];
                x++;
            }
            //不是界符也不是操作符，就存到临时字符串里面，等待判断
            else
            {
                temp=temp+s[i];
            }
        }
        if(temp.length())
        {
            panduan(temp);
        }
    }
    return p;
}
void LexAnalyse::panduan(string s)
{
    int flag;
    int j;
    for(j = 0; j < s.length(); j++)
    {
        if(s[j]>='0'&&s[j]<='9')
        {
            flag = 1;
        }
        else
        {
            flag = 0;
            break;
        }
    }
    if(flag == 1)
    {
        p[x].type = Number;
        p[x].value = s;
        x++;
    }
    else
    {
        int f=1;
        if(s==T[0])
        {
            f=0;
            p[x].type = MainKey;
            p[x].value = s;
            x++;
        }
        if(s==T[1])
        {
            f=0;
            p[x].type = IfKey;
            p[x].value = s;
            x++;
        }
        if(s==T[2])
        {
            f=0;
            p[x].type = ElseKey;
            p[x].value = s;
            x++;
        }
        if(s==T[3])
        {
            f=0;
            p[x].type = ForKey;
            p[x].value = s;
            x++;
        }
        if(s==T[4])
        {
            f=0;
            p[x].type = WhileKey;
            p[x].value = s;
            x++;
        }
        if(s==T[5])
        {
            f=0;
            p[x].type = IntKey;
            p[x].value = s;
            x++;
        }
        if(s==T[6])
        {
            f=0;
            p[x].type = ScanfKey;
            p[x].value = s;
            x++;
        }
        if(s==T[7])
        {
            f=0;
            p[x].type = PrintfKey;
            p[x].value = s;
            x++;
        }
        if(f==1)
        {
            p[x].type = Identifier;
            p[x].value = s;
            x++;
        }
    }
}
