#pragma once
#ifndef AST_HPP
#define AST_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "symtab.hpp"
#include "global.hpp"
using namespace std;

class DefineAST;
class FunDefAST; class FunCallAST; class BlockAST;
class AssignAST; class IfAST; class WhileAST; class BreakAST;
class ContinueAST; class ReturnAST;
class BinaryExpAST; class UnaryExpAST; class LvalAST;
class arrAST; class ListTree;

class DefineAST : public BaseAST{
    public:
        BaseEntry* sym;
        map<int, BaseAST*> mmap; //使用map处理变量声明部分 
        BaseAST* getMmap(int index);    

        DefineAST():sym(nullptr){type=Define;}
        ~DefineAST(){
            for(auto i:mmap){
                if(i.second) delete i.second;
            }
        }
        virtual void generator();
        BaseAST* getInitVal(int index);
};


class FunDefAST : public BaseAST{
    public:
        BaseEntry* sym;
        string name;
        BaseAST* body;
        vector<BaseAST*> fParams; //形参

        FunDefAST():sym(nullptr),body(nullptr){type=FunDef;};
        ~FunDefAST(){
            for(auto i:fParams){
                if(i) delete i;
            }
            if(body) delete body;
        }
        virtual void generator();
};

class FunCallAST : public BaseAST{
    public:
        BaseEntry* sym;
        vector<BaseAST*> rParams; //实参

        FunCallAST(){type=FunCall;};
        FunCallAST(Token* token);
        ~FunCallAST(){
            for(auto i:rParams)
                if(i) delete i;
        }
        bool checkParaSize(); //检查实参是否与形参数目相同
        virtual void generator();
        virtual bool isConst(){return false;}
        virtual int getVal(){return 0;}
        virtual bool isNumber(); //如果是int类型返回true，其他返回false
};

class BlockAST : public BaseAST{
    public:
        const int blockN;
        vector<BaseAST*> items;

        BlockAST():blockN(blockNum){type=Block;};
        ~BlockAST();
        virtual void generator();

        void insertItem(BaseAST* ptr){if(ptr) items.push_back(ptr);}
        void insertItems(vector<BaseAST*> &&ptr){
            for(auto i : ptr)
                items.push_back(i);
        }
};


class AssignAST : public BaseAST{
    public:
        BaseAST* lval;
        BaseAST* exp;

        AssignAST():lval(nullptr),exp(nullptr){type=Assign;}
        ~AssignAST(){
            if(lval) delete lval;
            if(exp) delete exp;
        }
        virtual void generator();
};

class IfAST : public BaseAST{
    public:
        BaseAST* cond;
        BaseAST* stmt;
        BaseAST* elseStmt;

        IfAST():cond(nullptr),stmt(nullptr),elseStmt(nullptr){type=If;};
        ~IfAST(){
            if(cond) delete cond;
            if(stmt) delete stmt;
            if(elseStmt) delete elseStmt;
        }
        virtual void generator();
};

class WhileAST : public BaseAST{
    public:
        BaseAST* cond;
        BaseAST* stmt;

        WhileAST():cond(nullptr),stmt(nullptr){type=While;}
        ~WhileAST(){
            if(cond) delete cond;
            if(stmt) delete stmt;
        }
        virtual void generator();
};

class BreakAST : public BaseAST{
    public:
        BaseAST* nextCode;

        BreakAST():nextCode(topWhileStack()){type=Break;}
        ~BreakAST();
        virtual void generator();
};

class ContinueAST : public BaseAST{
    public:
        BaseAST* nextCode;

        ContinueAST():nextCode(topWhileStack()){type=Continue;}
        ~ContinueAST();
        virtual void generator();
};
 
class ReturnAST : public BaseAST{
    public:
        BaseAST* nextCode;
        BaseAST* exp;

        ReturnAST():nextCode(currentFun),exp(nullptr){type=Return;}
        ~ReturnAST();
        virtual void generator();
};


class BinaryExpAST : public BaseAST{
    public:
        BaseAST* firstExp;
        BaseAST* secExp;
        int op;
        int val;
        bool isConst_;
        bool isCaled_;

        BinaryExpAST(){type=BinaryExp;}
        BinaryExpAST(Token* token);
        BinaryExpAST(BaseAST* a, BaseAST*b, Token* token);
        ~BinaryExpAST();
        virtual void generator();
        virtual void setVal(int a){val = a;}
        virtual bool isConst(){return isConst_;}
        virtual bool isNumber();
        virtual int getVal(){return val;}
        virtual int cal();
};

class UnaryExpAST : public BaseAST{
    public:
        BaseAST* firstExp;
        int op;
        int val;
        bool isConst_;
        bool isCaled_;

        UnaryExpAST(){type=UnaryExp;}
        UnaryExpAST(BaseAST* a, Token* token);
        ~UnaryExpAST();
        void dealOp(Token* token);
        virtual void generator();
        virtual void setVal(int a){val = a;}
        virtual bool isConst(){return isConst_;}
        virtual bool isNumber();
        virtual int getVal(){return val;}
        virtual int cal();
};

class LvalAST : public BaseAST{
    public:
        BaseEntry* sym;
        vector<BaseAST*> indexs;
        int val;
        bool isConst_;
        bool isCaled_;
	    bool isLeft_;

        LvalAST(){type=Lval;}
        LvalAST(Token* token, BaseAST* ast);
        ~LvalAST(){
            for(auto i:indexs){
                if(i) delete i;
            }
        }

        bool checkTooManyIndex();
        bool checkNegIndex();
        bool checkIfOutOfBound();

        virtual void generator();
        virtual void setVal(int a){val = a;}
        virtual bool isConst(){return isConst_;}
        virtual bool isNumber();
        virtual int getVal(){return val;}
        virtual int cal();
};

class arrAST: public BaseAST{
    public:
        arrAST(){type=ArrAST;}
        ~arrAST(){};
        vector<BaseAST*> exps;
        void insertArrAst(BaseAST* ast){ exps.push_back(ast); }
        vector<BaseAST*> output(){ return move(exps); }
        vector<int> calExps(){
            vector<int> ans;
            for(auto ptr :exps){
                ans.push_back(ptr->getVal());
            }
            for(auto ptr :exps){
                if(ptr) delete ptr;     
            }
            exps.clear();
            return ans;
        }
};

class ListTree: public BaseAST{
    public:
        BaseAST* astptr; // 如果数组声明含有表达式，则指向这个exp
        
        ListTree():BaseAST(),astptr(nullptr) { type=ListTree_; } 
        ListTree(Token* l, Token* r):BaseAST(), astptr(nullptr) { type=ListTree_; }// '{' '}', 空
        ListTree(BaseAST* astptr):BaseAST(), astptr(astptr) { type=ListTree_; } // 叶子节点

        ~ListTree() {
            if(!astptr) {
                for(auto p: sublist) delete p;
                sublist.clear();
            }
	    }

        vector<BaseAST* > sublist; // 高维的大括号指向低维的大括号
        void insertSublist(BaseAST* p){ sublist.push_back(p); } 
        map<int, BaseAST* > Flatten(const vector<int> &dims); 
        void _Flatten_(vector<int> &dim, map<int, BaseAST* > &res, int ind, int upper); 

};

#endif