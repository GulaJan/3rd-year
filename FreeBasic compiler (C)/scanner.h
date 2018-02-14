/**************************************************
Project: IFJ 17 FIT BUT
Info: Header file with enum types
Team: Jan Gula, Duong Nguyen Hoang, Martin Nizner 
**************************************************/

#ifndef SCANNER_H
#define SCANNER_H
#include <stdio.h>
#include "errs.h"

int Token(char **buffer, FILE *f);
char *AddChar(char *string, char c);
char *StringInit();

typedef enum {
	// No state loaded

	/*00*/START,

	/*	KEYWORDS	*/

	/*01*/KW_AS,	
	/*02*/KW_DECLARE,
	/*03*/KW_DIM,		
	/*04*/KW_DO,		
	/*05*/KW_DOUBLE, 	
	/*06*/KW_ELSE, 		
	/*07*/KW_END,		
	/*08*/KW_FIND,		
	/*09*/KW_FUNCTION,	
	/*10*/KW_IF,		
	/*11*/KW_INPUT,		
	/*12*/KW_INTEGER,	
	/*13*/KW_LOOP,		
	/*14*/KW_PRINT,	
	/*15*/KW_RETURN,	
	/*16*/KW_SCOPE,		
	/*17*/KW_SORT,		
	/*18*/KW_STRING,	
	/*19*/KW_THEN,		
	/*20*/KW_WHILE,		

	/*	TOKENS	*/

	/*21*/TOK_SEMICOLON,			// ;	
	/*22*/TOK_PLUS,					// +
	/*23*/TOK_MINUS,				// -
	/*24*/TOK_MULTIPLY,				// *
	/*25*/TOK_DIVIDE,				// /
	/*26*/TOK_QUOTES,				// "
	/*27*/TOK_STRING_START,			// !
	/*28*/TOK_ASSIGN_OR_EQUALS,		// =
	/*29*/TOK_LOWER,				// <
	/*30*/TOK_GREATER,				// >
	/*31*/TOK_LEFT_SB,				// [ SB - square bracket
	/*32*/TOK_RIGHT_SB,				// ]
	/*33*/TOK_LEFT_CB,				// { CB - curly bracket
	/*34*/TOK_RIGHT_CB,				// }
	/*35*/TOK_LEFT_RB,				// ( RB - round bracket
	/*36*/TOK_RIGHT_RB,				// )
	/*37*/TOK_EOF,					// end of file
	/*38*/TOK_WHOLENUMBER_DIVIDE,	// 5 / 2 = 2
	/*39*/TOK_ID,					// identifier
	/*40*/TOK_INT,					// int NOT A KEYWORD BUT A NUMBER LIKE 69
	/*41*/TOK_DOUBLE,				// double NOT A KEYWORD BUT A NUMBER LIKE 6.9
	/*42*/TOK_EXPONENT,				// e, E - 1E3; 2e-3; 2,3e-4;
	/*43*/TOK_ZERO_EXPONENT,		// when the exponent is 0 , eg. 2e00003
	/*44*/TOK_EXP_SIGN, 			// +- in exponent
	/*45*/TOK_EXTRA_ZERO, 			// zeros at the number start 00003
	/*46*/TOK_REAL_PART,			// real part .003
	/*47*/TOK_ESCAPE_SEQUENCE,		// e.g. \n
	/*48*/TOK_LINE_COMMENT,			// '
	/*49*/TOK_BLOCK_COMMENT,		// /'
	/*50*/TOK_END_BLOCK_COMMENT,	// '/
	/*51*/TOK_STRING_STREAM, 		// !"THIS IS A STRING STREAM IN FREEBASIC"
	/*52*/TOK_NUMBER,				// Any number, we don't know if it's an INT or DOUB
	/*53*/TOK_GREATER_EQ,			// >=
	/*54*/TOK_LOWER_EQ,				// <=
	/*55*/TOK_NOT_EQUAL,			// <>
	/*56*/TOK_NT, 					// Non terminal (white chars)
	/*57*/KW_SHARED,
	/*58*/KW_STATIC,
	/*59*/TOK_COMMA,
	/*60*/TOK_EOL
}TokState;

#endif