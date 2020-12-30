#ifndef TREE_H
#define TREE_H
#define MAXChildNum 4
#include "symbol.h"
#include <iostream>
#include <string>
using namespace std;

extern symbol_table SymbolsTable;

enum NodeType {
    NODE_PROG,
    NODE_STMT,
    NODE_TYPE,
    NODE_CONST,
    NODE_VAR,
    NODE_BOOL,
    NODE_EXPR,
    NODE_OP,
    NODE_NULL
};

enum StmtType {
    STMT_IF,
    STMT_WHILE,
    STMT_DECL,
    STMT_ASSIGN,
    STMT_PRINTF,
    STMT_SCANF,

    STMT_FOR,
    STMT_FORCONDITION,
    STMT_RETURN,
    STMT_BREAK,
    STMT_COMPOUND

};

enum OpType {
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

enum VarType {
    VAR_INTEGER,
    VAR_VOID,
    VAR_BOOL,
    VAR_CHAR,
    VAR_STRING,

    Notype
};

struct Label {
    string true_label;
    string false_label;
    string begin_label;
    string next_label;
};

struct TreeNode {
    int nodeID; //节点序号
    int line;   // 行号
    int childNum;
    NodeType nodeType; // 节点类型

    TreeNode *child[MAXChildNum];
    TreeNode *sibling = nullptr;

    void addChild(TreeNode *);
    void addSibling(TreeNode *);

    void genNodeId(); //从根节点开始逐个赋Id 实现方式同学们可以自行修改

    void printAST(); //打印语法树结点
    void printNodeInfo();
    void printNodeConnection();
    string nodeTypeInfo();

    int int_val;   // 节点为常量时，相对应的类型存储其值
    bool bool_val; // 布尔常量
    string str_val;
    string char_val;

    StmtType stmtType; // 节点为其他类型时，存储其内容
    OpType opType;     // 节点为表达式节点
    VarType varType;

    VarType type;
    string var_name;

    TreeNode(NodeType type, int lineno);

    string NodeTypeTostring(NodeType type);
    string StmtTypeTostring(StmtType type);
    string opTypeTostring(OpType type);
    string VarTypeTostring(VarType type);

    int temp_var = 0;
    Label label;
    int const_str = 0;

    void type_check();
};

class tree {
  public:
    TreeNode *root;
    int node_seq = 0;
    int temp_var_seq = 0;
    int label_seq = 0;
    int const_str_seq = 0;

    void get_temp_var(TreeNode *t);
    void get_const_str(TreeNode *t);
    string new_label(void);
    void recursive_get_label(TreeNode *t);
    void stmt_get_label(TreeNode *t);
    void expr_get_label(TreeNode *t);

    void gen_header(ostream &out);
    void gen_decl(ostream &out, TreeNode *t);
    void recursive_gen_code(ostream &out, TreeNode *t);
    void stmt_gen_code(ostream &out, TreeNode *t);
    void expr_gen_code(ostream &out, TreeNode *t);

    void get_label(void);
    void gen_code(ostream &out);
    tree(TreeNode *node) {
        root = node;
        temp_var_seq = root->temp_var;
        const_str_seq = root->const_str;
    }
};
#endif