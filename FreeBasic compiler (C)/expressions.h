#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#define TOK 67
#define INT 68
#define DBL 69
#define STR 70
#define E_NT 71
#define P_L 72
#define STATE_BOOL 73

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include "scanner.h"
#include "errs.h"
#include "stack.h"
#include "symtab.h"

int pt_decode(int tokNum);
bool rules(int num, ...);
int expressionAnalysis(FILE *f, int tokNum, char *input, int exp_type, char *variable, int if_flag, int func_flag);
int reduce (int exp_type);
int instr_push(int type, int logical);
int function_call(char *function, FILE *f, int exp_type);
#endif

