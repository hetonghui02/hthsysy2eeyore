/****************************************************/
/*File: global.hpp                                   */
/*Define Global Variables and data types            */
/****************************************************/
#pragma once
#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <optional>
#include <memory>
#include <utility>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <vector>
#include <string>
#include <list>
using namespace std;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUEs
#define TRUE 1
#endif

#ifndef Debug
#define Debug 0
#endif

extern int blockNum;
extern int lineno;

extern int labelNum; 
extern int paraNum; 
extern int tempVarNum; 
extern int nativeVarNum; 

extern list<string> strtab;

typedef list<string>::iterator strIt;
inline strIt insertStrtab(const char* c){
    string tmp = c;
    strtab.push_front(move(tmp));
    return strtab.begin();
}


typedef enum{
    Base, Define, FunDef, FunCall, Block,
    Assign, If, While, Break, Continue, Return,
    BinaryExp, UnaryExp, Lval,
    ArrAST, ListTree_
} ASTtype;

class BaseAst {
public:
    virtual ~BaseAst() = default;
    /*virtual std::optional<int> Eval(IRGen &gen) const {
        throw std::runtime_error("Not implemented. Should not be called!");
    };
    virtual ValPtr GenerateIR(IRGen &gen) const {
        throw std::runtime_error("Not implemented. Should not be called!");
    };*/
    int is_decl = 0;
    //zero if this node is a variable, one if this node is a array;
    //-1 otherwise
    int is_array = -1;

    int is_lval_array = -1;

}; 
class BaseAST{
    public:
        ASTtype type;
        string addr;
        string true_; 
        string false_; 
        string begin;
        string next;

        BaseAST():type(Base){};
        ~BaseAST(){};
        virtual void generator(){};
        virtual void setVal(int){};
        virtual int getVal(){return -1;};
        virtual int cal(){return -1;};
        virtual bool isConst(){ return false; };
        virtual bool isNumber(){return false;} 
        int is_decl = 0;
        //zero if this node is a variable, one if this node is a array;
        //-1 otherwise
        int is_array = -1;

        int is_lval_array = -1;
};


extern list<BaseAST*> whileStack;

inline BaseAST* topWhileStack() {return whileStack.back();}
inline bool isEmptyWhileStack() {return whileStack.empty();}
inline void pushWhileStack(BaseAST* p) {whileStack.push_back(p); }
inline void popWhileStack() {whileStack.pop_back();}

extern BaseAST* currentFun;

#define tokenType int

const string token2string[] = {
   "const", "int", "void", "if",
   "else", "while", "break", "continue",
   "return","ident","&&", "||",
   "==", ">=", "<=", "!=",
   "+=", "-=", "*=", "/=",
   "=", ">", "<",
   "+", "-", "*", "/",
   "%", "!","(", ")",
   "[", "]",
   "{", "}",
   ";", ",",
   "HEX_INT", "OCT_INT", "DIGIT_INT","INT_CONST"
   "OTHER", "Minus_Not","Minus_Not_Not"
};



class Token{
    public:
      tokenType type;
      int val; //const int
      strIt strPtr; //identifier
      int lineNum;

      Token(tokenType type_):type(type_){};
      Token(tokenType type_, int val_):type(type_),val(val_){};
      Token(tokenType type_, const char* c):type(type_),strPtr(insertStrtab(c)){};
      Token(tokenType type_, const char* c, int lineno):type(type_),strPtr(insertStrtab(c)),lineNum(lineno){};


};


vector<int> getArrWidth(const vector<int> &arrDef);

int getArrAddr(const vector<int> &width, const vector<int> &indexs);

string num2string(int num);

void stringListPushBack(list<string> &List_, const string &ss);

extern BaseAST* mainptr;

#endif