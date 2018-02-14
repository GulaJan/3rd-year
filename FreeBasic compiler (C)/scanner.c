#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "scanner.h"

int Token(char **buffer, FILE *f) {
	int state = START;
	int lex, blockCommentFlag = 0;
	int e = 0; // flag pre exponent
	int escape = 0; // flag pre znak '\'
	int dot = 0; // flag pre bodku
	char index[3]; // string pre cislo v escape sek
	int num = 0; // pocet cisel v escape sek
	int zaciatok = 0; // flag pre prvu "
	int konec = 0; // flag pre poslednu "
	*buffer = StringInit();
	while (1) {
		lex = getc(f);
		switch (state) {
			case START: 
				//printf ("Start\n");
				if ((isspace(lex)) && (lex != '\n')) {
					state = START;
					break;
				}
				else if (lex == '\n') {
					state = START;
					return TOK_EOL;
					break;
				}
				else if (lex == EOF) {
					return TOK_EOF;
					break;
				}
				else if (((lex >= 'A') && (lex <= 'Z')) || ((lex >= 'a') && (lex <= 'z'))) {
                   	state = TOK_ID;
          
                   	*buffer = AddChar(*buffer, lex);
                   	
                   	break;
                }   	
                else if (lex == '_') {
                	state = TOK_ID;
                	*buffer = AddChar(*buffer, lex);
                	break;
                }
                else if isdigit(lex) {
                	state = TOK_NUMBER;
                	*buffer = AddChar(*buffer, lex);
                	break;
                }
                else if (lex == '!'){
                	state = TOK_STRING_STREAM;
                	zaciatok = 1;
                	break;
                }
                else if (lex == '\'') {
                	state = TOK_LINE_COMMENT;
                	break;
                }	
                else if (lex == '/') {
                	state = TOK_DIVIDE;
                	break;
                }	
                else if (lex == '+') {
                	state = START;
                	return TOK_PLUS;
                	break;
                }	
                else if (lex == '*') {
                	state = START;
                	return TOK_MULTIPLY;
                	break;	
                }
                else if (lex == '=') {
                	state = START;
                	return TOK_ASSIGN_OR_EQUALS;
                	break;
                }	
                else if (lex == '<') {
                	state = TOK_LOWER;
                	break;
                }
                else if (lex == '>') {
                	state = TOK_GREATER;
                	break;
                }
                else if (lex == '-') {
                	state = START;
                	return TOK_MINUS;
                	break;
                }	
                else if (lex == '\\') {
                	state = START;
                	return TOK_WHOLENUMBER_DIVIDE;
                	break;
                }
                else if (lex == ';') {
                	state = START;
                	return TOK_SEMICOLON;
                	break;
                }
                else if (lex == '(') {
                	state = START;
                	return TOK_LEFT_RB;
                	break;
                }
                else if (lex == ')') {
                	state = START;
                	return TOK_RIGHT_RB;
                	break;
                }
                else if (lex == ',') {
                	state = START;
                	return TOK_COMMA;
                	break;
                }
                else { 
                	//fprintf(stderr,"Error lex\n");
                	return ERR_LEX;
                }	


	
				break;
			case TOK_ID:
			
				if (lex == '_') {
					state = TOK_ID;
					*buffer = AddChar(*buffer, lex);
					break;
				}
				
				else if (isalnum(lex)) {
					state = TOK_ID;
					*buffer = AddChar(*buffer, lex);
					break;
				}
				else if (isspace(lex) || isdigit(lex) || lex == '\'' || lex == '!' || lex == '/' || lex == '\\' || lex == '>' || lex == '<' || lex == '=' || lex == '+' || lex == '*' || lex == '-' || lex == '(' || lex == ')' || lex == ';' || lex == EOF || lex == ',' || lex == '\n')  {
					if ((strcasecmp(*buffer, "as")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_AS;
					}
					else if ((strcasecmp(*buffer, "declare")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_DECLARE;
					}
					else if ((strcasecmp(*buffer, "dim")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_DIM;
					}
					else if ((strcasecmp(*buffer, "do")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_DO;
					}
					else if ((strcasecmp(*buffer, "double")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_DOUBLE;
					}
					else if ((strcasecmp(*buffer, "else")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_ELSE;
					}
					else if ((strcasecmp(*buffer, "end")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_END;
					}
					else if ((strcasecmp(*buffer, "find")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_FIND;
					}
					else if ((strcasecmp(*buffer, "function")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_FUNCTION;
					}
					else if ((strcasecmp(*buffer, "if")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_IF;
					}
					else if ((strcasecmp(*buffer, "input")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_INPUT;
					}
					else if ((strcasecmp(*buffer, "integer")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_INTEGER;
					}
					else if ((strcasecmp(*buffer, "loop")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_LOOP;
					}
					else if ((strcasecmp(*buffer, "print")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_PRINT;
					}
					else if ((strcasecmp(*buffer, "return")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_RETURN;
					}
					else if ((strcasecmp(*buffer, "scope")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_SCOPE;
					}
					else if ((strcasecmp(*buffer, "sort")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_SORT;
					}
					else if ((strcasecmp(*buffer, "string")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_STRING;
					}
					else if ((strcasecmp(*buffer, "then")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_THEN;
					}
					else if ((strcasecmp(*buffer, "while")) == 0) {
						state = START;
						ungetc(lex,f);
						return KW_WHILE;
					}	
					else {	
					
					state = START;
					ungetc(lex, f);
					return TOK_ID;
				    }
 				}
 				else {
 				   // fprintf(stderr,"Lexikalny error\n");
 					return ERR_LEX;
 					break;
 				}

            case TOK_NUMBER:
				if (isdigit(lex)) {
					*buffer = AddChar(*buffer, lex);
					state = TOK_NUMBER;
					break;
				}
				else if ((lex == 'e') || (lex == 'E')) {
					*buffer = AddChar(*buffer, lex);
					e = 1;
					state = TOK_EXPONENT;
					break;
				}
				else if ((isspace(lex)) || (isdigit(lex)) || (lex == ')') || (lex == '\'') || (lex == '+') || (lex == '-') || (lex == '/') || (lex == '\\') || (lex == '*') || (lex == '<') || (lex == '>') || (lex == '=') || (lex == EOF) || (lex== ';') || (lex == ',') || (isalnum(lex)) || (lex == '_')) {
					// int token_int = atoi(buffer); -- ci treba kontrolovat velkost cisla
					ungetc(lex, f);
					return TOK_INT;
				}
				else if (lex == '.'){
					*buffer = AddChar(*buffer, lex);
					dot = 1;
					state = TOK_DOUBLE;
					break;
				}
				else{
					//fprintf(stderr, "Chyba. Neplatne cislo\n");
					return ERR_LEX;
				}

			case TOK_EXPONENT:
				if (e == 1) { // ked je predchadzajuci znak e
					if ((lex == '+') || (lex == '-') || (isdigit(lex))) {
						state = TOK_EXPONENT;
						*buffer = AddChar(*buffer, lex);
						e = 0;
						break;
					}
					else{
						//fprintf(stderr, "Chyba. Neplatne cislo s exponentom\n");
						return ERR_LEX;
					}
				}
				else if (isdigit(lex)){
					state = TOK_EXPONENT;
					*buffer = AddChar(*buffer, lex);
					e = 0;
					break;
				}
				else if ((isspace(lex)) || (isdigit(lex)) || (lex == ')') || (lex == '\'') || (lex == '+') || (lex == '-') || (lex == '/') || (lex == '\\') || (lex == '*') || (lex == '<') || (lex == '>') || (lex == '=') || (lex == EOF)) {
					
					ungetc(lex, f);
					return TOK_DOUBLE;
				}
				else{
					//fprintf(stderr, "Chyba. Neplatne cislo s exponentom\n");
					return ERR_LEX;
				}

			case TOK_DOUBLE:
				if (dot == 1){ // ked je predchadzajuci znak .
					if (isdigit(lex)){
						dot = 0;
						state = TOK_DOUBLE;
						*buffer = AddChar(*buffer, lex);
						break;
					}
					else {
						//fprintf(stderr, "Chyba. Neplatne desatinne cislo\n");
						return ERR_LEX;
					}
				}
				else if ((lex == 'e') || (lex == 'E')){
					e = 1;
					*buffer = AddChar(*buffer, lex);
					state = TOK_EXPONENT;
					break;
				}
				else if (isdigit(lex)){
					state = TOK_DOUBLE;
					*buffer = AddChar(*buffer, lex);
					break;
				}
				else if ((isspace(lex)) || (isdigit(lex)) || (lex == ')') || (lex == '\'') || (lex == '+') || (lex == '-') || (lex == '/') || (lex == '\\') || (lex == '*') || (lex == '<') || (lex == '>') || (lex == '=') || (lex == EOF) || (lex == ';')|| (lex == ',') || (isalnum(lex)) || (lex == '_')) {
					ungetc(lex, f);
					return TOK_DOUBLE;
				}
				else{
					//fprintf(stderr, "Chyba. Neplatne desatinne cislo\n");
					return ERR_LEX;
				}

			case TOK_STRING_STREAM:
				if ((lex == '"') && (zaciatok == 1)){
					//fprintf (stderr,"state 1\n");
					zaciatok = 0;
					//*buffer = AddChar(*buffer, lex);
					state = TOK_STRING_STREAM;
					break;
				}
				else if ((lex != '"') && (escape == 0) && (lex != '\n') && (lex != '\\') && (konec == 0)){
					//fprintf(stderr,"Znak1\n");
					if (lex == EOF)
						return ERR_LEX;
					state = TOK_STRING_STREAM;
					if (isspace(lex)){
						//fprintf(stderr,"Space\n");
						lex = '\\';
						*buffer = AddChar(*buffer, lex);
						 lex = '0';
						*buffer = AddChar(*buffer, lex);
						lex = '3';
						*buffer = AddChar(*buffer, lex);
						lex = '2';
						*buffer = AddChar(*buffer, lex);
					}
					else
						*buffer = AddChar(*buffer, lex);
					break;
				}
				else if ((lex == '\\') && (escape == 0)){
					//fprintf(stderr,"Dalsi stav\n");
					escape = 1;
					state = TOK_STRING_STREAM;
					break;
				}
				else if ((lex == 'n') && (escape == 1)){
					//fprintf(stderr,"Escape\n");
					lex = '\\';
					*buffer = AddChar(*buffer, lex);
					lex = '0';
					*buffer = AddChar(*buffer, lex);
					lex = '1';
					*buffer = AddChar(*buffer, lex);
					lex = '0';
					*buffer = AddChar(*buffer, lex);
					state = TOK_STRING_STREAM;
					escape = 0;
					break;
				}
				else if ((lex == 't') && (escape == 1)){
					lex = '\\';
					*buffer = AddChar(*buffer, lex);
					lex = '0';
					*buffer = AddChar(*buffer, lex);
					lex = '0';
					*buffer = AddChar(*buffer, lex);
					lex = '9';
					*buffer = AddChar(*buffer, lex);
					state = TOK_STRING_STREAM;
					escape = 0;
					break;
				}
				else if ((lex == '"') && (escape == 1)){
					*buffer = AddChar(*buffer, lex);
					state = TOK_STRING_STREAM;
					escape = 0;
					break;
				}
				else if ((lex == '\\') && (escape == 1)){
					*buffer = AddChar(*buffer, lex);
					lex = '0';
					*buffer = AddChar(*buffer, lex);
					lex = '9';
					*buffer = AddChar(*buffer, lex);
					lex = '2';
					*buffer = AddChar(*buffer, lex);
					state = TOK_STRING_STREAM;
					escape = 0;
					break;
				}
				else if ((isdigit(lex)) && (escape == 1)){ // pre \000 napr
					num = 1;
					index[0] = lex;
					escape = 0;
					state = TOK_ESCAPE_SEQUENCE;
					break;
				}
				
				else if ((lex != '\n') && (isspace(lex)) && (konec == 0)){
					*buffer = AddChar(*buffer, lex);
					state = TOK_STRING_STREAM;
					break;
				}
				else if ((lex == '"') && (escape == 0) && (zaciatok == 0)){
					//fprintf(stderr,"konec\n");
					state = TOK_STRING_STREAM;
					//*buffer = AddChar(*buffer, lex);
					konec = 1;

					//return TOK_STRING_STREAM;
					break;
				}
				
				else if ((lex == ';') && (konec == 1)){
					ungetc(lex, f);
					return TOK_STRING_STREAM;
				}
				else if ((lex == ')') && (konec == 1)){
					ungetc(lex, f);
					return TOK_STRING_STREAM;
				}
				else if ((lex == '\n') && (konec == 1)){
					ungetc(lex, f);
					return TOK_STRING_STREAM;
				}
				else if ((isspace(lex)) && (konec == 1)) {
					ungetc(lex, f);
					return TOK_STRING_STREAM;
					break;
				}
				else if ((lex == ',') && (konec == 1)){
					ungetc(lex, f);
					return TOK_STRING_STREAM;
				}
				else {
					//fprintf (stderr,"Neplatny string %d\n", lex);
					return ERR_LEX;
					break;
				}	
				break;
				//else if (lex == '\n'){ preg

			case TOK_ESCAPE_SEQUENCE:
				if ((!isdigit(lex)) && (num < 3)){
				//	fprintf(stderr, "Chyba. Nemoze byt napr /1");
					return ERR_LEX;
					break;
				}
				else if ((isdigit(lex)) && (num == 1)){
					index[1] = lex;
					num = 2;
					state = TOK_ESCAPE_SEQUENCE;
					break;
				}
				else if ((isdigit(lex)) && (num == 2)){
					num = 3;
					index[2] = lex;
					/*char * rest;
					int cislo;
					cislo = strtoul(index, &rest, 10);
					if ((cislo < 1) || (cislo > 255)){
						fprintf(stderr, "Neplatny rozsah escape sekv\n");
						return ERR_LEX;
					}

					char esp = cislo;
					*/char esp1 = '\\';
					*buffer = AddChar(*buffer, esp1);
					*buffer = AddChar(*buffer, index[0]);
					*buffer = AddChar(*buffer, index[1]);
					*buffer = AddChar(*buffer, index[2]);
					state = TOK_STRING_STREAM;
					num = 0;
					break;
				}
	       
				
			case TOK_LOWER:
				if(lex == '=') {
					state = START;
					return TOK_LOWER_EQ;
				} else if (lex == '>') {
					state = START;
					return TOK_NOT_EQUAL;
				} else {
					ungetc(lex, f);
					state = START;
					return TOK_LOWER;
				}
				break;

			case TOK_GREATER:
				if(lex == '=') {
					state = START;
					return TOK_GREATER_EQ;
				} else {
					ungetc(lex, f);
					state = START;
					return TOK_GREATER;
				}
				break;
			case TOK_LINE_COMMENT:
				if(lex == '\n' || lex == EOF) {
					state = START;
					ungetc(lex, f);
					break;
				}
				else {
					state = TOK_LINE_COMMENT;
					break;
				}
			case TOK_DIVIDE:
				if(blockCommentFlag == 1) {
					if(lex == '\'') {
						lex = getc(f);
						if(lex == '/') {
							state = START;
							blockCommentFlag = 0;
							//fprintf(stderr,"si tu?%c\n",lex);
							break;
						} else 
							return ERR_LEX;
					} else if(lex == EOF)
							return 0;
					break;
				}
				else if(isspace(lex)) {
					state = START;
					return TOK_DIVIDE;
				}
				else if(lex == '\'') {
					blockCommentFlag = 1;
					state = TOK_DIVIDE;
					break;
				}
				else if ((isspace(lex)) || (isdigit(lex)) || (lex == '!') || (lex == '\'') || (lex == '+') || (lex == '-') || (lex == '/') || (lex == '\\') || (lex == '*') || (lex == '<') || (lex == '>') || (lex == '=') || (lex == '(') || (lex == ')') || (lex == EOF) || (lex == ',')) {
					//else if (podmienky)
					ungetc(lex,f);
					state = START;
					return TOK_DIVIDE;
				} else {
					//fprintf(stderr,"Lexical error.\n");
					return ERR_LEX;
				}					

				break;
			default:
               // fprintf (stderr,"Neznamy tok %c\n",lex);
                return ERR_LEX;
			//TODO
		}
	}

	//fprintf(stderr,"Bez chyby\n");
	return OK;
}


char *AddChar(char *string, char c) {
	int i = strlen(string) + 1;
	char *new_string = (char *) realloc (string, (strlen(string) + 2) * sizeof(char));
	
	if (new_string == NULL)
		return NULL;
	new_string[strlen(string)] = c;
	new_string[strlen(string) + 1] = '\0';
	int sum = i - strlen(string);
	new_string[strlen(string) - 1 + sum] = c;
	new_string[strlen(string) - 1 + 1 + sum] = '\0';
	return new_string;
}

char *StringInit() {
	char *new_string;
	if ((new_string = (char *) malloc (sizeof(char))) == NULL)
		return NULL;
	new_string[0] = '\0';
	return new_string;
}