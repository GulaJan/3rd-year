#include "expressions.h"
#include "parser.h"
#include "errs.h"
#include "symtab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
STitem *addedItem;
int runId,  scopeFound = 0, paramsNum = 0, decParamsNum = 0 , skipElse = true, retFlag = false, globIfFlag = 0, elseCnt = 0, endifCnt=0, eol_counter = 0;
int topLevelTrigger(FILE *f) {
	/* This function will trigger our whole syntactic analyse , it is a starting point of the algorithm. */
	// infinite loop , breaked when TOK_EOF comes
	int tokNum, result;
	char *buffer,
		 *idStorage,
		 *frameName;
	STinit(&GST);
	// BUILTINS
    func_name = NULL;
	addedItem = STAdd(&GST, "substr", 1);
	addedItem->item_type = 1;
	addedItem->item_data.function->name = "substr";
	addedItem->item_data.function->data_type = string;
	addedItem->item_data.function->declared = 1;
	addedItem->item_data.function->initialized = 1;
	addedItem->item_data.function->num_of_args = 3;
	addedItem->item_data.function->funcFrame = 1;
	addedItem->item_data.function->emptyRet = 0;
	addedItem->item_data.function->declare_call = 0;

	
	addedItem = STAdd(&GST, "\045print_temp_variable", 0);
	addedItem->item_type = 0;
	addedItem->item_data.variable.varFrame = 1;
	addedItem->item_data.variable.name = "\045print_temp_variable";
	addedItem->item_data.variable.data_type = integer;
	instr_generator(IO_VAR_DEF, "\045print_temp_variable", NULL, NULL, 1, 1, 0);
	instr_generator(IO_ASS, "\045print_temp_variable", "0", NULL, 1, 1, 0);
	addedItem->item_data.variable.initialized = 1;
	addedItem->item_data.variable.declared = 1;

	addedItem = STAdd(&GST, "\045print_temp_variable1", 0);
	addedItem->item_type = 0;
	addedItem->item_data.variable.varFrame = 1;
	addedItem->item_data.variable.name = "\045print_temp_variable1";
	addedItem->item_data.variable.data_type = d_double;
	instr_generator(IO_VAR_DEF, "\045print_temp_variable1", NULL, NULL, 2, 1, 0);
	instr_generator(IO_ASS, "\045print_temp_variable1", "0.0", NULL, 2, 1, 0);
	addedItem->item_data.variable.initialized = 1;
	addedItem->item_data.variable.declared = 1;

	addedItem = STAdd(&GST, "\045print_temp_variable2", 0);
	addedItem->item_type = 0;
	addedItem->item_data.variable.varFrame = 1;
	addedItem->item_data.variable.name = "\045print_temp_variable2";
	addedItem->item_data.variable.data_type = string;
	instr_generator(IO_VAR_DEF, "\045print_temp_variable2", NULL, NULL, 3, 1, 0);
	instr_generator(IO_ASS, "\045print_temp_variable2", "", NULL, 3, 1, 0);
	addedItem->item_data.variable.initialized = 1;
	addedItem->item_data.variable.declared = 1;


	instr_generator(IO_JUMP, "\045scope", NULL, NULL, 1, 0, 0);


	addedItem = STAdd(&GST, "length", 1);
	addedItem->item_type = 1;
	addedItem->item_data.function->name = "length";
	addedItem->item_data.function->data_type = integer;
	addedItem->item_data.function->declared = 1;
	addedItem->item_data.function->initialized = 1;
	addedItem->item_data.function->num_of_args = 1;
	addedItem->item_data.function->funcFrame = 1;
	addedItem->item_data.function->emptyRet = 0;
	addedItem->item_data.function->declare_call = 0;
	STinit(&(addedItem->item_data.function->local_table));
	Param_Add(addedItem, "s", 1, string);
	STitem *s = STAdd(&(addedItem->item_data.function->local_table), addedItem->item_data.function->args[0].param_name, 0);
	s->item_data.variable.name = "s";
	s->item_data.variable.declared = 1;
	s->item_data.variable.data_type = string;
	s->item_data.variable.varFrame = 2;
	instr_generator(IO_LABEL, "\045length", NULL, NULL, 0, 0, 0);
	instr_generator(IO_PUSH_FRAME, NULL, NULL, NULL, 0, 0, 0);
	instr_generator(IO_LEN, "\045print_temp_variable", "s", NULL, 4, 1, 2);
	instr_generator(IO_PUSHS, "\045print_temp_variable", NULL, NULL, 4, 1, 0);
	instr_generator(IO_POP_FRAME, NULL, NULL, NULL, 0, 0, 0);
	instr_generator(IO_RET, NULL, NULL, NULL, 0, 0, 0);

	addedItem = STAdd(&GST, "chr", 1);
	addedItem->item_type = 1;
	addedItem->item_data.function->name = "chr";
	addedItem->item_data.function->data_type = string;
	addedItem->item_data.function->declared = 1;
	addedItem->item_data.function->initialized = 1;
	addedItem->item_data.function->num_of_args = 1;
	addedItem->item_data.function->funcFrame = 1;
	addedItem->item_data.function->emptyRet = 0;
	addedItem->item_data.function->declare_call = 0;
	Param_Add(addedItem, "i", 1, integer);
	s = STAdd(&(addedItem->item_data.function->local_table), addedItem->item_data.function->args[0].param_name, 0);
	s->item_data.variable.name = "i";
	s->item_data.variable.declared = 1;
	s->item_data.variable.data_type = integer;
	s->item_data.variable.varFrame = 2;
	instr_generator(IO_LABEL, "\045chr", NULL, NULL, 0, 0, 0);
	instr_generator(IO_PUSH_FRAME, NULL, NULL, NULL, 0, 0, 0);
	instr_generator(IO_INT2CHAR, "\045print_temp_variable2", "i", NULL, 4, 1, 2);
	instr_generator(IO_PUSHS, "\045print_temp_variable2", NULL, NULL, 4, 1, 0);
	instr_generator(IO_POP_FRAME, NULL, NULL, NULL, 0, 0, 0);
	instr_generator(IO_RET, NULL, NULL, NULL, 0, 0, 0);

	addedItem = STAdd(&GST, "asc", 1);
	addedItem->item_type = 1;
	addedItem->item_data.function->name = "asc";
	addedItem->item_data.function->data_type = integer;
	addedItem->item_data.function->declared = 1;
	addedItem->item_data.function->initialized = 1;
	addedItem->item_data.function->num_of_args = 2;
	addedItem->item_data.function->funcFrame = 1;
	addedItem->item_data.function->emptyRet = 0;
	addedItem->item_data.function->declare_call = 0;
	Param_Add(addedItem, "s", 1, string);
	Param_Add(addedItem, "i", 2, integer);
	s = STAdd(&(addedItem->item_data.function->local_table), addedItem->item_data.function->args[0].param_name, 0);
	s->item_data.variable.name = "s";
	s->item_data.variable.declared = 1;
	s->item_data.variable.data_type = string;
	s->item_data.variable.varFrame = 2;
	s = STAdd(&(addedItem->item_data.function->local_table), addedItem->item_data.function->args[1].param_name, 0);
	s->item_data.variable.name = "i";
	s->item_data.variable.declared = 1;
	s->item_data.variable.data_type = integer;
	s->item_data.variable.varFrame = 2;
	instr_generator(IO_LABEL, "\045asc", NULL, NULL, 0, 0, 0);
	instr_generator(IO_PUSH_FRAME, NULL, NULL, NULL, 0, 0, 0);
	instr_generator(IO_LEN, "\045print_temp_variable", "s", NULL, 4, 1, 2);
    instr_generator(IO_PUSHS, "i", NULL, NULL, 4, 2, 0);
    instr_generator(IO_PUSHS, "1", NULL, NULL, 1, 0, 0);
    instr_generator(IO_L, NULL, NULL, NULL, 0, 0, 0);
    instr_generator(IO_PUSHS, "i", NULL, NULL, 4, 2, 0);
    instr_generator(IO_PUSHS, "\045print_temp_variable", NULL, NULL, 4, 1, 0);
    instr_generator(IO_G, NULL, NULL, NULL, 0, 0 ,0);
    instr_generator(IO_OR_S, NULL, NULL, NULL, 0, 0 ,0);
    instr_generator(IO_PUSHS, "1", NULL, NULL, 5, 0, 0);
    instr_generator(IO_JUMP_IF_NEQ_S, "\045asclabel", NULL, NULL, 0, 0, 0);
    instr_generator(IO_PUSHS, "0", NULL, NULL, 1, 0, 0);
    instr_generator(IO_POP_FRAME, NULL, NULL, NULL, 0, 0, 0);  
     instr_generator(IO_RET, NULL, NULL, NULL, 0, 0, 0);  

    instr_generator(IO_LABEL, "\045asclabel", NULL, NULL, 0, 0, 0);
    instr_generator(IO_PUSHS, "i", NULL, NULL, 4, 2, 0);
    instr_generator(IO_PUSHS, "1", NULL, NULL, 1, 0, 0);
    instr_generator(IO_SUBS, NULL, NULL, NULL, 0, 0, 0); 
    instr_generator(IO_POPS, "i", NULL, NULL, 4, 2, 0);      // -1 lebo tato instrukcia indexuje od 0 a normalne indexuju od 1
    instr_generator(IO_STRI2INT, "\045print_temp_variable" , "s", "i", 4, 1, 2);
	instr_generator(IO_PUSHS, "\045print_temp_variable", NULL, NULL, 4, 1, 0);
	instr_generator(IO_POP_FRAME, NULL, NULL, NULL, 0, 0, 0);
	instr_generator(IO_RET, NULL, NULL, NULL, 0, 0, 0);
	
	while(1) {
		tokNum = Token(&buffer, f);
		switch(tokNum) {
			case KW_SCOPE:
				eol_counter++;
				instr_generator(IO_LABEL, "\045scope", NULL, NULL, 0, 0, 0);

				if((frameName = ((char *)malloc(100 * sizeof(char)))) == NULL){
					//fprintf(stderr, "Insufficient memory\n");
					return ERR_OTHER;
				}
				scopeFound = 1;
				
				if((STFind(&GST,"scope")) == NULL)
					addedItem = STAdd(&GST, "scope", 1);
				else{
					//fprintf(stderr, "Scope redeclaration ! Return code 2.\n");
					return 63;
				}
				addedItem->item_type = 1;
				addedItem->item_data.function->name = buffer;
				addedItem->item_data.function->data_type = integer;
				addedItem->item_data.function->declared = 1;
				addedItem->item_data.function->num_of_args = 0;
				addedItem->item_data.function->funcFrame = 1;
				addedItem->item_data.function->emptyRet = 0;
				addedItem->item_data.function->num_of_args = 1;
				addedItem->item_data.function->declare_call = 0;
				result = commandsCycle(0, 0, 0, 0, f, "scope", 0);

				
				if(result != OK)
					return result;
				frameName = NULL;
				
				free(frameName);
				
			break;
			case KW_DECLARE:
				eol_counter++;
				if (((tokNum = Token(&buffer, f)) != ERR_LEX) && (tokNum == KW_FUNCTION)){
					if (((tokNum = Token(&buffer, f)) != ERR_LEX) && (tokNum == TOK_ID)){
						if((idStorage = (char *) malloc((strlen(buffer) + 1) * sizeof(char))) == NULL){
							return 99;
						}
						strcpy(idStorage, buffer);
						addedItem = STFind(&GST, idStorage);
						if (addedItem != NULL)
							return ERR_SEM;
						if (((tokNum = Token(&buffer, f)) != ERR_LEX) && (tokNum == TOK_LEFT_RB)){ 
							
							addedItem = STAdd(&GST, idStorage, 1);
							result = ListOfParams(f, idStorage, 1);
							//fprintf(stderr, "Num of params %d\n", paramsNum);
							if (result == OK){
								if (((tokNum = Token(&buffer, f)) != ERR_LEX) && (tokNum == KW_AS)){
									//fprintf(stderr,"Pohoda\n");
									if (((tokNum = Token(&buffer, f)) != ERR_LEX) || (tokNum == KW_INTEGER) || (tokNum == KW_STRING) || (tokNum == KW_DOUBLE)){
										if (tokNum == KW_INTEGER)
											addedItem->item_data.function->data_type = integer;
										else if (tokNum == KW_DOUBLE)
											addedItem->item_data.function->data_type = d_double;
										else
											addedItem->item_data.function->data_type = string;
										addedItem->item_data.function->num_of_args = paramsNum;
										int i = 0;
										STitem *helpArr;
										func_name = STFind(&GST, idStorage);
										if(!(addedItem->item_data.function->initialized)) {
											STinit(&(func_name->item_data.function->local_table));
											while(i < addedItem->item_data.function->num_of_args) {
												if((STFind(&(func_name->item_data.function->local_table), addedItem->item_data.function->args[i].param_name)) != NULL){
													//fprintf(stderr, "PARAM REDECLARATION ! \n");
													return 64;
												}
												helpArr = STAdd(&(func_name->item_data.function->local_table), addedItem->item_data.function->args[i].param_name, 0);
												helpArr->item_type = 0;
												helpArr->item_data.variable.varFrame = 2;
												helpArr->item_data.variable.name = addedItem->item_data.function->args[i].param_name;
												helpArr->item_data.variable.data_type = addedItem->item_data.function->args[i].param_type;
												helpArr->item_data.variable.declared = 1;							
												helpArr->item_data.variable.initialized = 1;						
												i++;	
											}
											i = 0;
										}

										addedItem->item_type = 1;
										addedItem->item_data.function->name = idStorage;
										addedItem->item_data.function->funcFrame = 2;
										addedItem->item_data.function->num_of_args = paramsNum;
										addedItem->item_data.function->emptyRet = 0;
										addedItem->item_data.function->declare_call = 0;
						
										paramsNum = 0;						

										if(tokNum == KW_INTEGER)
											addedItem->item_data.function->dec_data_type = integer;
										else if(tokNum == KW_DOUBLE)
											addedItem->item_data.function->dec_data_type = d_double;
										else
											addedItem->item_data.function->dec_data_type = string;


									}
									if (tokNum == ERR_LEX){
									//fprintf(stderr, "Declare Lexierr1\n");							
									return ERR_LEX;
									}
								}
								if (tokNum == ERR_LEX){
									//fprintf(stderr, "Declare Lexierr2\n");																
									return ERR_LEX;
								}
							}
							else if (result == ERR_LEX){
								//fprintf(stderr, "Declare Lexierr3\n");							
								return ERR_LEX;
							}
							else{
								//fprintf(stderr, "Declare synerr\n");							
								return result;
							}
						}
						if (tokNum == ERR_LEX){
							//fprintf(stderr, "Declare Lexierr4\n");							
							return ERR_LEX;
						}
					}
					if (tokNum == ERR_LEX){
						//fprintf(stderr, "Declare Lexierr5\n");							
						return ERR_LEX;
					}	
				}
				if (tokNum == ERR_LEX){
					//fprintf(stderr, "Declare Lexierr6\n");							
					return ERR_LEX;
				} else if (tokNum == ERR_SYN){
					//fprintf(stderr, "Declare synerr2\n");											
					return ERR_SYN;
				}
				//fprintf(stderr,"na breaku\n");
				break;
			case KW_FUNCTION:
				eol_counter++;
				//fprintf(stderr,"Telo funkcie\n");
				if (((tokNum = Token(&buffer, f)) != ERR_LEX) && (tokNum == TOK_ID)){
					if((idStorage = (char *) malloc((strlen(buffer) + 1) * sizeof(char))) == NULL){
						//fprintf(stderr, "Insufficient memory space!\n");
						return 99;
					}					
					if((frameName = ((char *)malloc((strlen(buffer) + 1) * sizeof(char)))) == NULL){
						//fprintf(stderr, "Insufficient memory\n");
						return ERR_OTHER;
					}
					char *help;
					if((help = (char *) malloc((strlen(buffer) + 1) * sizeof(char))) == NULL){
						//fprintf(stderr, "Insufficient memory space!\n");
						return 99;
					}					
					sprintf(help, "%%%s", buffer);
					strcpy(idStorage, buffer);
					strcpy(frameName, buffer);
					//fprintf(stderr,"Frame name je %s\n", frameName);
					instr_generator(IO_LABEL, help, NULL, NULL, 0, 0, 0);
					if(((tokNum = Token(&buffer, f)) != ERR_LEX) && (tokNum == TOK_LEFT_RB)){
						if((addedItem = STFind(&GST, idStorage)) == NULL) {
							addedItem = STAdd(&GST, idStorage, 1);
						} else {
							if(addedItem->item_data.function->initialized){
								//fprintf(stderr, "FUNC REDECLARATION ! %s\n", idStorage);
								return 64;
							}
						}
						func_name = STFind(&GST, idStorage);
						result = ListOfParams(f, idStorage, 0);
						if(result != OK)
							return result;
						//fprintf(stderr,"Pohoda2\n");
						int cnt = 0;
						if(addedItem->item_data.function->declared){
							if(decParamsNum != paramsNum){
								fprintf(stderr, "WRONG NUMBER OF ARGUMENTS RETURN CODE 4! cnt %d par %d \n", cnt, paramsNum);
								return 65;
							}
							cnt = 0;
						}
						//fprintf(stderr,"Pohoda3\n");
						addedItem->item_data.function->num_of_args = paramsNum;
						//fprintf(stderr,"Pohoda3\n");
						int i = 0;
						STitem *helpArr;
						if(!(addedItem->item_data.function->declared)){
							STinit(&(func_name->item_data.function->local_table));
							while(i < addedItem->item_data.function->num_of_args) {
								helpArr = STAdd(&(func_name->item_data.function->local_table), addedItem->item_data.function->args[i].param_name, 0);
								helpArr->item_type = 0;
								helpArr->item_data.variable.varFrame = 2;
								helpArr->item_data.variable.name = addedItem->item_data.function->args[i].param_name;
								helpArr->item_data.variable.data_type = addedItem->item_data.function->args[i].param_type;
								helpArr->item_data.variable.declared = 1;							
								helpArr->item_data.variable.initialized = 1;						
								i++;	
							}
							i = 0;
						}
						else {
							while(i < addedItem->item_data.function->num_of_args) {
								helpArr = STAdd(&(func_name->item_data.function->local_table), addedItem->item_data.function->args[i].param_name, 0);
								helpArr->item_type = 0;
								helpArr->item_data.variable.varFrame = 2;
								helpArr->item_data.variable.name = addedItem->item_data.function->args[i].param_name;
								helpArr->item_data.variable.data_type = addedItem->item_data.function->args[i].param_type;
								helpArr->item_data.variable.declared = 1;							
								helpArr->item_data.variable.initialized = 1;						
								i++;
								//fprintf(stderr,"Meno zmeny je %s\n",helpArr->item_data.variable.name );	
							}
							i = 0;

						}
						if (addedItem->item_data.function->declare_call == 1) {
							while(i < addedItem->item_data.function->num_of_args) {
								if (addedItem->item_data.function->args[i].param_type == integer) {
								instr_generator(IO_VAR_DEF,  addedItem->item_data.function->args[i].param_name, NULL, NULL, 1, 3, 0);
								instr_generator(IO_ASS, addedItem->item_data.function->args[i].param_name, addedItem->item_data.function->params[i].param_name, NULL,4, 3, 3);
								}
								else if (addedItem->item_data.function->args[i].param_type == d_double) {
								instr_generator(IO_VAR_DEF,  addedItem->item_data.function->args[i].param_name, NULL, NULL, 2, 3, 0);
								instr_generator(IO_ASS, addedItem->item_data.function->args[i].param_name, addedItem->item_data.function->params[i].param_name, NULL, 4, 3, 3);
							}
								else if (addedItem->item_data.function->args[i].param_type == string) {
								instr_generator(IO_VAR_DEF,  addedItem->item_data.function->args[i].param_name, NULL, NULL, 3, 3, 0);
								instr_generator(IO_ASS, addedItem->item_data.function->args[i].param_name, addedItem->item_data.function->params[i].param_name, NULL, 4, 3, 3);
							}
							i++;
						}
						i = 0;
					}
						instr_generator(IO_PUSH_FRAME, NULL, NULL, NULL, 0, 0, 0);
						//fprintf(stderr, "Num of params %d\n", paramsNum);
						paramsNum = 0;						
						if (result == OK){
							if (((tokNum = Token(&buffer, f)) != ERR_LEX) && (tokNum == KW_AS)){
								if (((tokNum = Token(&buffer, f)) != ERR_LEX) && ((tokNum == KW_INTEGER) || (tokNum == KW_STRING) || (tokNum == KW_DOUBLE))){
									addedItem->item_data.function->funcFrame = 1;
									addedItem->item_type = 1;
									addedItem->item_data.function->name = idStorage;
									addedItem->item_data.function->emptyRet = 0;
									
									if(tokNum == KW_INTEGER){
										if (addedItem->item_data.function->declared == 1) {
											if (addedItem->item_data.function->data_type != integer)
												return ERR_SEM_TYPE;
										}
										addedItem->item_data.function->data_type = integer;
									}
									else if(tokNum == KW_DOUBLE){
										if (addedItem->item_data.function->declared == 1) {
											if (addedItem->item_data.function->data_type != d_double)
												return ERR_SEM_TYPE;
										}
										addedItem->item_data.function->data_type = d_double;
									}
									else if (tokNum == KW_STRING) {
										if (addedItem->item_data.function->declared == 1) {
											if (addedItem->item_data.function->data_type != string)
												return ERR_SEM_TYPE;
										}
										addedItem->item_data.function->data_type = string;
									}
									else
										return ERR_SYN;
									if(addedItem->item_data.function->declared != 1)
										addedItem->item_data.function->declared = 0;
									addedItem->item_data.function->initialized = 1;
									//fprintf(stderr, "Frame name je %s\n", frameName);
									result = commandsCycle(0, 0, 0, 1, f, frameName, 0);
									if(result != OK){
										//fprintf(stderr, "commandsCycle in function failed!\n");
										return result;
									} else 
										break;
								}
								if (tokNum == ERR_LEX){
									//fprintf(stderr, "Function Lexierr1\n");							
									return ERR_LEX;
								}
							}
							if (tokNum == ERR_LEX){
							//	fprintf(stderr, "Function Lexierr2\n");							
								return ERR_LEX;
							}
						}
						else if (result == ERR_LEX){
							//fprintf(stderr, "Function Lexierr3\n");							
							return ERR_LEX;
						}
						else{
							//fprintf(stderr, "Function err1\n");							
							return result;
						}
					}
				}
				if (tokNum == ERR_LEX){
					//fprintf(stderr, "Lexierr1\n");							
					return ERR_LEX;
				}
				frameName = NULL;
				free(frameName);
			break;
			case TOK_EOF:
				if (eol_counter == 0)
					return ERR_SYN;
				return OK;
			case TOK_EOL:
				break;
			default:
				//fprintf(stderr, "Unknown start of a syntactic construction tu %d\n",tokNum);
				if(tokNum == 62)
					return ERR_LEX;
				return ERR_SYN;
		}
	}
	return 0;
}
// PARAMETER NEMOZE BYT LOKALNA PREMENNA
// DEKLARACIE SA MUSI ZHODOVA S DEFINICIOU

int commandsCycle(int whileFlag, int ifFlag, int elseFlag, int functionFlag, FILE *f, char* frameNameLoc, int while_counter) {
	int breakWhileFlag = 0, tokNum = 0, result = 0;
	static int whileCnt;
	char *parseBuffer,
		 *idStorage,
		 *idStorageTwo;
	while(!breakWhileFlag) {
		//fprintf(stderr,"Dim\n");
		tokNum = Token(&parseBuffer, f);
		//fprintf(stderr,"Fasa\n");
		//fprintf(stderr, "Token v commands je %s\n", parseBuffer);
		switch(tokNum) {
			case KW_DIM :
				//fprintf (stderr,"Frame loc je %s\n", frameNameLoc);
				if ((result = varDefinition(f, functionFlag, frameNameLoc)) != OK)
					return result;
				break;
			case TOK_ID :
				if((idStorage = (char *) malloc((strlen(parseBuffer) + 1) * sizeof(char))) == NULL){
					//fprintf(stderr, "Insufficient memory space!\n");
					return 99;
				}
				strcpy(idStorage, parseBuffer);
				if (scopeFound) 
					addedItem = STFind(&GST, idStorage);
				else
					addedItem = STFind(&(func_name->item_data.function->local_table), idStorage);
				if (addedItem == NULL)
					return ERR_SEM;
				else if (addedItem->item_type != variable)
					return ERR_SEM;
				tokNum = Token(&parseBuffer, f);
				if (tokNum != TOK_ASSIGN_OR_EQUALS){
					//fprintf(stderr, "An assignment must follow ID!\n");							
					return ERR_SYN;
				} else {
					tokNum = Token(&parseBuffer, f);
					if (tokNum == ERR_LEX)
						return ERR_LEX;
					result = expressionAnalysis(f, tokNum, parseBuffer, 1, idStorage, 0, 0);
					if(result != OK){
					//fprintf(stderr, "expressionAnalysis failed !\n");
					return result;
					}
				}
				break;
			case KW_INPUT:
				tokNum = Token(&parseBuffer, f);
				if (tokNum != TOK_ID){
					//fprintf(stderr, "Expected ID after INPUT!\n");							
					return ERR_SYN;				
				} else {
					if(scopeFound)
						addedItem = STFind(&GST, parseBuffer);
					else
						addedItem = STFind(&(func_name->item_data.function->local_table), parseBuffer);

					if(addedItem != NULL){
						int frame = addedItem->item_data.variable.varFrame;
						if(addedItem->item_data.variable.data_type == integer){
							instr_generator(IO_INPUT, parseBuffer, NULL, NULL, 1, frame, 0);
						}
						else if(addedItem->item_data.variable.data_type == d_double){
							instr_generator(IO_INPUT, parseBuffer, NULL, NULL, 2, frame, 0);
						}
						else{
							instr_generator(IO_INPUT, parseBuffer, NULL, NULL, 3, frame, 0);
						}
					} else {
						//fprintf(stderr, "INPUT FAILED!\n");	
						return 64;						
					}
				}
				break;
			case KW_PRINT:
				if ((result = printAnalysis(f)) != 0){
					return result;
				}
				break;
			case KW_RETURN:
				if(functionFlag){
					//fprintf(stderr,"\nDOSOL?%s\n", frameNameLoc);
					tokNum = Token(&parseBuffer, f);
					if (tokNum == ERR_LEX)
						return ERR_LEX;
					switch(tokNum) {
						case TOK_ID :
								//fprintf(stderr,"TOT JE FRAMELOC : %s\n", frameNameLoc);
								//fprintf(stderr,"\n%s\n", frameNameLoc);
								addedItem = STFind(&GST, frameNameLoc);
								if(addedItem == NULL){
									//fprintf(stderr, "FUNCTION NOT FOUND!\n");
									return ERR_SEM;
								}

								result = expressionAnalysis(f, tokNum, parseBuffer, 2, NULL, 0, addedItem->item_data.function->data_type + 1);
								if(result != OK){
									//fprintf(stderr, "expressionAnalysis failed !\n");
									return result;
								}
								instr_generator(IO_POP_FRAME,NULL, NULL, NULL, 0, 0, 0);
								instr_generator(IO_RET,NULL, NULL, NULL, 0, 0, 0);
							break;
						case TOK_NUMBER :
							if(!(retFlag)){
								addedItem = STFind(&GST, frameNameLoc);
								if(addedItem == NULL){
									//fprintf(stderr, "FUNCTION NOT FOUND!\n");
									return ERR_SEM;
								}
								result = expressionAnalysis(f, tokNum, parseBuffer, 2, NULL, 0, addedItem->item_data.function->data_type + 1);
								if(result != OK){
									//fprintf(stderr, "expressionAnalysis failed !\n");
									return result;
								}
								instr_generator(IO_POP_FRAME,NULL, NULL, NULL, 0, 0, 0);
								instr_generator(IO_RET,NULL, NULL, NULL, 0, 0, 0);
								return OK;
							}
							break;
						case TOK_LEFT_RB :
							addedItem = STFind(&GST, frameNameLoc);
							if(addedItem == NULL){
								//fprintf(stderr, "FUNCTION NOT FOUND!\n");
								return ERR_SEM;
							}
							result = expressionAnalysis(f, tokNum, parseBuffer, 2, NULL, 0, addedItem->item_data.function->data_type + 1);
							if(result != OK){
								//fprintf(stderr, "expressionAnalysis failed !\n");
								return result;
							}
							instr_generator(IO_POP_FRAME,NULL, NULL, NULL, 0, 0, 0);
						    instr_generator(IO_RET,NULL, NULL, NULL, 0, 0, 0);
						break;
						case TOK_STRING_STREAM :
							addedItem = STFind(&GST, frameNameLoc);
							if(addedItem == NULL){
								//fprintf(stderr, "FUNCTION NOT FOUND!\n");
								return ERR_SEM;
							}
							result = expressionAnalysis(f, tokNum, parseBuffer, 2, NULL, 0, addedItem->item_data.function->data_type + 1);
							if(result != OK){
								//fprintf(stderr, "expressionAnalysis failed !\n");
								return result;
							}
							instr_generator(IO_POP_FRAME,NULL, NULL, NULL, 0, 0, 0);
						    instr_generator(IO_RET,NULL, NULL, NULL, 0, 0, 0);
							break;
						case TOK_INT:
							addedItem = STFind(&GST, frameNameLoc);
							if(addedItem == NULL){
								//fprintf(stderr, "FUNCTION NOT FOUND!\n");
								return ERR_SEM;
							}
							result = expressionAnalysis(f, tokNum, parseBuffer, 2, NULL, 0, addedItem->item_data.function->data_type + 1);
							if(result != OK){
								//fprintf(stderr, "expressionAnalysis failed !\n");
								return result;
							}
							instr_generator(IO_POP_FRAME,NULL, NULL, NULL, 0, 0, 0);
							instr_generator(IO_RET,NULL, NULL, NULL, 0, 0, 0);
							break;
						default :
							//fprintf(stderr, "Return without a value! %d \n",tokNum);
							return ERR_SYN;	
					}
					retFlag = true;
				} else {
					//fprintf(stderr, "Cannot use return out of a function!\n");
					return ERR_SYN;
				}
				break;
			case KW_IF:
				globIfFlag = 1;
				tokNum = Token(&parseBuffer, f);
				if (tokNum == ERR_LEX)
						return ERR_LEX;
				if((idStorage = (char *) malloc(100 * sizeof(char))) == NULL){
					//fprintf(stderr, "No memory!\n");
					return ERR_OTHER;
				}
				result = expressionAnalysis(f, tokNum, parseBuffer, 0, NULL, 1, 0);
				if(result != OK){
					//fprintf(stderr, "expressionAnalysis failed !\n");
					return result;
				}
				if((tokNum = Token(&parseBuffer, f)) != TOK_EOL){
					//fprintf(stderr, "Missing END OF LINE after THEN statement!\n");					
					return ERR_SYN;
				}
				sprintf(idStorage, "%%else%d", elseCnt);
				instr_generator(IO_PUSHS, "1", NULL, NULL, 5, 0, 0);
				instr_generator(IO_JUMP_IF_NEQ_S, idStorage, NULL, NULL, 0, 0, 0);
				if(functionFlag){
					if((result = commandsCycle(0, 1, 0, 1, f, frameNameLoc, 0)) != OK) 
						return result;
				} else {
					if((result = commandsCycle(0, 1, 0, 0, f, frameNameLoc, 0)) != OK) 
						return result;
				}
				if((idStorageTwo = ((char *) malloc(10*sizeof(char)))) == NULL) {
					//fprintf(stderr, "NOT ENOUGH MEMORY!\n");
					return ERR_OTHER;
				}
				sprintf(idStorageTwo, "%%endif%d",endifCnt); // ZLE PISE ENDIF1 Z NEJAKEHO DOVODU
				instr_generator(IO_JUMP, idStorageTwo, NULL, NULL, 0, 0, 0);								
				sprintf(idStorage, "%%else%d", elseCnt++);
				instr_generator(IO_LABEL, idStorage, NULL, NULL, 0, 0, 0);	
				if ((tokNum = Token(&parseBuffer, f)) != TOK_EOL) {
					if (tokNum == ERR_LEX){
						//fprintf(stderr, "Lex ERR in Else\n");							
						return ERR_LEX;
					}
					else{
						//fprintf(stderr, "Syn ERR in Else!\n");							
						return ERR_SYN;
					}
				}
				//fprintf(stderr, "f1 %d f2 %d f3 %d f4 %d \n", whileFlag, ifFlag, elseFlag, functionFlag);
				if(!(skipElse)){
					if(functionFlag){
						if ((result = commandsCycle(0, 0, 1, 1, f, frameNameLoc, 0)) != OK) {
							if (result == ERR_LEX){
								//fprintf(stderr, "Commands cycle after else returned err LEX!\n");							
								return ERR_LEX;
							}
							else{
								//fprintf(stderr, "Commands cycle after else returned err SYN!\n");							
								return ERR_SYN;
							}
						}
					} else {
						if ((result = commandsCycle(0, 0, 1, 0, f, frameNameLoc, 0)) != OK) {
							if (result == ERR_LEX){
								//fprintf(stderr, "Commands cycle after else returned err LEX!\n");							
								return ERR_LEX;
							}
							else{
								//fprintf(stderr, "Commands cycle after else returned err SYN!\n");							
								return ERR_SYN;
							}
						}						
					}
				}
				skipElse = true;
				break;
			case KW_DO:
				idStorage = (char *) malloc(8 * sizeof(char));
				sprintf(idStorage, "%%while%d", whileCnt++);
				instr_generator(IO_LABEL, idStorage, NULL, NULL, 0, 0, 0);
				if ((result = whileAnalysis(f, frameNameLoc, whileCnt - 1)) != OK) {
					if (result == ERR_LEX){
						//fprintf(stderr, "Lex ERR in DO!\n");							
						return ERR_LEX;
					}
					else if (result == ERR_SYN){
						//fprintf(stderr, "Syn ERR in DO!\n");							
						return ERR_SYN;
					}
					else
						break;
				}
				break;	
			case KW_END:
				tokNum = Token(&parseBuffer, f);
				//fprintf(stderr,"END\n");
				switch(tokNum) {
					case KW_IF :
						if((whileFlag == 0) && ((ifFlag == 1) || (elseFlag == 1))){
							sprintf(parseBuffer, "%%endif%d", endifCnt++);
							instr_generator(IO_LABEL, parseBuffer, NULL, NULL, 0, 0, 0);	
							globIfFlag = 0;													
							return OK;
						} else if ((functionFlag == 0) && (whileFlag == 0) && (ifFlag == 0) && (elseFlag == 0) && (globIfFlag)){
							sprintf(parseBuffer, "%%endif%d", endifCnt++);
							instr_generator(IO_LABEL, parseBuffer, NULL, NULL, 0, 0, 0);	
							globIfFlag = 0;													
							break;
						} else {
							//fprintf(stderr, "END IF outside of an IF construction\n %d %d %d %d", whileFlag, ifFlag, elseFlag, functionFlag);
							return ERR_SYN;
						}
						break;
					case KW_SCOPE :
						if((scopeFound == 1) && (whileFlag == 0) && (ifFlag == 0) && (elseFlag == 0)){
							scopeFound = 0;
							//fprintf(stderr,"Pohoda\n");
							return OK;
						}
						else{
							//fprintf(stderr, "END SCOPE outside of a SCOPE construction\n");							
							return ERR_SYN;
						}
						break;
					case KW_FUNCTION :
						if((scopeFound == 0) && (whileFlag == 0) && (ifFlag == 0) && (elseFlag == 0) && (functionFlag == 1)){
							instr_generator(IO_POP_FRAME, NULL, NULL, NULL, 0, 0, 0);							
							if(!retFlag) {
								if((addedItem = STFind(&GST, frameNameLoc)) != NULL){
									if((addedItem->item_data.function->data_type) == 0)
										instr_generator(IO_PUSHS ,"0",NULL, NULL, 1, 0, 0);
									else if((addedItem->item_data.function->data_type) == 1)
										instr_generator(IO_PUSHS ,"0.0",NULL, NULL, 2, 0, 0);
									else if((addedItem->item_data.function->data_type) == 2){
										addedItem->item_data.function->emptyRet = 1;	
										instr_generator(IO_PUSHS ,"",NULL, NULL, 3, 0, 0);
									}
								} else {
									//fprintf(stderr, "Func name not found !%s\n", frameNameLoc);
									return 65;
								}
							}
							instr_generator(IO_RET ,NULL ,NULL, NULL, 0, 0, 0);
							retFlag = false;
							func_name = NULL;
							return OK;
						}
						else{
							//fprintf(stderr, "END FUNCTION outside of a FUNCTION!\n");							
							return ERR_SYN;
						}
						break;
					default :
						//fprintf(stderr, "Keyword END followed by an unknown token!\n");
						return ERR_SYN;	
				}		
				break;
			case KW_ELSE:
				if((whileFlag == 0) && (ifFlag == 1) && (elseFlag == 0)){
					skipElse = false;
					return OK;
				}
				else{
					//fprintf(stderr, "Else with previous if\n");
					return ERR_SYN;
				}
				break;
			case KW_LOOP:
				idStorage = (char *) malloc(8 * sizeof(char));
				sprintf(idStorage, "%%while%d", while_counter);
				instr_generator(IO_JUMP, idStorage, NULL, NULL, 0, 0, 0);
				idStorage = (char *) malloc(8 * sizeof(char));
				sprintf(idStorage, "%%loops%d", while_counter);
				instr_generator(IO_LABEL, idStorage, NULL, NULL, 0, 0, 0);
				if ((whileFlag == 1) && (ifFlag == 0) && (elseFlag == 0)) {
					breakWhileFlag = 1;
					break;
				}	
				else {
					//fprintf(stderr, "An assignment must follow ID! LOOP\n");												
					return ERR_SYN;
				}		
			case TOK_EOL:
				break;

			default:
					//fprintf(stderr, "Unknown start of a command!%d\n",tokNum);
					if(tokNum == 0)
						return 62;
					return ERR_SYN;
		}
	}
	free(parseBuffer);
	parseBuffer = NULL;
	//fprintf(stderr,"pohoda\n");
	return 0;
}

int ListOfParams(FILE *f, char *funcName, int declaration){
	int tok, ret;
	static int commaFound;
	char *buffer,
		 *paramName;

	if ((tok = Token(&buffer, f)) != ERR_LEX){
		switch(tok){
			case TOK_ID:
				commaFound = 1;
				if((paramName = (char *) malloc((strlen(buffer) + 1) * sizeof(char))) == NULL){
					//fprintf(stderr, "Insufficient memory space!\n");
					return 99;
				}
				strcpy(paramName, buffer);				
				if (((tok = Token(&buffer, f)) != ERR_LEX) && (tok == KW_AS)){
					if (((tok = Token(&buffer, f)) != ERR_LEX) && ((tok == KW_STRING) || (tok == KW_DOUBLE) || (tok == KW_INTEGER))){
						addedItem = STFind(&GST, funcName);
						if(addedItem == NULL){
							//fprintf(stderr, "Func used not found in GST\n");
							return ERR_SEM;
						}
						paramsNum++;
						if(declaration)
							decParamsNum++;
						if(tok == KW_STRING){
							if(STFind(&GST, paramName) != NULL){
								//fprintf(stderr, "PARAM IN GST\n");
								return 64;
							}
							//fprintf(stderr, "paramsnum : %d\n", paramsNum);
							if(addedItem->item_data.function->declared == 1){
								//fprintf(stderr, "FITS? : %d\n", addedItem->item_data.function->params[paramsNum-1].param_type);
								if(addedItem->item_data.function->params[paramsNum-1].param_type != 2){
									//fprintf(stderr, "Invalid parameters for fuction %s ! Return code 3\n1p", funcName);
									return 64;
								}
							} else if(declaration){

								Param_Declaration_Add(addedItem, paramName, paramsNum, string);
							}
					   
							Param_Add(addedItem, paramName, paramsNum, string);
						}
						else if(tok == KW_INTEGER){
							if(STFind(&GST, paramName) != NULL){
								//fprintf(stderr, "PARAM IN GST\n");
								return 64;
							}
							if(addedItem->item_data.function->declared == 1){
								if(addedItem->item_data.function->params[paramsNum-1].param_type != 0){
									//fprintf(stderr, "Invalid parameters for fuction %s ! Return code 3\n2p", funcName);
									return 64;
								}
							} else if(declaration){
								Param_Declaration_Add(addedItem, paramName, paramsNum, integer);
							}
		
							Param_Add(addedItem, paramName, paramsNum, integer);
						}
						else if(tok == KW_DOUBLE){
							if(STFind(&GST, paramName) != NULL){
								//fprintf(stderr, "PARAM IN GST\n");
								return 64;
							}
							if(addedItem->item_data.function->declared == 1){
								if(addedItem->item_data.function->params[paramsNum-1].param_type != 1){
									//fprintf(stderr, "Invalid parameters for fuction %s ! Return code 3\n3p", funcName);
									return 64;
								}
							} else if(declaration){
								Param_Declaration_Add(addedItem, paramName, paramsNum, d_double);
							}
							Param_Add(addedItem, paramName, paramsNum, d_double);
						}
						ret = ListOfParams(f, funcName, declaration);
						if (ret == OK)
							return OK;
						else 
							return ret;
					}
					if (tok == ERR_LEX){
						//fprintf(stderr, "Lex ERR in list of params!\n");							
						return ERR_LEX;
					}
				}
				if (tok == ERR_LEX){
					//fprintf(stderr, "Lex ERR in list of params2!\n");												
					return ERR_LEX;
				}
				//fprintf(stderr, "ERR in list of params! %d \n", tok);							
				return ret;
			case TOK_COMMA:
				if(commaFound){
					if ((tok = Token(&buffer, f)) != ERR_LEX){
						switch (tok) {
							case TOK_ID:
								if((paramName = (char *) malloc((strlen(buffer) + 1) * sizeof(char))) == NULL){
									//fprintf(stderr, "Insufficient memory space!\n");
									return 99;
								}
								strcpy(paramName, buffer);
								if (((tok = Token(&buffer, f)) != ERR_LEX) && (tok == KW_AS)){
									if (((tok = Token(&buffer, f)) != ERR_LEX) && ((tok == KW_STRING) || (tok == KW_DOUBLE) || (tok == KW_INTEGER))){
										addedItem = STFind(&GST, funcName);
										if(addedItem == NULL){
											//fprintf(stderr, "Func used not found in GST\n");
											return ERR_SEM;
										}
										paramsNum++;
										if(declaration)
											decParamsNum++;
										if(tok == KW_STRING){
											//fprintf(stderr, "param: %d parNum: %d\n", addedItem->item_data.function->params[paramsNum-1].param_type,paramsNum);
											if(addedItem->item_data.function->declared == 1){
												if(addedItem->item_data.function->params[paramsNum-1].param_type != 2){
													//fprintf(stderr, "Invalid parameters for fuction %s ! Return code 3\n4p", funcName);
													return 64;
												}
											} else if(declaration){
												Param_Declaration_Add(addedItem, paramName, paramsNum, string);
											}
											int pom = paramsNum - 1;
											int pom_tmp = 0;
											int redeclar = 0;
											while (pom_tmp < pom) {
												if ((strcmp(addedItem->item_data.function->args[pom_tmp].param_name, paramName)) == 0)
													redeclar = 1;
												pom_tmp++; 
											}
											if (redeclar == 1)
												return ERR_SEM;
											Param_Add(addedItem, paramName, paramsNum, string);
										}
										else if(tok == KW_INTEGER){
											if(addedItem->item_data.function->declared == 1){
												if(addedItem->item_data.function->params[paramsNum-1].param_type != 0){
													//fprintf(stderr, "Invalid parameters for fuction %s %d %d! Return code 3\n5p", funcName, addedItem->item_data.function->params[paramsNum-2].param_type, paramsNum-2);
													//fprintf(stderr, "Invalid parameters for fuction %s %d %d! Return code 3\n5p", funcName, addedItem->item_data.function->params[paramsNum-1].param_type, paramsNum-1);
													//fprintf(stderr, "Invalid parameters for fuction %s %d %d! Return code 3\n5p", funcName, addedItem->item_data.function->params[paramsNum].param_type, paramsNum);
													return 64;
												}
											} else if(declaration){
												Param_Declaration_Add(addedItem, paramName, paramsNum, integer);
											}
											int pom = paramsNum - 1;
											int pom_tmp = 0;
											int redeclar = 0;
											while (pom_tmp < pom) {
												if ((strcmp(addedItem->item_data.function->args[pom_tmp].param_name, paramName)) == 0)
													redeclar = 1;
												pom_tmp++; 
											}
											if (redeclar == 1)
												return ERR_SEM;
											Param_Add(addedItem, paramName, paramsNum, integer);
										}
										else if(tok == KW_DOUBLE){
											if(addedItem->item_data.function->declared == 1){
												if(addedItem->item_data.function->params[paramsNum-1].param_type != 1){
													//fprintf(stderr, "Invalid parameters for fuction %s ! Return code 3\n6p", funcName);
													return 64;
												}
											} else if(declaration){
												Param_Declaration_Add(addedItem, paramName, paramsNum, d_double);
											}
											int pom = paramsNum - 1;
											int pom_tmp = 0;
											int redeclar = 0;
											while (pom_tmp < pom) {
												if ((strcmp(addedItem->item_data.function->args[pom_tmp].param_name, paramName)) == 0)
													redeclar = 1;
												pom_tmp++; 
											}
											if (redeclar == 1)
												return ERR_SEM;
											Param_Add(addedItem, paramName, paramsNum, d_double);
										}

										ret = ListOfParams(f, funcName, declaration);
										if (ret == OK)
											return OK;
										else 
											return ret;
									}
									if (tok == ERR_LEX){
										//fprintf(stderr, "Lex ERR in list of params!\n");							
										return ERR_LEX;
									}
								}
								if (tok == ERR_LEX){
									//fprintf(stderr, "Lex ERR in list of params2!\n");												
									return ERR_LEX;
								}
								//fprintf(stderr, "ERR in list of params!\n");							
								return tok;
							default:
								//fprintf(stderr, "Comma not followed by ID!\n");
								return ERR_SYN;
						}
					} else {
					//fprintf(stderr, "Lex err end of comma!\n");							
					return ERR_LEX;
					}
				} else {
					return ERR_SYN;
				}
			case TOK_RIGHT_RB:
//				free(buffer);
//				buffer = NULL;
				commaFound = 0;
				if(declaration){
					addedItem = STFind(&GST, funcName);
					addedItem->item_data.function->declared = 1;
				}
				return OK;
			default:
					//fprintf(stderr, "default err in list of params%d!\n",tok);										
					return ret;
		}
	}
	free(buffer);
	buffer = NULL;
	return OK;
}
// Verifies if a variable definition is correct according to the syntactic analyse. 
// Correct formats: 
//Dim id As data_type 
//Dim id As data_type = expression
int varDefinition(FILE *f, int funcFlag, char *funcName) {
	int tok,
		result,
		i = 0;
    char *buffer,
    	 *idStorage;
   	addedItem = NULL;
   	fprintf(stderr,"ID\n");
    tok = Token(&buffer, f);
    fprintf(stderr,"FASA ID\n");
    switch(tok) {
    	case TOK_ID:
			if((idStorage = (char *) malloc((strlen(buffer) + 1) * sizeof(char))) == NULL){
				//fprintf(stderr, "Insufficient memory space!\n");
				return 99;
			}
    		strcpy(idStorage, buffer);
    		if(funcFlag){
	    		if((addedItem = STFind(&GST, funcName)) == NULL){
	    			//fprintf(stderr, "FUNC %s NOT FOUND\n", funcName);
	    			return ERR_OTHER;
	    		}
	    		while(i < addedItem->item_data.function->num_of_args){
		   			//fprintf(stderr, "FUNC %s NOT FOUND\n\n\n\n", idStorage);
	    			if((strcmp(addedItem->item_data.function->args[i].param_name, idStorage)) == 0) {
						//fprintf(stderr, "%s IS ALREADY DEFINED AS A FUNC PARAM\n", idStorage);    				
	    				return ERR_SEM;
	    			}
	    			i++;
	    		}
	    	}
    		if ((tok = Token (&buffer, f)) == ERR_LEX) {
				//fprintf(stderr, "Lex err in varDefinition!\n");							
    			return ERR_LEX;
    		}
    		else if (tok != KW_AS){
				//fprintf(stderr, "Syn err in varDefinition! Expected AS after ID\n");							
    			return ERR_SYN;
    		}
    		else {
    			//fprintf(stderr,"za AS\n");
    			tok = Token (&buffer, f);
    			//fprintf(stderr,"fasa AS\n");
    			if (tok == ERR_LEX){
   					//fprintf(stderr, "!\n");							 				
    				return ERR_LEX;
    			}
    			else if ((tok == KW_INTEGER) || (tok == KW_DOUBLE) || (tok == KW_STRING)) {
    				//fprintf(stderr,"Nieco s int/double/string\n");
					if(scopeFound){
						if((STFind(&GST, idStorage)) != NULL) {
							//fprintf(stderr, "Redifinition of %s ! Return code 3\n GST", idStorage);
							return 64;
						}						
					} else {
						if((STFind(&(func_name->item_data.function->local_table), idStorage)) != NULL) {
							//fprintf(stderr, "Redifinition of %s ! Return code 3\n LST", idStorage);
							return 64;
						}
					}
					if(func_name != NULL){
						addedItem = STAdd(&(func_name->item_data.function->local_table), idStorage, 0);
					} else {
						//fprintf(stderr,"do tabulky\n");
						addedItem = STAdd(&GST, idStorage, 0);
						//fprintf(stderr,"fasa tabulka\n");
						//if (addedItem == NULL)
							//fprintf(stderr,"Adt item je NULL\n");						
					}
					//fprintf(stderr,"do tabulky3\n");
					addedItem->item_type = 0;
					//fprintf(stderr,"fasa tabulka 3\n");
					if(scopeFound == 1) {
						//fprintf(stderr,"do tabulky4\n");
						//fprintf(stderr,"fasa tabulka4\n");
						addedItem->item_data.variable.varFrame = 1;
						//fprintf(stderr,"fasa tabulka 4\n");
					}
					else if(scopeFound == 0)
						addedItem->item_data.variable.varFrame = 2;
					//fprintf(stderr,"do tabulky2\n");
					addedItem->item_data.variable.name = idStorage;
					//fprintf(stderr,"fasa tabulka 2\n");	
					if(tok == KW_INTEGER){
						//fprintf(stderr,"INT\n");
						addedItem->item_data.variable.data_type = integer;
						//fprintf(stderr,"INT fasa\n");
						instr_generator(IO_VAR_DEF, idStorage, NULL, NULL, 1, addedItem->item_data.variable.varFrame, 0);
						instr_generator(IO_ASS, idStorage, "0", NULL, 1, addedItem->item_data.variable.varFrame, 0);
					}
					else if(tok == KW_DOUBLE){
						addedItem->item_data.variable.data_type = d_double;
						instr_generator(IO_VAR_DEF, idStorage, NULL, NULL, 2, addedItem->item_data.variable.varFrame, 0);
						instr_generator(IO_ASS, idStorage, "0.0", NULL, 2, addedItem->item_data.variable.varFrame, 0);
					}
					else{
						instr_generator(IO_VAR_DEF, idStorage, NULL, NULL, 3, addedItem->item_data.variable.varFrame, 0);
						instr_generator(IO_ASS, idStorage, "", NULL, 3, addedItem->item_data.variable.varFrame, 0);
						addedItem->item_data.variable.data_type = string;
					}

					addedItem->item_data.variable.declared = 1;
					addedItem->item_data.variable.initialized = 1;

    				tok = Token (&buffer, f);
    				if (tok == ERR_LEX){
						//fprintf(stderr, "Lex err in varDefinition!\n");							
    					return ERR_LEX;
    				}
    				else if (tok == TOK_ASSIGN_OR_EQUALS) {
	    				tok = Token (&buffer, f);
	    				if (tok == ERR_LEX)
							return ERR_LEX;
	    				//fprintf(stderr,"zaciatok analyzy\n");
    					result = expressionAnalysis(f, tok, buffer, 1, idStorage, 0, 0);
    					//fprintf(stderr,"koniec analyzy\n");
    					return result;
					}
    				else if (tok != TOK_EOL && tok != TOK_LINE_COMMENT && tok != TOK_BLOCK_COMMENT) {
    					printf ("tok je %d\n", tok);
						//fprintf(stderr, "Syn err in varDefinition!\n");							
    					return ERR_SYN;
    				}
    				else {
    						free(buffer);
							buffer = NULL;
    						return OK;
    				}			
    			}
				else{
					//fprintf(stderr, "Undefined char in varDefinition\n");							
					return ERR_SYN;
					}	
			}
		case ERR_LEX:
			//fprintf(stderr, "Err lex in varDefinition!\n");							
			return ERR_LEX;	
		default:
			//fprintf(stderr, "Default in varDefinition!\n");							
			return ERR_SYN;	
	}
	return OK;
}

int printAnalysis(FILE *f) { // two way analysis done
	int tokNum = 0;
	char *parseBuffer;
		 //*stringStorage;
	tokNum = Token(&parseBuffer, f);	
	if (tokNum == ERR_LEX)
		return ERR_LEX; 
	while(tokNum != TOK_EOL) {	
		//fprintf(stderr,"Print\n");
		int exp_res = expressionAnalysis(f, tokNum, parseBuffer, 4, NULL, 0, 0);
		if (exp_res != OK)
			return exp_res;
		tokNum = Token(&parseBuffer, f);
	}
	free(parseBuffer);
	parseBuffer = NULL;	
	return 0;
}

int whileAnalysis(FILE *f, char *frameNameLoc, int while_counter) {
	int tokNum;
	char *buffer;
	tokNum = Token(&buffer, f);
    if (tokNum == ERR_LEX){
		//fprintf(stderr, "Err lex after DO!\n");							
		return ERR_LEX;
    }
	else if (tokNum != KW_WHILE){
		//fprintf(stderr, "No while after do!\n");							
		return ERR_SYN;
	}
	else {
		//fprintf (stderr,"precedencna analyza");
		int result;
		tokNum = Token(&buffer, f);
		if (tokNum == ERR_LEX)
			return ERR_LEX;
		if ((result = expressionAnalysis(f, tokNum, buffer, 0, NULL, 0, 0)) != OK) {
			if (result == ERR_LEX){
				//fprintf(stderr, "semanticAnalysis failed!\n");							
				return ERR_LEX;
			}
			else {
				//fprintf(stderr, "semanticAnalysis failed!\n");							
				return ERR_SYN;
			}
		}
		else {
			char *idStorage;
			idStorage = (char *) malloc(8 * sizeof(char));
			sprintf(idStorage, "%%loops%d", while_counter);
			instr_generator(IO_PUSHS, "1", NULL, NULL, 5, 0, 0);
			instr_generator(IO_JUMP_IF_NEQ_S, idStorage, NULL, NULL, 0, 0, 0);
				int result;
				result = commandsCycle(1, 0, 0, 0, f, frameNameLoc, while_counter);
				if (result != OK) {
					if (result == ERR_LEX){
						//fprintf(stderr, "Lexierr after commandscycle in whileAnalysis\n");
						return ERR_LEX;
					}
					else if (result == ERR_SYN){
						//fprintf(stderr, "Commands cycle in while returned syn err!\n");							
						return ERR_SYN;
					}
					else {
						free(buffer);
						buffer = NULL; 
						return OK;
					}
				}
				else
					return OK;
			}
		}	    
	}   

