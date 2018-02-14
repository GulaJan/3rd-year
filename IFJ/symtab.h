#ifndef SYMTAB_H
#define SYMTAB_H

#include <stdbool.h>
#include <stdlib.h>
#include "instruction.h"

// STitem je prvok v hashovacej tabulke -> ten obsahuje 2 struct, 1 je id_type(bud variable alebo function) a druhy je item_data -> ten obsahuje 2 ukazatele na 2 struct id a function_id



typedef enum{
    variable,
    function,
}id_type;

typedef enum{
	integer,
	d_double,
	string,
}data_type;

typedef struct{
    data_type param_type;
	char *param_name;
}func_param;

typedef struct{
    data_type param_type;
	char *param_name;
}func_param_declaration;

typedef union{
	int v_int;
	double v_double;
	char *v_string;
} value;

typedef struct id{
	bool initialized;
	bool declared;
	value value;
    data_type data_type;
	char *name;
	int varFrame;  // 1 = GF , 2 = TF, 3 
}id;

typedef union{
	struct id variable;
	struct function_id *function;
}item_data;

typedef struct STitem{
   	char *key;
   	struct STitem *next;
   	id_type item_type;
   	item_data item_data;
}STitem;

typedef struct STable{
    STitem *items[1000];
    int size;
}STable;

typedef struct function_id{
	char *name;
	bool declared;
	bool initialized;
	int num_of_args;
	func_param *args;
	func_param_declaration *params;
	int Start;
	data_type data_type;
	data_type dec_data_type;
	STable local_table;
	int funcFrame;
	bool emptyRet;
	bool declare_call;
}function_id;
STitem *func_name;
STable GST, LST;  // GST = Global Symbol Table  LST = Local symbol table
int STHash(char* key);
STitem* STFind(STable* st, char* key);
STitem* STAdd(STable* st, char* key, int malloc_type);
void STinit (STable *table);
void STfree (STable *table);
void Param_Add (STitem *item, char *param, int num_arg, data_type param_type);
void Param_Declaration_Add (STitem *item, char *param, int num_arg, data_type param_type);
char * f_Param (STitem *item, int num);
char * f_Param_declaration(STitem *item, int num);
#endif