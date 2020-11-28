%option noyywrap
%{
    #include"common.h"
    #include"main.tab.hh"
    int lineno=0;
%}

INTEGER [0-9]+
ID [[:alpha:]_][[:alpha:][:digit:]_]*

EOL (\r\n|\n)
WHITE [\t ]

%%

{EOL} {lineno++;}

"true" {
    TreeNode *node = new TreeNode(NODE_BOOL, lineno);
    node->bool_val = true;
    yylval = node;
    return TRUE;
}
"false" {
    TreeNode *node = new TreeNode(NODE_BOOL, lineno);
    node->bool_val = false;
    yylval = node;
    return FALSE;
}

"int" return INT;
"void" return VOID;

"if" return IF;
"while" return WHILE;
"else" return ELSE;

"printf" return PRINTF;
"scanf" return SCANF;

"=" return ASSIGN;

"+" return ADD;

"!" return NOT;
"==" return EQUAL;

";" return SEMICOLON;
"(" return LPAREN;
")" return RPAREN;
"{" return LBRACE;
"}" return RBRACE;

{INTEGER} {
    TreeNode *node = new TreeNode(NODE_CONST, lineno);
    node->int_val = atoi(yytext);
    yylval = node;
    return INTEGER;
}
{ID} {
    TreeNode *node = new TreeNode(NODE_VAR, lineno);
    node->var_name = string(yytext);
    yylval = node;
    return ID;
}

{WHITE}

%%