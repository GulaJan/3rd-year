#ifndef _INSLISTH_
#define _INSLISTH_
#include "errs.h"
//#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
typedef struct {
	int insOp;
	void *adr1;
	void *adr2;
	void *adr3;
	int ins_type;
	int frame;
	int frame2;
}tIns;

typedef struct tElem {
	tIns instr;
    struct tElem *next;
} *tElemPtr;

typedef struct {
	tElemPtr Last;
    tElemPtr Act;
    tElemPtr First;
} tList;
/****************************************
				EXPLANATION
<var> -> variable
<symb> -> a constant or a variable
<label> -> navesti
GF -> Global Frame -> Automatically set as empty on the start, used for GLOBAL variables
LF -> Local Frame -> Undefined at the start, used for LOCAL variables, we need to use CREATEFRAME first. Points to the TOP of our Frame Stack which refers to the frame we use at the moment
TF -> Temporary Frame -> Undefined at the start can be used to store information and then be pushed into our Frame Stack that will make him a LOCAL frame.
*****************************************/
typedef enum {
	IO_ADDS,			// ADDS 								ZOZERIE 3 VECI ZO ZASOBNIKU
	IO_SUBS,			// SUBS 
	IO_MULS,			// MULS 
	IO_DIVS,			// DIVS 					 			AK JEDEN OPERAND (symb1 or symb2) NENI DESATINNE CISLO -> CHYBA 53 AK JE DELENIE NULOUT -> CHYBA 57
	IO_EQ,				// EQS 
	IO_G,				// GTS 
	IO_L,				// LTS 
	IO_GE,
	IO_LE,
	IO_ASS,				// MOVE <var> <symbol>
	IO_IF,				// LABEL <label>
	IO_WHILE,			// LABEL <label> 
	IO_WHILEJUMP,		// JUMP <label>
	IO_IFCHECK,
	IO_IFEND,
	IO_INPUT,			// READ <var> <type>
	IO_WRITE,			// WRITE <symb>
	IO_SORT,
	IO_COPY,
	IO_FIND,
	IO_LEN,				// STRLEN <var> <symb>
	IO_ID_VAL,
	IO_NULL,
	IO_PRINT,			// WRITE <symb>
	IO_CONCAT,			// CONCAT <var> <symb1> <symb2>
	IO_GETCHAR,			// GETCHAR <var> <symb1> <symb2> 	  VRATI ZNAK V RETAZCI SYMB1 NA POZICII SYMB2 DO PREMMEN VAR INDEX OD NULY MIMO RETAZEC -> CHYBA 58
	IO_SETCHAR,			// SETCHAR <var> <symb1> <symb2>	  ZMENI ZNAK V RETAZCI VAR NA POZICII SYMB1 NA ZNAK SYMB2 INDEX OD NULY AK MIMO RETAZEC -> CHYBA 58
	IO_TYPE,			// TYPE <var> <symb>                  VRATI TYP SYMB DO PREMENNEJ VAR
	IO_LABEL, 			// LABEL <label>					  POKUS O REDEFINICI NAVESTI -> CHYBA 52 , OZNACUJU SA NIM USEKY KODU KAM MOZNO BUDEME CHCIET SKOCIT
	IO_JUMP,			// JUMP <label>
	IO_JUMP_IF_EQ,		// JUMPIFEQ <label> <symb1> <symb2>   SKOK NA NAVESTI PRI ROVNOSTI AK SU SYMB1 A SYMB2 NENI ROVNAKEHO TYPU -> CHYBA 53
	IO_JUMP_IF_NEQ,		// JUMPIFNEQ <label> <symb1> <symb2>  SKOK NA NAVESTI PRI NEROVNOSTI AK SU SYMB1 A SYMB2 NENI ROVNAKEHO TYPU -> CHYBA 53
	IO_JUMP_IF_EQ_S,	// JUMPIFEQS <label>
	IO_JUMP_IF_NEQ_S,	// JUMPIFNEQS <label>
	IO_BREAK,			// BREAK
	IO_DPRINT,			// DPRINT <symb> 					  VYPIS HODNOTY NA STDERR
	// STACK OPERATIONS
	IO_PUSHS,			// PUSHS <symb>						
	IO_POPS,			// POPS <var>						  AK JE ZASOBNIK PRAZDNY -> CHYBA 56 !!!
	IO_CLEARS,			// CLEARS
	// CONVERSIONS
	
	IO_INT2FLOAT,		// INT2FLOAT <var> <symb>
	IO_FLOAT2INT,		// FLOAT2INT <var> <symb>                IBA OREZANIE DESATINNEJ CASTI
	IO_FLOAT2R2EINT,	// FLOAT2R2EINT <var> <symb>			 E - EVEN - PARNE, ZAOKRUHLENIE NA CELE CISLO - PREFERENCIA SUDA 1.5 A 2.5 SA OBE ZAOKRUHLIA NA PARNE CISLO CIZE 2
	IO_FLOAT2R2OINT,	// FLOAT2R2OINT <var> <symb>			 O - ODD - NEPARNE ZAOKRUHLENIE NA CELE CISLO - PREFERENCIA LICHA 1.5 -> 1 , 2.5 -> 3
	IO_INT2CHAR,		// INT2CHAR <var> <symb>				 CELE CISLO NA JEHO ASCII REPREZENTACIU AK INT NENI 0-255 -> CHYBA 58
	IO_STRI2INT,		// STRI2INT <var> <symb1> <symb2>		 ZNAK V RETAZCI SYMB1 NA POZICII SYMB2 OD NULY PREVEDIE NA JEHO ORDINALNU ASCII HODNOTU A ULOZI DO VAR AK JE HODNOTA MIMO RETAZEC -> CHYBA 58
	// STACK VERSIONS OF CONVERSION INSTRUCTIONS
	IO_INT2FLOATS,		// INT2FLOATS <var> <symb>
	IO_FLOAT2INTS,		// FLOAT2INTS <var> <symb>
	IO_FLOAT2R2EINTS,	// FLOAT2R2EINTS <var> <symb>
	IO_FLOAT2R2OINTS,	// FLOAT2R2OINTS <var> <symb>
	IO_INT2CHARS,		// INT2CHARS <var> <symb>
	IO_STRI2INTS,		// STRI2INTS <var> <symb1> <symb2>

	// FRAME INSTRUCTIONS
	IO_CREATE_FRAME,		// CREATEFRAME
	IO_PUSH_FRAME, 		// PUSHFRAME    PRESUN DOCASNEHO, TEMPORARY FRAME -> TF NA ZASOBNIK RAMCU TF JE PO PREVEDENI NEDEFINOVANY , SNAHA OD PUSHNUTIE NEDEFINOVANEHO TF => ERROR 55 !!!!!!!
	IO_POP_FRAME,		// POPFRAME 	PRESUN AKTUALNEHO -> LF DO DOCASNEHO -> TF POKIAL NENI ZIADNY LF -> CHYBA 55 !!!

	// MAY BE NEEDED
	IO_VAR_DEF,			// DEFVAR <var>
	IO_CALL,				// CALL <label>
	IO_RET, 				// RETURN
	
	IO_ADD,				// ADD <var> <symb1> <symb2>
	IO_SUB,				// SUB <var> <symb1> <symb2>
	IO_MUL,				// MUL <var> <symb1> <symb2>
	IO_DIV,				// DIV <var> <symb1> <symb2>
	IO_L_NON_STACK,		// LT <var> <symb1> <symb2>
	IO_G_NON_STACK,		// GT <var> <symb1> <symb2>
	IO_EQ_NON_STACK,	// EQ <var> <symb1> <symb2>
	IO_AND,				// AND <var> <symb1> <symb2>
	IO_OR,				// OR <var> <symb1> <symb2>
	IO_NOT,				// NOT <var> <symb1> <symb2>
	
	IO_AND_S,			// ANDS <var> <symb1> <symb2>
	IO_OR_S,			// ORS <var> <symb1> <symb2>
	IO_NOT_S			// NOTS <var> <symb1> <symb2>
}tInsOp;

tIns instr;
tList list;
char *tmp_ins;
void InitList();
void DisposeList();
void InsertFirst(tIns ins);
void InsertLast(tIns ins);
void DeleteFirst();
void PostDelete();
void PostInsert(tIns ins);
void Succ();
void Jump(void *jumpIns);
int generator();
int switchtype(int type, char* instruct, FILE *f, void* adr);
void instr_generator(int ins_type, void *a1, void *a2, void *a3, int type, int frame, int frame2);
#endif