%option noyywrap
%{
    #include"common.h"
    #include"main.tab.hh"
    int lineno=0;
    extern vector<IDAttributes> SymbolsTable;
    // int flag;
    bool for_flag=false;
    stack<int> flag;
%}

INTEGER (0|[1-9][0-9]*)
ID [[:alpha:]_][[:alpha:][:digit:]_]*

EOL (\r\n|\n)
WHITE [\t ]

BlockComm (\/\/.*\n)|(\/\*(\n|.)*?\*\/)
LineComm (\/\/.*\n?)

ConstStr (\".*\")
ConstChar (\'.?\')

%%

{EOL} {++lineno;}

{BlockComm} {
        string str=string(yytext);
        int index=0; 
    	while( (index=str.find("\n",index)) < str.length() ){
		++lineno;
		index++;
	}
}
{LineComm} {++lineno;}

"true" {
    TreeNode *node = new TreeNode(NODE_BOOL, lineno);
    node->bool_val = true;
    node->str_val = "true";
    yylval = node;
    return TRUE;
}
"false" {
    TreeNode *node = new TreeNode(NODE_BOOL, lineno);
    node->bool_val = false;
    node->str_val = "false";
    yylval = node;
    return FALSE;
}

"int" return INT;
"void" return VOID;
"char" return CHAR;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
"string" return STRING;
"bool" return BOOL;
"const" return CONST;

"if" return IF;
"while" return WHILE;
"else" return ELSE;
"for" return FOR;
"switch" return SWITCH;
"case" return CASE;

"main" return MAIN;
"continus" return CONTINUS;
"break" return BREAK;

"return" return RETURN;

"printf" return PRINTF;
"scanf" return SCANF;

"=" return ASSIGN;
"+=" return AASSIGN;
"-=" return SASSIGN;
"*=" return MASSIGN;

"+" return ADD;
"-" return SUB;
"*" return MUL;
"/" return DIV;
"%" return MOD;
"++" return INC;
"--" return DEC;

"==" return EQUAL;
"!=" return NOTEQUAL;
">" return GT;
">=" return GTE;
"<" return LT;
"<=" return LTE;
"!" return NOT;

"&&" return LAND;
"||" return LOR;

"&" return ADDRO;

"(" return LPAREN;
")" return RPAREN;
"[" return LBRACK;
"]" return RBRACK;
"{" {
    if(for_flag){
        // flag = SymbolsTable.size()-1;
        flag.push(SymbolsTable.size()-1);
        for_flag=false;
    }
    else{
        // flag = SymbolsTable.size();
        flag.push(SymbolsTable.size());
    }
    return LBRACE;
}
"}" {
    int t = SymbolsTable.size()-flag.top();
    for(int i=0;i<t;i++){
        SymbolsTable.pop_back();
    }
    flag.pop();
    return RBRACE;
}
";" return SEMICOLON;
"," return COMMA;
"." return DOT;
":" return COLON;
"->" return POT;


{INTEGER} {
    TreeNode *node = new TreeNode(NODE_CONST, lineno);
    node->int_val = atoi(yytext);
    node->str_val = string(yytext);
    node->var_name = "Interger";
    yylval = node;
    return INTEGER;
}
{ID} {
    TreeNode *node = new TreeNode(NODE_VAR, lineno);
    node->var_name = string(yytext);
    bool f=false;
    IDAttributes id;
    for(int i=SymbolsTable.size()-1;i>=0;i--){
        if(node->var_name==SymbolsTable[i].var_name){
           f=true;
           id=SymbolsTable[i];
           break;
        }
    }
    if(f){
         node->varType=id.var_type;
    }else{
        //error未定义变量
    }
    yylval = node;
    return ID;
}
{ConstStr} {
    TreeNode *node = new TreeNode(NODE_CONST, lineno);
    node->str_val = string(yytext);
    node->var_name = "ConstStr";
    yylval = node;
    return ConstStr;
}
{ConstChar} {
    TreeNode *node = new TreeNode(NODE_CONST, lineno);
    string s = string(yytext);
    node->char_val = s.at(1);
    node->str_val = s.substr(1,1);
    node->var_name = "ConstChar";
    yylval = node;
    return ConstChar;
}

{WHITE}

%%