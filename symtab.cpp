#include "symtab.hpp"

symTab* const globalSymTab = new symTab(blockNum);
static symTab* currentSymTab = globalSymTab;

void varSymEntry::setDims(vector<int> && arr) {dims = arr;}
void funSymEntry::insertParams(BaseEntry* ptr) { fParams.push_back(ptr); }

BaseEntry* newVarEntry(bool isConst, Token* token, BaseAST* ast){
    BaseEntry* newEntry = new varSymEntry(isConst, token, ast);

    BaseEntry* temp = checkIfDecl(newEntry);
    if(temp){
        if(Debug) printf("Error in function 'symtab.cpp->newVarEntry', statement conflict!\n");
        if(Debug) printf("id = %s\n", token->strPtr->c_str());
        delete newEntry;
        return nullptr;
    }

    return newEntry;
}

BaseEntry* newFunEntry(bool isVoid, Token* token, BaseAST* ast){
    BaseEntry* newEntry = new funSymEntry(isVoid, token, ast);

    BaseEntry* temp = checkIfDecl(newEntry);
    if(temp){
        if(Debug) printf("Error in function 'symtab.cpp->newFunEntry', statement conflict!\n");
        if(Debug) printf("id = %s\n", token->strPtr->c_str());
        delete newEntry;
        return nullptr;
    }

    return newEntry;
}

void pushSymtab(){
    blockNum++;
    symTab* tmp = new symTab(blockNum, currentSymTab);
    currentSymTab->children.push_back(tmp);
    currentSymTab = tmp;
}

void popSymtab(){
    blockNum--;
    currentSymTab = currentSymTab->father;
    if(!currentSymTab){
        if(Debug) printf("Error in function 'symtab.cpp->popSymtab', no father symTab!\n");
    }
}

void insertEntry(BaseEntry* entry){
    if(entry) currentSymTab->table.insert(entry);
    if(Debug){
        printf("insert entry: %s\n",entry->strp->c_str());
    }
}

BaseEntry* findString(strIt strp){
    BaseEntry* tmp = new BaseEntry(strp, nullptr);
    for(auto i=currentSymTab; i; i = i->father){
        i->printSymtab();
        if( i==nullptr ){
            if(Debug) printf("Error in findString!\n");
            return nullptr;
        }
        auto j = i->table.find(tmp);
        if( j != i->table.end() ) {
            delete tmp;
            return *j;
        }
    }
    delete tmp;
    return nullptr;
}

BaseEntry* checkIfDecl(BaseEntry* entry){
    auto j = currentSymTab->table.find(entry);
    if( j!=currentSymTab->table.end() )
        return *j;
    return nullptr;
}

hash<string> hashstr;