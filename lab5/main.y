%{
    #include"common.h"
    extern TreeNode * root;
    int yylex();
    int yyerror( char const * );
    extern int lineno;
    extern vector<IDAttributes> SymbolsTable;
    extern int flag;
    extern bool for_flag;
%}
%defines

%start program

%token ID INTEGER ConstStr ConstChar
%token IF ELSE WHILE FOR SWITCH CASE MAIN CONTINUS BREAK RETURN 
%token INT VOID CHAR STRING BOOL CONST
%token LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE SEMICOLON COMMA DOT COLON POT
%token TRUE FALSE
%token ASSIGN AASSIGN SASSIGN MASSIGN
%token ADD SUB MUL DIV MOD
%token EQUAL NOTEQUAL GT GTE LT LTE
%token LAND LOR NOT
%token PRINTF SCANF

%left COMMA
%right ASSIGN AASSIGN SASSIGN MASSIGN
%left LAND LOR
%left EQUAL NOTEQUAL
%left GT GTE LT LTE
%left ADD SUB
%left MUL DIV MOD
%right NOT UMINUS INC DEC ADDRO SADD
%nonassoc LOWER_THEN_ELSE
%nonassoc ELSE 
%%
program
    : statements {root=new TreeNode(NODE_PROG, $1->line);root->addChild($1);}
    ;
statements
    : statement {$$=$1;}
    | statements statement{$$=$1;$$->addSibling($2);}
    ;
statement
    : declare {$$=$1;}
    | instruction {$$=$1;}
    | if_else {$$=$1;}
    | while {$$=$1;}
    | for {$$=$1;}
    | LBRACE statements RBRACE {$$=$2;}
    | type MAIN LPAREN RPAREN statement {$$=$5;}
    ;
if_else
    : IF bool_statment statement %prec LOWER_THEN_ELSE {
        TreeNode *node=new TreeNode(NODE_STMT, $2->line);
        node->stmtType=STMT_IF;
        node->addChild($2);
        node->addChild($3);
        $$=node;
    }
    | IF bool_statment statement ELSE statement {
        TreeNode *node=new TreeNode(NODE_STMT, $2->line);
        node->stmtType=STMT_IF;
        node->addChild($2);
        node->addChild($3);
        node->addChild($5);
        $$=node;
    }
    ;
while
    : WHILE bool_statment statement {
        TreeNode *node=new TreeNode(NODE_STMT, $2->line);
        node->stmtType=STMT_WHILE;
        node->addChild($2);
        node->addChild($3);
        $$=node;
    }
    ;
for
    : FOR LPAREN for_statment RPAREN statement {
        TreeNode *node=new TreeNode(NODE_STMT, $3->line);
        node->stmtType=STMT_FOR;
        node->addChild($3);
        node->addChild($5);
        $$=node;
    }
    ;
for_statment
    : declare bool_expr SEMICOLON expr {
        TreeNode *node=new TreeNode(NODE_STMT, lineno);
        node->stmtType=STMT_FORCONDITION;
        node->addChild($1);
        node->addChild($2);
        node->addChild($4);
        for_flag=true;
        $$=node;
    }
    | expr SEMICOLON bool_expr SEMICOLON expr{
        TreeNode *node=new TreeNode(NODE_STMT, lineno);
        node->stmtType=STMT_FORCONDITION;
        node->addChild($1);
        node->addChild($3);
        node->addChild($5);
        $$=node;
    }
    ;
bool_statment
    : LPAREN bool_expr RPAREN {$$=$2;}
    ;
declare
    : type id_list SEMICOLON {
        TreeNode *node=new TreeNode(NODE_STMT, lineno);
        node->stmtType=STMT_DECL;

        TreeNode *t=$2;
        while(t->sibling){
            IDAttributes id;
            bool repeat=false;
            if(t->nodeType==NODE_OP){// 声明变量且带有赋值表达式
                if(flag==SymbolsTable.size()){// 此作用域还没有任何变量
                    id.var_name=t->child[0]->var_name;
                    id.var_type=t->child[0]->varType;
                    SymbolsTable.push_back(id);
                }else{
                    for(int i=flag;i<SymbolsTable.size();i++){// 作用域存在变量，遍历符号表，查找是否有同名变量
                        if(SymbolsTable[i].var_name==t->child[0]->var_name){
                            repeat=true;
                            break;
                        }
                    }
                    if(!repeat){// 没有重名变量，加入符号表
                        id.var_name=t->child[0]->var_name;
                        id.var_type=t->child[0]->varType;
                        SymbolsTable.push_back(id);
                    }else{
                        // error
                    }
                }
            }else{// 声明变量不带有赋值表达式
                if(flag==SymbolsTable.size()){
                    id.var_name=t->var_name;
                    id.var_type=t->varType;
                    SymbolsTable.push_back(id);
                }else{
                    for(int i=flag;i<SymbolsTable.size();i++){
                        if(SymbolsTable[i].var_name==t->var_name){
                            repeat=true;
                            break;
                        }
                    }
                    if(!repeat){
                        id.var_name=t->var_name;
                        id.var_type=t->varType;
                        SymbolsTable.push_back(id);
                    }else{
                        // error
                    }
                }
                
            }
            t=t->sibling;
        }

        node->addChild($1);
        node->addChild($2);
        $$=node;
    }
    ;
instruction
    : expr SEMICOLON {
        $$=$1;  
    }
    | printf SEMICOLON {$$=$1;}
    | scanf SEMICOLON {$$=$1;}
    ;
id_list
    : id_list COMMA expr {
        $$=$1;
        $$->addSibling($3);
    }
    | expr {$$=$1;}
    ;
printf
    : PRINTF LPAREN expr RPAREN {
        TreeNode *node=new TreeNode(NODE_STMT, lineno);
        node->stmtType=STMT_PRINTF;
        node->addChild($3);
        $$=node;
    }
    ;
scanf
    : SCANF LPAREN expr RPAREN {
        TreeNode *node=new TreeNode(NODE_STMT, lineno);
        node->stmtType=STMT_SCANF;
        node->addChild($3);
        $$=node;
    }
    ;
bool_expr
    : TRUE {$$=$1;}
    | FALSE {$$=$1;}
    | expr EQUAL expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_EQUAL;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | expr GT expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_GT;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | expr GTE expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_GTE;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | expr LT expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_LT;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | expr LTE expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_LTE;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | expr NOTEQUAL expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_NOTEQUAL;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | NOT bool_expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_NOT;
        node->addChild($2);
        $$=node;        
    }
    | bool_expr LAND bool_expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_LAND;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | bool_expr LOR bool_expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_LOR;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    ;
expr
    : ID {$$=$1;}
    | INTEGER {$$=$1;}
    | ConstStr {$$=$1;}
    | ConstChar {$$=$1;}
    | ConstStr COMMA id_list {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_COMMA;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | expr ADD expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_ADD;
        node->addChild($1);
        node->addChild($3);
        $$=node;   
    }
    | expr SUB expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_SUB;
        node->addChild($1);
        node->addChild($3);
        $$=node;   
    }
    | expr MUL expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_MUL;
        node->addChild($1);
        node->addChild($3);
        $$=node;   
    }
    | expr DIV expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_DIV;
        node->addChild($1);
        node->addChild($3);
        $$=node;   
    }
    | expr MOD expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_MOD;
        node->addChild($1);
        node->addChild($3);
        $$=node;   
    }
    | ADD expr %prec SADD {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_SADD;
        node->addChild($2);
        $$=node;
    }
    | SUB expr %prec UMINUS {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_UMINUS;
        node->addChild($2);
        $$=node;
    }
    | ADDRO ID {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_ADDRO;
        node->addChild($2);
        $$=node;
    }
    | ID INC {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_INC;
        node->addChild($1);
        $$=node;
    }
    | ID DEC {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_DEC;
        node->addChild($1);
        $$=node;
    }
    | ID ASSIGN expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_ASSIGN;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | ID AASSIGN expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_AASSIGN;
        node->addChild($1);
        node->addChild($3);
        $$=node;  
    }
    | ID MASSIGN expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_MASSIGN;
        node->addChild($1);
        node->addChild($3);
        $$=node;  
    }
    | ID SASSIGN expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_SASSIGN;
        node->addChild($1);
        node->addChild($3);
        $$=node;  
    }
    ;
assign_expr
    : ID ASSIGN expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_ASSIGN;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    | ID AASSIGN expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_AASSIGN;
        node->addChild($1);
        node->addChild($3);
        $$=node;  
    }
    | ID MASSIGN expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_MASSIGN;
        node->addChild($1);
        node->addChild($3);
        $$=node;  
    }
    | ID SASSIGN expr {
        TreeNode *node=new TreeNode(NODE_OP, lineno);
        node->opType=OP_SASSIGN;
        node->addChild($1);
        node->addChild($3);
        $$=node;
    }
    ;
type
    : INT {
        TreeNode *node=new TreeNode(NODE_TYPE, lineno);
        node->varType=VAR_INTEGER;
        $$=node; 
    }
    | VOID {
        TreeNode *node=new TreeNode(NODE_TYPE, lineno);
        node->varType=VAR_VOID;
        $$=node;         
    }
    | CHAR {
        TreeNode *node=new TreeNode(NODE_TYPE, lineno);
        node->varType=VAR_CHAR;
        $$=node;         
    }
    ;

%%