#ifndef TREE_H
#define TREE_H
#define MAXChildNum 5
#include<iostream>
#include<string>

using namespace std;

enum NodeType{
    NODE_PROG,
    NODE_STMT,
    NODE_TYPE,
    NODE_CONST,
    NODE_VAR,
    NODE_BOOL,
    NODE_EXPR, 
    NODE_OP
};

enum StmtType{
    STMT_IF,
    STMT_WHILE,
    STMT_DECL,
    STMT_ASSIGN,
    STMT_PRINTF,
    STMT_SCANF,

    STMT_FOR,
    STMT_FORCONDITION

};

enum OpType{
    OP_EQUAL,
    OP_GT,
    OP_GTE,
    OP_LT,
    OP_LTE,
    OP_NOTEQUAL,
    
    OP_LAND,
    OP_LOR,
    OP_NOT,


    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_INC,
    OP_DEC,
    OP_UMINUS,
    OP_SADD,

    OP_ASSIGN,
    OP_AASSIGN,
    OP_SASSIGN,
    OP_MASSIGN,
    
    OP_LPAREN,
    OP_RPAREN,
    OP_LBRACK,
    OP_RBRACK,
    OP_LBRACE,
    OP_RBRACE,
    OP_COMMA,
    OP_DOT,
    OP_COLON,
    OP_SEMICOLON,
    OP_DQUOTATION,
    OP_SQUOTATION,
    OP_POT,

    OP_ADDRO
};

enum VarType{
    VAR_INTEGER,
    VAR_VOID,

    VAR_CHAR,
    VAR_STRING
};


struct TreeNode {
    int nodeID;
    int line;
    int childNum;
    NodeType nodeType;

    TreeNode *child[MAXChildNum];
    TreeNode *sibling = nullptr;

    void addChild(TreeNode *);
    void addSibling(TreeNode *);

    void genNodeId();//从根节点开始逐个赋Id 实现方式同学们可以自行修改

    void printAST();//打印语法树结点
    /***
     * 以下的几个函数皆为在printAST过程中辅助输出使用
     * 同学们可以根据需要自己使用其他方法
    ***/
    void printNodeInfo();
    void printNodeConnection();
    string nodeTypeInfo();

    int int_val;
    bool bool_val;
    string str_val;
    char char_val;
    StmtType stmtType;
    OpType opType;
    VarType varType;
    string var_name;

    TreeNode(NodeType type, int lineno);

    string NodeTypeTostring(NodeType type);
    string StmtTypeTostring(StmtType type);
    string opTypeTostring(OpType type);
    string VarTypeTostring(VarType type);
};
#endif