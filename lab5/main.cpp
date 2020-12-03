#include"main.tab.hh"
#include"common.h"
#include<iostream>
using std::cout;
using std::endl;
TreeNode *root=nullptr;
vector<IDAttributes> SymbolsTable; 
int main ()
{
    yyparse();
    if(root){//若存在语法树结点
        root->genNodeId();//将整棵语法树赋予id
        root->printAST();//打印相关信息
    }
    for(int i=0;i<SymbolsTable.size();i++){
      //printf("%ld\n", SymbolsTable.size());
      printf("%s   %s\n", SymbolsTable[i].var_name.c_str(), root->VarTypeTostring(SymbolsTable[i].var_type).c_str());
    }
}
int yyerror(char const* message)
{
  cout << message << endl;
  return -1;
}