#pragma once
#ifndef SYMTAB_HPP
#define SYMTAB_HPP

#include <string>
#include <vector>
#include <list>
#include <set>
#include "global.hpp"
#include <iostream>
#include <unordered_set>
using namespace std;


class varSymEntry; class funSymEntry;
class symTab;

extern symTab* const globalSymTab;
extern hash<string> hashstr;
typedef enum{varSym, funSym} entryType;
class BaseEntry{
    public:
        BaseAST* father; //指向创建符号表的AST node
        entryType type;
        BaseEntry(strIt str, BaseAST* ast):strp(str),father(ast),fngprnt(hashstr(*str)){};
        ~BaseEntry(){};
        list<string>::iterator strp; //表项名
        bool operator != (const BaseEntry* tmp){
            return (strp->c_str() != tmp->strp->c_str());
        }
        bool operator == (const BaseEntry* tmp){
            return (strp->c_str() == tmp->strp->c_str());
        }
        bool operator < (const BaseEntry* tmp){
            return *strp < *(tmp->strp);
        }
        virtual void justAVirtual(){};
        const size_t fngprnt; // Fingerprint of the token name. Compute it once for all.
};
class BaseEntryCompare{
    public:
        bool operator()(const BaseEntry* entry1, const BaseEntry* entry2) const {
            return ( entry1->strp->c_str() < entry2->strp->c_str());
        }
};


class varSymEntry:public BaseEntry{
    public:
        const bool isConst;
        vector<int> dims; //记录数组width，例如a[3][2],为1,2,6
        void setDims(vector<int> && a);

        varSymEntry(bool isConst_, Token* token, BaseAST* ast): \
        BaseEntry(token->strPtr, ast),isConst(isConst_){
            type = varSym;
        };
        ~varSymEntry();
};

class funSymEntry:public BaseEntry{
    public:
        const bool isVoid;
        vector<BaseEntry*> fParams;
        void insertParams(BaseEntry*);

        funSymEntry(bool isVoid_,Token* token, BaseAST* ast): \
        BaseEntry(token->strPtr, ast),isVoid(isVoid_){
            type = funSym;
        };
        ~funSymEntry();
};

class EqualSYM{
public:
	bool operator () (BaseEntry* p, BaseEntry* q) const {
		return *(p->strp) == *(q->strp);
	}
};

class HashSYM{
public:
	size_t operator () (BaseEntry* p) const {return p->fngprnt;}
};

class symTab{
    public:
        symTab(int blockN_, symTab* father_ = nullptr): father(father_), blockN(blockN_) {}
	    ~symTab(){for(auto i: table) delete i; for(auto i: children) delete i;}; 
        unordered_set<BaseEntry*, HashSYM, EqualSYM> table;
        vector<symTab*> children;
        symTab* father;
        int blockN;
        void printSymtab(){
            if(Debug){
                printf("symTab blockN = %d :\n",blockN);
                int entryNum = 0;
                for(auto i: table){
                    printf("%d: enrty name is: %s\n",entryNum++,i->strp->c_str());
                }
            }else return ;
        }
};


BaseEntry* newVarEntry(bool isConst, Token* token, BaseAST* ast);
BaseEntry* newFunEntry(bool isVoid, Token* token, BaseAST* ast);

void pushSymtab();
void popSymtab();

void insertEntry(BaseEntry*);

BaseEntry* findString(strIt strp);
BaseEntry* checkIfDecl(BaseEntry* entry);

#endif