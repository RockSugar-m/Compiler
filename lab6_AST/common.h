#ifndef COMMON_H
#define COMMON_H

#include "tree.h"
#define YYSTYPE TreeNode *


// struct IDAttributes
// {
//     string name;
//     VarType type;
// 	bool is_first=true;
// };

// class symbol_table{
// public:
// 	IDAttributes table[MAX_ID];
// 	int size;
// 	int lookup(string name){
// 		for (int i = 0; i < size; i++)
// 			if (table[i].name == name)
// 				return i;
// 		return -1;
// 	}
// 	int insert(string name, VarType type){
// 		if (size >= MAX_ID)
// 			return -1;
// 		table[size].name = name;
// 		table[size].type = type;
// 		size++;
// 		return size - 1;
// 	}
// };

#endif