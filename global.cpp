#include "global.hpp"

int blockNum = 0;
int lineno = 1;

int labelNum = 0; 
int paraNum = 0; 
int tempVarNum = 0; 
int nativeVarNum = 0; 


list<BaseAST*> whileStack;
BaseAST* currentFun;

BaseAST* mainptr = nullptr;

list<string> strtab; 



vector<int> getArrWidth(const vector<int> &arrDef){
   vector<int> tmp (1,1);
   if(arrDef.size()==0) return tmp;
   int k = 1;
   for( auto i = arrDef.rbegin(); i!= arrDef.rend(); ++i){
      k *= *i;
      tmp.push_back(k);
   }
   return tmp;
}

int getArrAddr(const vector<int> &width, const vector<int> &indexs){
   int ans = 0;
   int widthLen = width.size();
   int indexsLen = indexs.size();
   if(widthLen==1) return ans;
   for(int i = 0; i<indexsLen; ++i){
      ans += (indexs[i]*width[widthLen-2-i]);
   }
   return ans;
}

string num2string(int num){
    string ans = to_string(num);
    return ans;
}

void stringListPushBack(list<string> &List_, const string &ss){
    List_.push_back(ss);
}