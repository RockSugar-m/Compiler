#include "tree.h"

bool decl_flag = false;
int NodeNum = 0;
string str_table[10];
int temp_var_all = 0;
int const_str_all = 0;
int m = 0;

TreeNode::TreeNode(NodeType nodeType, int lineno) {
    this->nodeType = nodeType;
    line = lineno;
    childNum = 0;
    type = Notype;
    for(int i = 0; i < MAXChildNum; i++) {
        child[i] = nullptr;
    }
    label.begin_label = label.false_label = label.next_label =
        label.true_label = "";
}

void TreeNode::addChild(TreeNode *t) {
    if(childNum < MAXChildNum) {
        child[childNum] = t;
        childNum++;
    }
}

void TreeNode::addSibling(TreeNode *t) {
    if(!sibling) {
        sibling = t;
    } else {
        TreeNode *temp = sibling;
        while(temp->sibling) {
            temp = temp->sibling;
        }
        temp->sibling = t;
    }
}

void TreeNode::genNodeId() {
    nodeID = NodeNum++;
    int i = 0;
    while(child[i]) {
        child[i]->genNodeId();
        i++;
    }
    if(sibling)
        sibling->genNodeId();
}

void TreeNode::printNodeInfo() {
    printf("@%-3d lineno:%-3d ", nodeID, line);
    switch(nodeType) {
    case NODE_PROG:
        printf("Program ");
        break;
    case NODE_STMT:
        printf("StmtType:%-17s ", StmtTypeTostring(stmtType).c_str());
        break;
    case NODE_TYPE:
        printf("TYPE:%-11s ", VarTypeTostring(varType).c_str());
        break;
    case NODE_CONST:
        printf("CONST:%-10s value:%-5s ", var_name.c_str(), str_val.c_str());
        break;
    case NODE_VAR:
        printf("VarType:%-5s VarName:%-10s ", VarTypeTostring(varType).c_str(),
               var_name.c_str());
        break;
    case NODE_BOOL:
        printf("BOOL   value:%-5s", str_val.c_str());
        break;
    case NODE_EXPR:
        printf("Expression:%-3s ", opTypeTostring(opType).c_str());
        break;
    case NODE_OP:
        printf("Operator");
        break;
    case NODE_NULL:
        printf("null_node");
        break;
    default:
        break;
    }
}

void TreeNode::printAST() {
    printNodeInfo();
    printNodeConnection();
    printf("\n");
    if(nodeType == NODE_EXPR) {
        if(opType == OP_ASSIGN) {
            temp_var_all++;
            temp_var = temp_var_all - 1;
        } else {
            if(child[0]->nodeType == NODE_EXPR) {
                temp_var_all++;
                child[0]->temp_var = temp_var_all - 1;
            }
            if(child[1] != NULL && child[1]->nodeType == NODE_EXPR) {
                temp_var_all++;
                child[0]->temp_var = temp_var_all - 1;
            }
        }
    }
    if(nodeType == NODE_CONST && (type == VAR_CHAR || type == VAR_STRING)) {
        str_table[const_str_all] = str_val;
        const_str_all++;
        const_str = const_str_all - 1;
    }
    if(nodeType == NODE_STMT && stmtType == STMT_DECL)
        decl_flag = true;
    int i = 0;
    while(child[i]) {
        child[i]->printAST();
        i++;
    }
    type_check();
    if(sibling)
        sibling->printAST();
}

void TreeNode::printNodeConnection() {
    if(childNum == 0)
        return;
    printf("children: ");
    for(int i = 0; i < childNum; i++) {
        printf("@%d ", child[i]->nodeID);
        int j = 0;
        TreeNode *tnode = child[i]->sibling;
        while(tnode) {
            printf("@%d ", tnode->nodeID);
            tnode = tnode->sibling;
        }
    }
}

void TreeNode::type_check() {
    int i = 0;
    TreeNode *t = child[1];
    switch(nodeType) {
    case NODE_STMT:
        switch(stmtType) {
        case STMT_IF:
            if(child[0]->type != VAR_BOOL) {
                cerr << "Bad boolean type at line: " << line << endl;
            }
            break;
        case STMT_WHILE:
            if(child[0]->type != VAR_BOOL) {
                cerr << "Bad boolean type at line: " << line << endl;
            }
            break;
        case STMT_FORCONDITION:
            if(child[1]->type != VAR_BOOL && child[1]->nodeType != NODE_NULL) {
                cerr << "Bad boolean type at line: " << line << endl;
            }
            break;
        case STMT_DECL:
            while(t) {
                int j = 0;
                if(t->nodeType == NODE_EXPR) { // 声明变量且带有赋值表达式
                    if((j = SymbolsTable.lookup(t->child[0]->var_name)) != -1) {
                        if(!SymbolsTable.table[j].defined)
                            SymbolsTable.table[j].defined = true;
                        else
                            cerr << "variable repeat define at line: " << line
                                 << endl;
                    }
                } else { // 声明变量不带有赋值表达式
                    if((j = SymbolsTable.lookup(t->var_name)) != -1) {
                        if(!SymbolsTable.table[j].defined)
                            SymbolsTable.table[j].defined = true;
                        else
                            cerr << "variable repeat define at line: " << line
                                 << endl;
                    }
                }
                t = t->sibling;
            }
            decl_flag = false;
            break;
        default:
            break;
        }
        break;
    case NODE_EXPR:
        switch(opType) {
        case OP_ADD:
            if(child[0]->type != child[1]->type)
                cerr << "add factor error at line: " << line << endl;
            type = child[0]->type;
            break;
        case OP_SUB:
            if(child[0]->type != child[1]->type ||
               child[0]->type != VAR_INTEGER)
                cerr << "sub factor error at line: " << line << endl;
            type = child[0]->type;
            break;
        case OP_MUL:
            if(child[0]->type != child[1]->type ||
               child[0]->type != VAR_INTEGER)
                cerr << "mul factor error at line: " << line << endl;
            type = child[0]->type;
            break;
        case OP_DIV:
            if(child[0]->type != child[1]->type ||
               child[0]->type != VAR_INTEGER)
                cerr << "DIV factor error at line: " << line << endl;
            type = child[0]->type;
            break;
        case OP_MOD:
            if(child[0]->type != child[1]->type ||
               child[0]->type != VAR_INTEGER)
                cerr << "mod factor error at line: " << line << endl;
            type = child[0]->type;
            break;
        case OP_SADD:
            if(child[0]->type != VAR_INTEGER)
                cerr << "single add error at line: " << line << endl;
            type = child[0]->type;
            break;
        case OP_UMINUS:
            if(child[0]->type != VAR_INTEGER)
                cerr << "uminus factor at line: " << line << endl;
            type = child[0]->type;
            break;
        case OP_ADDRO:
            type = child[0]->type;
            break;
        case OP_INC:
            if(child[0]->type != VAR_INTEGER)
                cerr << "increase error at line: " << line << endl;
            type = child[0]->type;
            break;
        case OP_DEC:
            if(child[0]->type != VAR_INTEGER)
                cerr << "decrease error at line: " << line << endl;
            type = child[0]->type;
            break;
        case OP_ASSIGN:
            if(child[0]->type != child[1]->type)
                cerr << "assign factor error at line: " << line << endl;
            type = child[0]->type;
            break;
        case OP_AASSIGN:
            if(child[0]->type != child[1]->type ||
               child[0]->type != VAR_INTEGER)
                cerr << "add assign factor error at line: " << line << endl;
            type = child[0]->type;
            break;
        case OP_MASSIGN:
            if(child[0]->type != child[1]->type ||
               child[0]->type != VAR_INTEGER)
                cerr << "mul assign factor error at line: " << line << endl;
            type = child[0]->type;
            break;
        case OP_SASSIGN:
            if(child[0]->type != child[1]->type ||
               child[0]->type != VAR_INTEGER)
                cerr << "sub assign factor error at line: " << line << endl;
            type = child[0]->type;
            break;
        case OP_EQUAL:
            if(child[0]->type != child[1]->type)
                cerr << "equal factor error at line: " << line << endl;
            type = VAR_BOOL;
            break;
        case OP_GT:
            if(child[0]->type != child[1]->type ||
               child[0]->type != VAR_INTEGER)
                cerr << "> factor error at line: " << line << endl;
            type = VAR_BOOL;
            break;
        case OP_GTE:
            if(child[0]->type != child[1]->type ||
               child[0]->type != VAR_INTEGER)
                cerr << ">= factor error at line: " << line << endl;
            type = VAR_BOOL;
            break;
        case OP_LT:
            if(child[0]->type != child[1]->type ||
               child[0]->type != VAR_INTEGER)
                cerr << "< factor error at line: " << line << endl;
            type = VAR_BOOL;
            break;
        case OP_LTE:
            if(child[0]->type != child[1]->type ||
               child[0]->type != VAR_INTEGER)
                cerr << "<= factor error at line: " << line << endl;
            type = VAR_BOOL;
            break;
        case OP_NOTEQUAL:
            if(child[0]->type != child[1]->type)
                cerr << "!= factor error at line: " << line << endl;
            type = VAR_BOOL;
            break;
        case OP_NOT:
            if(child[0]->type != VAR_BOOL)
                cerr << "! factor error at line: " << line << endl;
            type = VAR_BOOL;
            break;
        case OP_LAND:
            if(child[0]->type != VAR_BOOL)
                cerr << "&& factor error at line: " << line << endl;
            type = VAR_BOOL;
            break;
        case OP_LOR:
            if(child[0]->type != VAR_BOOL)
                cerr << "|| factor error at line: " << line << endl;
            type = VAR_BOOL;
            break;
        default:
            break;
        }
        break;
    case NODE_VAR:
        i = SymbolsTable.lookup(var_name);
        if(decl_flag) {
            type = (VarType)SymbolsTable.table[i].type;
        } else {
            if(i != -1 && SymbolsTable.table[i].defined)
                type = (VarType)SymbolsTable.table[i].type;
            else
                cerr << "variable not define at line: " << line << endl;
        }
        break;
    default:
        break;
    }
}

string TreeNode::NodeTypeTostring(NodeType type) {
    switch(type) {
    case NODE_CONST:
        return "NODE_CONST";
        break;
    case NODE_BOOL:
        return "NODE_BOOL";
        break;
    case NODE_VAR:
        return "NODE_VAR";
        break;
    case NODE_EXPR:
        return "NODE_EXPR";
        break;
    case NODE_TYPE:
        return "NODE_TYPE";
        break;
    case NODE_STMT:
        return "NODE_STMT";
        break;
    case NODE_PROG:
        return "NODE_PROG";
        break;
    case NODE_OP:
        return "NODE_OP";
        break;
    case NODE_NULL:
        return "NODE_NULL";
        break;
    default:
        return "";
        break;
    }
}
string TreeNode::StmtTypeTostring(StmtType type) {
    switch(type) {
    case STMT_IF:
        return "if";
        break;
    case STMT_WHILE:
        return "while";
        break;
    case STMT_DECL:
        return "declare";
        break;
    case STMT_ASSIGN:
        return "assign";
        break;
    case STMT_PRINTF:
        return "print";
        break;
    case STMT_SCANF:
        return "scanf";
        break;
    case STMT_FOR:
        return "for";
        break;
    case STMT_FORCONDITION:
        return "for_condition";
        break;
    case STMT_RETURN:
        return "return";
        break;
    case STMT_BREAK:
        return "break";
        break;
    case STMT_COMPOUND:
        return "compound_stmt";
        break;
    default:
        return "";
        break;
    }
}
string TreeNode::opTypeTostring(OpType type) {
    switch(type) {
    case OP_EQUAL:
        return "==";
        break;
    case OP_GT:
        return ">";
        break;
    case OP_GTE:
        return ">=";
        break;
    case OP_LT:
        return "<";
        break;
    case OP_LTE:
        return "<=";
        break;
    case OP_NOTEQUAL:
        return "!=";
        break;
    case OP_LAND:
        return "&&";
        break;
    case OP_LOR:
        return "||";
        break;
    case OP_NOT:
        return "!";
        break;
    case OP_ADD:
        return "+";
        break;
    case OP_SADD:
        return "+";
        break;
    case OP_SUB:
        return "-";
        break;
    case OP_UMINUS:
        return "-";
        break;
    case OP_MUL:
        return "*";
        break;
    case OP_DIV:
        return "/";
        break;
    case OP_MOD:
        return "%";
        break;
    case OP_INC:
        return "++";
        break;
    case OP_DEC:
        return "--";
        break;
    case OP_ASSIGN:
        return "=";
        break;
    case OP_AASSIGN:
        return "+=";
        break;
    case OP_SASSIGN:
        return "-=";
        break;
    case OP_MASSIGN:
        return "*=";
        break;
    case OP_COMMA:
        return ",";
        break;
    case OP_ADDRO:
        return "&";
        break;
    default:
        return "";
        break;
    }
}
string TreeNode::VarTypeTostring(VarType type) {
    switch(type) {
    case VAR_INTEGER:
        return "int";
        break;
    case VAR_VOID:
        return "void";
        break;
    case VAR_BOOL:
        return "bool";
        break;
    case VAR_CHAR:
        return "char";
        break;
    case VAR_STRING:
        return "string";
        break;
    case Notype:
        return "Notype";
        break;
    default:
        return "";
        break;
    }
}

void tree::get_temp_var(TreeNode *t) {
    if(t->nodeType != NODE_EXPR)
        return;
    if(t->opType < OP_ADD || t->opType > OP_ADDRO)
        return;

    // TreeNode *arg1 = t->child[0];
    // TreeNode *arg2 = t->child[1];

    // if (arg1->nodeType == NODE_EXPR)
    //     temp_var_seq--;
    // if (arg2 && arg2->nodeType == NODE_EXPR)
    //     temp_var_seq--;
    t->temp_var = temp_var_seq;
    tree::temp_var_seq++;
}

void tree::get_const_str(TreeNode *t) {}

string tree::new_label(void) {
    char tmp[20];

    sprintf(tmp, "label%d", label_seq);
    label_seq++;
    return tmp;
}

void tree::stmt_get_label(TreeNode *t) {
    TreeNode *e = t->child[0];
    TreeNode *s1 = t->child[1];
    TreeNode *s2 = t->child[2];
    TreeNode *s3 = t->child[3];
    switch(t->stmtType) {
    case STMT_COMPOUND:
        TreeNode *last;
        TreeNode *p;
        for(p = t->child[0];
            p->nodeType == NODE_STMT && p->stmtType == STMT_DECL;
            p = p->sibling)
            ;

        for(; p; p = p->sibling) {
            last = p;
            recursive_get_label(p);
        }

        t->label.next_label = last->label.next_label;
        if(t->sibling)
            t->sibling->label.begin_label = t->label.next_label;
        break;
    case STMT_WHILE:
        if(t->label.begin_label == "")
            t->label.begin_label = new_label();
        s1->label.next_label = t->label.begin_label;

        s1->label.begin_label = e->label.true_label = new_label();

        if(t->label.next_label == "")
            t->label.next_label = new_label();
        e->label.false_label = t->label.next_label;
        if(t->sibling)
            t->sibling->label.begin_label = t->label.next_label;

        recursive_get_label(e);
        recursive_get_label(s1);
        break;
    case STMT_FOR:
        s3 = s1;
        s1 = e->child[1];
        s2 = e->child[2];
        e = e->child[0];
        if(t->label.begin_label == "")
            t->label.begin_label = new_label();
        s1->label.true_label = new_label();
        if(t->label.next_label == "")
            t->label.next_label = new_label();
        s3->label.next_label = t->label.begin_label;
        s3->label.begin_label = s1->label.true_label;
        s1->label.false_label = t->label.next_label;
        if(t->sibling)
            t->sibling->label.begin_label = t->label.next_label;

        recursive_get_label(s1);
        recursive_get_label(s3);
        break;
    case STMT_IF:
        if(s2 == NULL) { // if(e) s1
            if(e->label.true_label == "")
                e->label.true_label = new_label();
            if(t->label.next_label == "")
                t->label.next_label = new_label();
            e->label.false_label = t->label.next_label;
            s1->label.begin_label = e->label.true_label;
            s1->label.next_label = t->label.next_label;
        } else { // if(e) s1  else s2
            if(e->label.true_label == "")
                e->label.true_label = new_label();
            if(t->label.next_label == "")
                t->label.next_label = new_label();
            e->label.false_label = new_label();
            s1->label.begin_label = e->label.true_label;
            s1->label.next_label = t->label.next_label;
            s2->label.begin_label = e->label.false_label;
            s2->label.next_label = t->label.next_label;
        }
        recursive_get_label(e);
        recursive_get_label(s1);
        if(s2)
            recursive_get_label(s2);
        break;
    }
}
void tree::expr_get_label(TreeNode *t) {
    if(t->type != VAR_BOOL)
        return;

    TreeNode *e1 = t->child[0];
    TreeNode *e2 = t->child[1];
    switch(t->opType) {
    case OP_LAND:
        if(e1->label.true_label == "")
            e1->label.true_label = new_label();
        if(t->label.true_label == "")
            t->label.true_label = new_label();
        if(t->label.false_label == "")
            t->label.false_label = new_label();
        e2->label.true_label = t->label.true_label;
        e1->label.false_label = e2->label.false_label = t->label.false_label;
        break;
    case OP_LOR:
        if(t->label.true_label == "")
            t->label.true_label = new_label();
        if(t->label.false_label == "")
            t->label.false_label = new_label();
        e1->label.false_label = new_label();
        e2->label.false_label = t->label.false_label;
        // 三者真值出口相同
        e1->label.true_label = e2->label.true_label = t->label.true_label;
        break;
    case OP_NOT:
        if(t->label.true_label == "")
            t->label.true_label = new_label();
        if(t->label.false_label == "")
            t->label.false_label = new_label();
        e1->label.false_label = t->label.true_label;
        e1->label.true_label = t->label.false_label;
        break;
    }
    if(e1)
        recursive_get_label(e1);
    if(e2)
        recursive_get_label(e2);
}

void tree::recursive_get_label(TreeNode *t) {
    if(t != NULL) {
        if(t->nodeType == NODE_STMT)
            stmt_get_label(t);
        else if(t->nodeType == NODE_EXPR)
            expr_get_label(t);
    }
}

void tree::get_label(void) {
    TreeNode *p = root->child[0];
    for(; p->nodeType == NODE_STMT && p->stmtType == STMT_DECL; p = p->sibling)
        ;
    p->label.begin_label = "main";
    while(p) {
        recursive_get_label(p);
        p = p->sibling;
    }
}

void tree::stmt_gen_code(ostream &out, TreeNode *t) {
    int i = 0;
    int j = 0;
    int q = 0;
    TreeNode *e = t->child[0];
    TreeNode *s1 = t->child[1];
    TreeNode *s2 = t->child[2];
    TreeNode *s3 = t->child[3];
    switch(t->stmtType) {
    case STMT_IF:
        if(s2 == NULL) {
            recursive_gen_code(out, e);
            out << e->label.true_label << ":" << endl;
            recursive_gen_code(out, s1);
        } else {
            recursive_gen_code(out, e);
            out << e->label.true_label << ":" << endl;
            recursive_gen_code(out, s1);
            out << "\tJMP " << t->label.next_label << endl;
            out << e->label.false_label << ":" << endl;
            recursive_gen_code(out, s2);
        }
        if(t->label.next_label != "")
            out << t->label.next_label << ":" << endl;
        break;
    case STMT_WHILE:
        if(t->label.begin_label != "")
            out << t->label.begin_label << ":" << endl;
        recursive_gen_code(out, e);
        out << e->label.true_label << ":" << endl;
        recursive_gen_code(out, s1);
        out << "\tjmp " << t->label.begin_label << endl;
        if(t->label.next_label != "")
            out << t->label.next_label << ":" << endl;
        break;
    case STMT_FOR:
        s3 = s1;
        s1 = e->child[1];
        s2 = e->child[2];
        e = e->child[0];
        recursive_gen_code(out, e);
        out << t->label.begin_label << ":" << endl;
        recursive_gen_code(out, s1);
        out << s1->label.true_label << ":" << endl;
        recursive_gen_code(out, s3);
        recursive_gen_code(out, s2);
        out << "\tJMP " << t->label.begin_label << endl;

        if(t->label.next_label != "")
            out << t->label.next_label << ":" << endl;
        break;
    case STMT_COMPOUND:
        for(TreeNode *p = t->child[0]; p; p = p->sibling)
            recursive_gen_code(out, p);
        out << endl;
        break;
    case STMT_DECL:
        while(s1) {
            if(s1->nodeType == NODE_EXPR)
                recursive_gen_code(out, s1);
            s1 = s1->sibling;
        }
        break;
    case STMT_SCANF:
        s1 = e->child[1];
        while(s1) {
            i++;
            s1 = s1->sibling;
        }
        for(j = i; j > 0; j--) {
            q = j - 1;
            s2 = e->child[1];
            for(q; q > 0; q--) {
                s2 = s2->sibling;
            }
            out << "\tpushl $_" << s2->child[0]->var_name << endl;
        }
        out << "\tpushl $STR" << e->child[0]->const_str << endl;
        out << "\tcall scanf" << endl;
        out << "\taddl $" << 4 * i + 4 << ", %esp" << endl;
        break;
    case STMT_PRINTF:
        if(e->opType == OP_COMMA) { // printf("", n)
            s1 = e->child[1];
            while(s1) {
                i++;
                s1 = s1->sibling;
            }
            for(j = i; j > 0; j--) {
                q = j - 1;
                s2 = e->child[1];
                for(q; q > 0; q--) {
                    s2 = s2->sibling;
                }
                if(s2->nodeType == NODE_EXPR)
                    out << "\tpushl t" << s2->temp_var << endl;
                else
                    out << "\tpushl _" << s2->var_name << endl;
            }
            out << "\tpushl $STR" << e->child[0]->const_str << endl;
            out << "\tcall printf" << endl;
            out << "\taddl $" << 4 * i + 4 << ", %esp" << endl;
        } else {
            out << "\tpushl $STR" << e->const_str << endl;
            out << "\tcall printf" << endl;
            out << "\taddl $" << 4 << ", %esp" << endl;
        }
        break;
    default:
        break;
    }
}

void tree::expr_gen_code(ostream &out, TreeNode *t) {
    if(t->opType != OP_LAND && t->opType != OP_LOR && t->opType != OP_NOT)
        for(int i = 0; i < MAXChildNum, t->child[i] != NULL; i++)
            recursive_gen_code(out, t->child[i]);
    TreeNode *e1 = t->child[0];
    TreeNode *e2 = t->child[1];
    switch(t->opType) {
    case OP_ASSIGN:
        out << "\tmovl ";
        if(e2->nodeType == NODE_VAR) {
            out << "_" << e2->var_name;
        } else if(e2->nodeType == NODE_CONST) {
            if(e2->type == VAR_INTEGER)
                out << "$" << e2->int_val;
            else if(e2->type == VAR_CHAR)
                out << "STR" << e2->const_str;
        } else {
            out << "t" << e2->temp_var;
        }
        out << ", %eax" << endl;
        out << "\tmovl %eax, t" << t->temp_var << endl;
        out << "\tmovl %eax, _" << e1->var_name << endl;
        break;
    case OP_AASSIGN:
        out << "\tmovl _" << e1->var_name << ", %eax" << endl;
        out << "\taddl ";
        if(e2->nodeType == NODE_VAR)
            out << "_" << e2->var_name;
        else if(e2->nodeType == NODE_CONST)
            out << "$" << e2->int_val;
        else
            out << "t" << e2->temp_var;
        out << ", %eax" << endl;
        out << "\tmovl %eax, t" << t->temp_var << endl;
        out << "\tmovl %eax, _" << e1->var_name << endl;
        break;
    case OP_SASSIGN:
        out << "\tmovl _" << e1->var_name << ", %eax" << endl;
        out << "\tsubl ";
        if(e2->nodeType == NODE_VAR)
            out << "_" << e2->var_name;
        else if(e2->nodeType == NODE_CONST)
            out << "$" << e2->int_val;
        else
            out << "t" << e2->temp_var;
        out << ", %eax" << endl;
        out << "\tmovl %eax, t" << t->temp_var << endl;
        out << "\tmovl %eax, _" << e1->var_name << endl;
        break;
    case OP_ADD:
        out << "\tmovl ";
        if(e1->nodeType == NODE_VAR)
            out << "_" << e1->var_name;
        else if(e1->nodeType == NODE_CONST)
            out << "$" << e1->int_val;
        else
            out << "t" << e1->temp_var;
        out << ", %eax" << endl;
        out << "\taddl ";
        if(e2->nodeType == NODE_VAR)
            out << "_" << e2->var_name;
        else if(e2->nodeType == NODE_CONST)
            out << "$" << e2->int_val;
        else
            out << "t" << e2->temp_var;
        out << ", %eax" << endl;
        out << "\tmovl %eax, t" << t->temp_var << endl;
        break;
    case OP_SUB:
        out << "\tmovl ";
        if(e1->nodeType == NODE_VAR)
            out << "_" << e1->var_name;
        else if(e1->nodeType == NODE_CONST)
            out << "$" << e1->int_val;
        else
            out << "t" << e1->temp_var;
        out << ", %eax" << endl;
        out << "\tsubl ";
        if(e2->nodeType == NODE_VAR)
            out << "_" << e2->var_name;
        else if(e2->nodeType == NODE_CONST)
            out << "$" << e2->int_val;
        else
            out << "t" << e2->temp_var;
        out << ", %eax" << endl;
        out << "\tmovl %eax, t" << t->temp_var << endl;
        break;
    case OP_MUL:
        out << "\tmovl ";
        if(e1->nodeType == NODE_VAR)
            out << "_" << e1->var_name;
        else if(e1->nodeType == NODE_CONST)
            out << "$" << e1->int_val;
        else
            out << "t" << e1->temp_var;
        out << ", %eax" << endl;
        out << "\timull ";
        if(e2->nodeType == NODE_VAR)
            out << "_" << e2->var_name;
        else if(e2->nodeType == NODE_CONST)
            out << "$" << e2->int_val;
        else
            out << "t" << e2->temp_var;
        out << ", %eax" << endl;
        out << "\tmovl %eax, t" << t->temp_var << endl;
        break;
    case OP_DIV:
        out << "\tXOR %edx, %edx" << endl;
        out << "\tmovl ";
        if(e1->nodeType == NODE_VAR)
            out << "_" << e1->var_name;
        else if(e1->nodeType == NODE_CONST)
            out << "$" << e1->int_val;
        else
            out << "t" << e1->temp_var;
        out << ", %eax" << endl;
        out << "\tcmp %edx, %eax" << endl; // 判断被除数是否大于0
        out << "\tjge O" << m << endl;
        out << "\tmovl $0, %edx" << endl;
        out << "\tnot %edx" << endl;
        out << "O" << m << ":" << endl;
        m++;
        out << "\tmovl ";
        if(e2->nodeType == NODE_VAR)
            out << "_" << e2->var_name;
        else if(e2->nodeType == NODE_CONST)
            out << "$" << e2->int_val;
        else
            out << "t" << e2->temp_var;
        out << ", %ebx" << endl;

        out << "\tidiv %ebx" << endl;
        out << "\tXOR %edx, %edx" << endl;
        out << "\tmovl %eax, t" << t->temp_var << endl;
        break;
    case OP_MOD:
        out << "\tXOR %edx, %edx" << endl;
        out << "\tmovl ";
        if(e1->nodeType == NODE_VAR)
            out << "_" << e1->var_name;
        else if(e1->nodeType == NODE_CONST)
            out << "$" << e1->int_val;
        else
            out << "t" << e1->temp_var;
        out << ", %eax" << endl;
        out << "\tcmp %edx, %eax" << endl; // 判断被除数是否大于0
        out << "\tjge O" << m << endl;
        out << "\tmovl $0, %edx" << endl;
        out << "\tnot %edx" << endl;
        out << "O" << m << ":" << endl;
        m++;
        out << "\tmovl ";
        if(e2->nodeType == NODE_VAR)
            out << "_" << e2->var_name;
        else if(e2->nodeType == NODE_CONST)
            out << "$" << e2->int_val;
        else
            out << "t" << e2->temp_var;
        out << ", %ebx" << endl;

        out << "\tidiv %ebx" << endl;
        out << "\tXOR %eax, %eax" << endl;
        out << "\tmovl %edx, t" << t->temp_var << endl;
        break;
    case OP_SADD:
        out << "\tmovl ";
        if(e1->nodeType == NODE_VAR)
            out << "_" << e1->var_name;
        else if(e1->nodeType == NODE_CONST)
            out << "$" << e1->int_val;
        else
            out << "t" << e1->temp_var;
        out << ", %eax" << endl;
        out << "\tmovl %eax, t" << t->temp_var << endl;
        break;
    case OP_UMINUS:
        out << "\tmovl ";
        if(e1->nodeType == NODE_VAR)
            out << "_" << e1->var_name;
        else if(e1->nodeType == NODE_CONST)
            out << "$" << e1->int_val;
        else
            out << "t" << e1->temp_var;
        out << ", %eax" << endl;
        out << "\timul $-1, %eax" << endl;
        out << "\tmovl %eax, t" << t->temp_var << endl;
        break;
    case OP_INC:
        out << "\tmovl _" << e1->var_name << ", %eax" << endl;
        out << "\tmovl %eax, t" << t->temp_var << endl;
        out << "\taddl $1, %eax" << endl;
        out << "\tmovl %eax, _" << e1->var_name << endl;
        break;
    case OP_DEC:
        out << "\tmovl _" << e1->var_name << ", %eax" << endl;
        out << "\tmovl %eax, t" << t->temp_var << endl;
        out << "\tsubl $1, %eax" << endl;
        out << "\tmovl %eax, _" << e1->var_name << endl;
        break;
    case OP_EQUAL:
        out << "\tmovl ";
        if(e1->nodeType == NODE_VAR)
            out << "_" << e1->var_name;
        else if(e1->nodeType == NODE_CONST) {
            if(e1->type == VAR_INTEGER)
                out << "$" << e1->int_val;
            else if(e1->type == VAR_CHAR)
                out << "STR" << e1->const_str;
        } else
            out << "t" << e1->temp_var;
        out << ", %eax" << endl;
        out << "\tcmp ";
        if(e2->nodeType == NODE_VAR)
            out << "_" << e2->var_name;
        else if(e2->nodeType == NODE_CONST) {
            if(e2->type == VAR_INTEGER)
                out << "$" << e2->int_val;
            else if(e2->type == VAR_CHAR)
                out << "STR" << e2->const_str;
        } else
            out << "t" << e2->temp_var;
        out << ", %eax" << endl;
        out << "\tje " << t->label.true_label << endl;
        out << "\tjmp " << t->label.false_label << endl;
        break;
    case OP_NOTEQUAL:
        out << "\tmovl ";
        if(e1->nodeType == NODE_VAR)
            out << "_" << e1->var_name;
        else if(e1->nodeType == NODE_CONST) {
            if(e1->type == VAR_INTEGER)
                out << "$" << e1->int_val;
            else if(e1->type == VAR_CHAR)
                out << "STR" << e1->const_str;
        } else
            out << "t" << e1->temp_var;
        out << ", %eax" << endl;
        out << "\tcmp ";
        if(e2->nodeType == NODE_VAR)
            out << "_" << e2->var_name;
        else if(e2->nodeType == NODE_CONST) {
            if(e2->type == VAR_INTEGER)
                out << "$" << e2->int_val;
            else if(e2->type == VAR_CHAR)
                out << "STR" << e2->const_str;
        } else
            out << "t" << e2->temp_var;
        out << ", %eax" << endl;
        out << "\tjne " << t->label.true_label << endl;
        out << "\tjmp " << t->label.false_label << endl;
        break;
    case OP_GT:
        out << "\tmovl ";
        if(e1->nodeType == NODE_VAR)
            out << "_" << e1->var_name;
        else if(e1->nodeType == NODE_CONST && e1->type == VAR_INTEGER)
            out << "$" << e1->int_val;
        else
            out << "t" << e1->temp_var;
        out << ", %eax" << endl;
        out << "\tcmp ";
        if(e2->nodeType == NODE_VAR)
            out << "_" << e2->var_name;
        else if(e2->nodeType == NODE_CONST && e2->type == VAR_INTEGER)
            out << "$" << e2->int_val;
        else
            out << "t" << e2->temp_var;
        out << ", %eax" << endl;
        out << "\tjg " << t->label.true_label << endl;
        out << "\tjmp " << t->label.false_label << endl;
        break;
    case OP_GTE:
        out << "\tmovl ";
        if(e1->nodeType == NODE_VAR)
            out << "_" << e1->var_name;
        else if(e1->nodeType == NODE_CONST && e1->type == VAR_INTEGER)
            out << "$" << e1->int_val;
        else
            out << "t" << e1->temp_var;
        out << ", %eax" << endl;
        out << "\tcmp ";
        if(e2->nodeType == NODE_VAR)
            out << "_" << e2->var_name;
        else if(e2->nodeType == NODE_CONST && e2->type == VAR_INTEGER)
            out << "$" << e2->int_val;
        else
            out << "t" << e2->temp_var;
        out << ", %eax" << endl;
        out << "\tjge " << t->label.true_label << endl;
        out << "\tjmp " << t->label.false_label << endl;
        break;
    case OP_LT:
        out << "\tmovl ";
        if(e1->nodeType == NODE_VAR)
            out << "_" << e1->var_name;
        else if(e1->nodeType == NODE_CONST && e1->type == VAR_INTEGER)
            out << "$" << e1->int_val;
        else
            out << "t" << e1->temp_var;
        out << ", %eax" << endl;
        out << "\tcmp ";
        if(e2->nodeType == NODE_VAR)
            out << "_" << e2->var_name;
        else if(e2->nodeType == NODE_CONST && e2->type == VAR_INTEGER)
            out << "$" << e2->int_val;
        else
            out << "t" << e2->temp_var;
        out << ", %eax" << endl;
        out << "\tjl " << t->label.true_label << endl;
        out << "\tjmp " << t->label.false_label << endl;
        break;
    case OP_LTE:
        out << "\tmovl ";
        if(e1->nodeType == NODE_VAR)
            out << "_" << e1->var_name;
        else if(e1->nodeType == NODE_CONST && e1->type == VAR_INTEGER)
            out << "$" << e1->int_val;
        else
            out << "t" << e1->temp_var;
        out << ", %eax" << endl;
        out << "\tcmp ";
        if(e2->nodeType == NODE_VAR)
            out << "_" << e2->var_name;
        else if(e2->nodeType == NODE_CONST && e2->type == VAR_INTEGER)
            out << "$" << e2->int_val;
        else
            out << "t" << e2->temp_var;
        out << ", %eax" << endl;
        out << "\tjle " << t->label.true_label << endl;
        out << "\tjmp " << t->label.false_label << endl;
        break;
    case OP_LAND:
        recursive_gen_code(out, e1);
        out << e1->label.true_label << ":" << endl;
        recursive_gen_code(out, e2);
        break;
    case OP_LOR:
        recursive_gen_code(out, e1);
        out << e1->label.false_label << ":" << endl;
        recursive_gen_code(out, e2);
        break;
    case OP_NOT:
        recursive_gen_code(out, e1);
        break;
    }
}

void tree::recursive_gen_code(ostream &out, TreeNode *t) {
    if(t != NULL) {
        if(t->nodeType == NODE_STMT)
            stmt_gen_code(out, t);
        else if(t->nodeType == NODE_EXPR)
            expr_gen_code(out, t);
    }
}

void tree::gen_decl(ostream &out, TreeNode *t) {
    for(TreeNode *p = t->child[1]; p; p = p->sibling) {
        if(p->nodeType == NODE_VAR && p->type == VAR_INTEGER) {
            out << "_" << p->var_name << ":" << endl;
        } else if(p->nodeType == NODE_VAR && p->type == VAR_CHAR) {
            out << "_" << p->var_name << ":" << endl;
        } else if(p->nodeType == NODE_EXPR && p->opType == OP_ASSIGN) {
            out << "_" << p->child[0]->var_name << ":" << endl;
        }
        out << "\t.zero\t4" << endl;
        out << "\t.align\t4" << endl;
    }
}

void tree::gen_code(ostream &out) {
    // gen_header(out);

    out << "\t.section .bss" << endl;
    TreeNode *p = root->child[0];
    while(p) {
        if(p->nodeType == NODE_STMT && p->stmtType == STMT_COMPOUND)
            p = p->child[0];
        if(p->nodeType == NODE_STMT && p->stmtType == STMT_DECL)
            gen_decl(out, p);
        p = p->sibling;
    }

    for(int i = 0; i < temp_var_all; i++) {
        out << "t" << i << ":" << endl;
        out << "\t.zero\t4" << endl;
        out << "\t.align\t4" << endl;
    }
    out << endl;

    out << "\t.section .rodata" << endl;
    for(int i = 0; i < const_str_all; i++) {
        out << "STR" << i << ":" << endl;
        out << "\t.string\t" << str_table[i] << "" << endl;
        out << "\t.align\t4" << endl;
    }

    out << endl;
    out << "\t.section .text" << endl;
    out << "\t.globl main" << endl << endl;
    out << "main:" << endl;

    p = root->child[0];
    // for (; p->nodeType == NODE_STMT && p->stmtType == STMT_DECL; p =
    // p->sibling);
    while(p) {
        recursive_gen_code(out, p);
        p = p->sibling;
    }

    if(root->label.next_label != "")
        out << root->label.next_label << ":" << endl;
    out << "\tret" << endl;
}
