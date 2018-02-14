#include "instruction.h"
#include "symtab.h"
int create_flag = 0;
int push_flag = 0;
void InitList(){
	list.Act = NULL;
	list.Last = NULL;
	list.First = NULL;
}

void DisposeList(){
	tElemPtr temp;
	for(list.Act = list.First; list.Act != NULL; list.Act = list.First) { 
		temp = list.Act;
		list.First = temp->next;
		free(temp);
		temp = NULL;
	}
}

void InsertFirst(tIns ins){
	tElemPtr temp;
	if((temp = (tElemPtr) malloc(sizeof(struct tElem))) == NULL) 
		exit(99);
	temp->instr = ins;
	temp->next = list.First;
	if(list.First == NULL)
		list.Last = temp;
	list.First = temp; 
}

void InsertLast(tIns ins){
	tElemPtr temp;
	if((temp = (tElemPtr) malloc(sizeof(struct tElem))) == NULL)
		exit(99);
	temp->instr = ins;
	//if (temp->instr.insOp == IO_PUSHS) {
		//fprintf (stderr,"Push list\n");
		//fprintf(stderr,"hodnota je %d\n", atoi((char *) temp->instr.adr1));
	//}
	temp->next = NULL;
	if(list.First == NULL)
		list.First = temp;
	else
		list.Last->next = temp;
	list.Last = temp; 
	//if (list.Last->instr.insOp == IO_PUSHS)
	//fprintf (stderr,"priradilo sa %d\n", atoi((char *) list.Last->instr.adr1));
}

void DeleteFirst(){
	tElemPtr temp;
	if(list.First != NULL) {  
		temp = list.First;
		if(list.Act == list.First){ 
			list.Act = NULL;
		}
		list.First = temp->next; 
		free(temp); 
	}
}

void PostDelete(){
	tElemPtr temp;
	if(list.Act != NULL){
		if(list.Act->next != NULL) { 
			temp = list.Act->next; 
			list.Act->next = temp->next;  
			free(temp); 
		}
	}
}

void PostInsert(tIns ins){
	tElemPtr temp;
	if(list.Act != NULL){
		if((temp = (tElemPtr) malloc(sizeof(struct tElem))) == NULL) 
			exit(99);
		temp->instr = ins;
		temp->next = list.Act->next; 
		list.Act->next = temp; 
	}
}

void Succ(){
	if (list.Act == NULL)
		return;
	if (list.Act == list.Last)
		list.Act = NULL;	
	else
		list.Act = list.Act->next; 	
}

void Jump(void *jumpIns){
	list.Act = (tElemPtr) jumpIns;
}

tElemPtr Last(){
	return list.Last;
}

tIns *RetAct(){
	if(list.Act != NULL)
		return &(list.Act->instr);

	return NULL;
}

int generator() { // symb moze byt aj premenna
	FILE *f;
	int var;
	double d_var;
	int fr, fr1, fr2;
	char * symbf;
	char *symbf1;
	char *symbf2;
	//STitem *tmp_item;
	f = stdout;
	
	fprintf(f,".IFJcode17\n"); // Compulsory start of a .ifj file
	list.Act = list.First;
	
	while(list.Act != NULL) {
	
		switch(list.Act->instr.insOp) {
			case IO_ADDS:
				fprintf(f, "ADDS\n");
				break;
			case IO_SUBS:
				fprintf(f, "SUBS\n");			
				break;
			case IO_MULS:
				fprintf(f, "MULS\n");			
				break;
			case IO_DIVS:
				fprintf(f, "DIVS\n");			
				break;
			case IO_L:
				fprintf(f, "LTS\n");			
				break;
			case IO_G:
				fprintf(f, "GTS\n");			
				break;
			case IO_EQ:
				fprintf(f, "EQS\n");			
				break;
			case IO_CLEARS:
				fprintf(f, "CLEARS\n");
				break;
			case IO_CREATE_FRAME:
				create_flag = 1;
				fprintf(f, "CREATEFRAME\n");
				break;
			case IO_PUSH_FRAME:
				push_flag = 1;
				fprintf(f, "PUSHFRAME\n");
				break;
			case IO_POP_FRAME:
				push_flag = 0;
				fprintf(f, "POPFRAME\n");
				break;
			case IO_RET:
				fprintf(f, "RETURN\n");
				break;
			case IO_INT2FLOATS:
				fprintf(f, "INT2FLOATS\n");			
				break;
			case IO_INT2CHARS:
				fprintf(f, "INT2CHARS\n");			
				break;
			case IO_FLOAT2R2EINTS:
				fprintf(f, "FLOAT2R2EINTS\n");			
				break;	
			case IO_STRI2INTS:
				fprintf(f, "STRI2INTS\n");			
				break;
			case IO_AND_S:
				fprintf(f, "ANDS\n");			
				break;
			case IO_OR_S:
				fprintf(f, "ORS\n");			
				break;			
			case IO_NOT_S:
				fprintf(f, "NOTS\n");			
				break;
			case IO_BREAK:
				fprintf(f, "BREAK\n");			
				break;
			case IO_INT2FLOAT:
				fr = list.Act->instr.frame;
				if (fr == 1){
					symbf = "GF";
				}
				else if (fr == 2){
					symbf = "LF";
				}
				else if (fr == 3){
					symbf = "TF";
				}
				else{
					//fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF120\n");
					return 99;
				}
				switch(list.Act->instr.ins_type){
					case 1:
						var = atoi((char*)list.Act->instr.adr2);
						fprintf(f, "INT2FLOAT %s@%s int@%d\n", symbf, (char*)list.Act->instr.adr1, var);
						break;
					case 4:
		
						fr1 = list.Act->instr.frame2;
						if (fr1 == 1){
							symbf1 = "GF";
						}
						else if (fr1 == 2){
							symbf1 = "LF";
						}
						else if (fr1 == 3){
							symbf1 = "TF";
						}
						else{
							//fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF222\n");
							return 99;
						}
						fprintf(f, "INT2FLOAT %s@%s %s@%s\n", symbf, (char*)list.Act->instr.adr1, symbf1, (char*)list.Act->instr.adr2);
						break;
					default:
						//fprintf(stderr, "Type in the instruction is wrong!\n");
						return 99;
						break;

				}	
				break;
			case IO_FLOAT2INT:
		
				fr = list.Act->instr.frame;
				if (fr == 1){
					symbf = "GF";
				}
				else if (fr == 2){
					symbf = "LF";
				}
				else if (fr == 3){
					symbf = "TF";
				}
				else{
					//fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF323\n");
					return 99;
				}
				switch(list.Act->instr.ins_type){
					case 1:
						d_var = atof((char*)list.Act->instr.adr2);
						fprintf(f, "FLOAT2INT %s@%s float@%g\n", symbf, (char*)list.Act->instr.adr1, d_var);
						break;
					case 4:
					
						fr1 = list.Act->instr.frame2;
						if (fr1 == 1){
							symbf1 = "GF";
						}
						else if (fr1 == 2){
							symbf1 = "LF";
						}
						else if (fr1 == 3){
							symbf1 = "TF";
						}
						else{
						//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF424\n");
							return 99;
						}
						fprintf(f, "FLOAT2INT %s@%s %s@%s\n", symbf, (char*)list.Act->instr.adr1, symbf1, (char*)list.Act->instr.adr2);
						break;
					default:
						//fprintf(stderr, "Type in the instruction is wrong!\n");
						return 99;
						break;

				}	
				break;
			case IO_FLOAT2R2EINT:
			
				fr = list.Act->instr.frame;
				if (fr == 1){
					symbf = "GF";
				}
				else if (fr == 2){
					symbf = "LF";
				}
				else if (fr == 3){
					symbf = "TF";
				}
				else{
					//fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF525\n");
					return 99;
				}
				switch(list.Act->instr.ins_type){
					case 1:
						d_var = atof((char*)list.Act->instr.adr2);
						fprintf(f, "FLOAT2R2EINT %s@%s float@%g\n", symbf, (char*)list.Act->instr.adr1, d_var);
						break;
					case 4:
				
						fr1 = list.Act->instr.frame2;
						if (fr1 == 1){
							symbf1 = "GF";
						}
						else if (fr1 == 2){
							symbf1 = "LF";
						}
						else if (fr1 == 3){
							symbf1 = "TF";
						}
						else{
							//fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF626\n");
							return 99;
						}
						fprintf(f, "FLOAT2R2EINT %s@%s %s@%s\n", symbf, (char*)list.Act->instr.adr1, symbf1, (char*)list.Act->instr.adr2);
						break;
					default:
						fprintf(stderr, "Type in the instruction is wrong!\n");
						return 99;
						break;

				}	
				break;
			case IO_ASS:
		
				fr = list.Act->instr.frame;
				if (fr == 1){
					symbf = "GF";
				}
				else if (fr == 2){
					symbf = "LF";
				}
				else if (fr == 3){
					symbf = "TF";
				}
				else{
				//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF727\n");
					return 99;
				}
				switch (list.Act->instr.ins_type){
					case 1:
						var = atoi((char*)list.Act->instr.adr2);
						fprintf(f, "MOVE %s@%s int@%d\n", symbf, (char*)list.Act->instr.adr1, var);
						break;
					case 2:
						d_var = atof((char*)list.Act->instr.adr2);
						fprintf(f, "MOVE %s@%s float@%g\n", symbf, (char*)list.Act->instr.adr1, d_var);
						break;
					case 3:
						fprintf(f, "MOVE %s@%s string@%s\n", symbf, (char*)list.Act->instr.adr1, (char*)list.Act->instr.adr2);
						break;
					case 4:
				
						fr1 = list.Act->instr.frame2;
						if (fr1 == 1){
							symbf1 = "GF";
						}
						else if (fr1 == 2){
							symbf1 = "LF";
						}
						else if (fr1 == 3){
							symbf1 = "TF";
						}
						else{
							//fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF828\n");
							return 99;
						}
						fprintf(f, "MOVE %s@%s %s@%s\n", symbf, (char*)list.Act->instr.adr1, symbf1, (char*)list.Act->instr.adr2);
						break;
					default:
						//fprintf(stderr, "Type in the instruction is wrong!\n");
						return 99;
						break;
				}
				break;
			case IO_PUSHS:
			   // fprintf (stderr,"Pushs pri instrukcii je %s\n", (char *) list.Act->instr.adr1);
				switchtype(list.Act->instr.ins_type, "PUSHS", f, list.Act->instr.adr1);
			//	fprintf (stderr,"Pushs pri instrukcii je %d\n", atoi((char *) list.Act->instr.adr1));
				/*
				switch (list.Act->instr.ins_type){
					case 1:
						fprintf(f, "PUSHS int@%d\n", *((int*)list.Act->instr.adr1));
						break;
					case 2:
						fprintf(f, "PUSHS float@%g\n", *((float*)list.Act->instr.adr1));
						break;
					case 3:
						fprintf(f, "PUSHS string@%s\n", (char*)list.Act->instr.adr1);
						break;
					default:
						fprintf(ftderr, "Type in the instruction is wrong!\n");
						return 99;
						break
				}
				*/
				break;
			case IO_POPS: // not finished
		
				fr = list.Act->instr.frame;
				if (fr == 1){
					symbf = "GF";
				}
				else if (fr == 2){
					symbf = "LF";
				}
				else if (fr == 3){
					symbf = "TF";
				}
				else{
				//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF929\n");
					return 99;
				}
				fprintf(f, "POPS %s@%s\n", symbf, (char*)list.Act->instr.adr1); 
				break;
			case IO_VAR_DEF: // not finished
	
				fr = list.Act->instr.frame;
				if (fr == 1){
					symbf = "GF";
				}
				else if (fr == 2){
					symbf = "LF";
				}
				else if (fr == 3){
					symbf = "TF";
				}
				else{
				//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF1030\n");
					return 99;
				}
				fprintf(f, "DEFVAR %s@%s\n", symbf, (char*)list.Act->instr.adr1); // pred @ bude LF alebo GF
				break;
			case IO_INPUT: // not finished
	
				fr = list.Act->instr.frame;
				if (fr == 1){
					symbf = "GF";
				}
				else if (fr == 2){
					symbf = "LF";
				}
				else if (fr == 3){
					symbf = "TF";
				}
				else{
				//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF1131\n");
					return 99;
				}
				fprintf(f, "WRITE string@?\\032\n");
				switch(list.Act->instr.ins_type){
					case 1:
						fprintf(f, "READ %s@%s int\n", symbf, (char*)list.Act->instr.adr1);
						break;
					case 2:
						fprintf(f, "READ %s@%s float\n", symbf, (char*)list.Act->instr.adr1);
						break;
					case 3:
						fprintf(f, "READ %s@%s string\n", symbf, (char*)list.Act->instr.adr1);
						break;
					default:
						fprintf(stderr, "Type in the instruction is wrong!\n");
						return 99;
						break;
				}
				break;
			case IO_PRINT:
				switchtype(list.Act->instr.ins_type, "WRITE", f, list.Act->instr.adr1);
				break;
				/*
				switch(list.Act->instr.ins_type){
					case 1:
						fprintf(f, "WRITE int@%d\n", *((int*)list.Act->instr.adr1));
						break;
					case 2:
						fprintf(f, "WRITE float@%g\n", *((float*)list.Act->instr.adr1));
						break;
					case 3:
						fprintf(f, "WRITE string@%s\n", (char*)list.Act->instr.adr1);
						break;
					default:
						fprintf(ftderr, "Type in the instruction is wrong!\n");
						return 99;
						break;
				}
				break;
				*/
			case IO_CONCAT:
				
				fr = list.Act->instr.frame;
				if (fr == 1){
					symbf = "GF";
				}
				else if (fr == 2){
					symbf = "LF";
				}
				else if (fr == 3){
					symbf = "TF";
				}
				else{
				//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF1232\n");
					return 99;
				}
				switch(list.Act->instr.ins_type){
					case 3:
						fprintf(f, "CONCAT %s@%s string@%s string@%s\n", symbf, (char*)list.Act->instr.adr1, (char*)list.Act->instr.adr2, (char*)list.Act->instr.adr3);
						break;
					case 4:
			
						fr1 = list.Act->instr.frame2;
						if (fr1 == 1){
							symbf1 = "GF";
						}
						else if (fr1 == 2){
							symbf1 = "LF";
						}
						else if (fr1 == 3){
							symbf1 = "TF";
						}
						else{
						//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF1333\n");
							return 99;
						}

						fr2 = list.Act->instr.frame2;
						if (fr2 == 1){
							symbf2 = "GF";
						}
						else if (fr2 == 2){
							symbf2 = "LF";
						}
						else if (fr2 == 3){
							symbf2 = "TF";
						}
						else{
						//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF1434\n");
							return 99;
						}
						fprintf(f, "CONCAT %s@%s %s@%s %s@%s\n", symbf, (char*)list.Act->instr.adr1, symbf1, (char*)list.Act->instr.adr2, symbf2, (char*)list.Act->instr.adr3);
						break;
					case 5:
						fr1 = list.Act->instr.frame2;
						if (fr1 == 1){
							symbf1 = "GF";
						}
						else if (fr1 == 2){
							symbf1 = "LF";
						}
						else if (fr1 == 3){
							symbf1 = "TF";
						}
						else{
						//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF15\n");
							return 99;
						}
						fprintf(f, "CONCAT %s@%s string@%s %s@%s\n", symbf, (char*)list.Act->instr.adr1, (char*)list.Act->instr.adr2, symbf1, (char*)list.Act->instr.adr3);
						break;
					case 6:
				
						fr1 = list.Act->instr.frame2;
						if (fr1 == 1){
							symbf1 = "GF";
						}
						else if (fr1 == 2){
							symbf1 = "LF";
						}
						else if (fr1 == 3){
							symbf1 = "TF";
						}
						else{
						//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF16\n");
							return 99;
						}
						fprintf(f, "CONCAT %s@%s %s@%s string@%s\n", symbf, (char*)list.Act->instr.adr1, symbf1, (char*)list.Act->instr.adr2, (char*)list.Act->instr.adr3);
						break;
				}
				break;
			case IO_LEN: // not finished
				fr = list.Act->instr.frame;
				if (fr == 1){
					symbf = "GF";
				}
				else if (fr == 2){
					symbf = "LF";
				}
				else if (fr == 3){
					symbf = "TF";
				}
				else{
				//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF17\n");
					return 99;
				}
				switch(list.Act->instr.ins_type){
					case 3:
						fprintf(f, "STRLEN %s@%s string@%s\n", symbf, (char*)list.Act->instr.adr1, (char*)list.Act->instr.adr2);
						break;
					case 4:
						fr1 = list.Act->instr.frame2;
						if (fr1 == 1){
							symbf1 = "GF";
						}
						else if (fr1 == 2){
							symbf1 = "LF";
						}
						else if (fr1 == 3){
							symbf1 = "TF";
						}
						else{
						//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF18\n");
							return 99;
						}
						fprintf(f, "STRLEN %s@%s %s@%s\n", symbf, (char*)list.Act->instr.adr1, symbf1, (char*)list.Act->instr.adr2);
						break;
				}
				break;
			case IO_INT2CHAR:
				fr = list.Act->instr.frame;
				if (fr == 1){
					symbf = "GF";
				}
				else if (fr == 2){
					symbf = "LF";
				}
				else if (fr == 3){
					symbf = "TF";
				}
				else{
				//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF\n");
					return 99;
				}
				switch(list.Act->instr.ins_type){
					case 1:
						var = atoi((char*)list.Act->instr.adr2);
						fprintf(f, "INT2CHAR %s@%s int@%d\n", symbf, (char*)list.Act->instr.adr1, var);
						break;
					case 4:
		
						fr1 = list.Act->instr.frame2;
						if (fr1 == 1){
							symbf1 = "GF";
						}
						else if (fr1 ==	2){
							symbf1 = "LF";
						}
						else if (fr1 == 3){
							symbf1 = "TF";
						}
						else{
						//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF\n");
							return 99;
						}
						fprintf(f, "INT2CHAR %s@%s %s@%s\n", symbf, (char*)list.Act->instr.adr1, symbf1, (char*)list.Act->instr.adr2);
						break;
					default:
					//	fprintf(stderr, "Type in the instruction is wrong!\n");
						return 99;
						break;

				}
				break;	
			case IO_STRI2INT:
				fr = list.Act->instr.frame;
				if (fr == 1){
					symbf = "GF";
				}
				else if (fr == 2){
					symbf = "LF";
				}
				else if (fr == 3){
					symbf = "TF";
				}
				else{
				//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF\n");
					return 99;
				}
				switch(list.Act->instr.ins_type){
					case 4:
        				
						fr1 = list.Act->instr.frame2;
						if (fr1 == 1){
							symbf1 = "GF";
						}
						else if (fr1 == 2){
							symbf1 = "LF";
						}
						else if (fr1 == 3){
							symbf1 = "TF";
						}
						else{
						//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF\n");
							return 99;
						}
						fprintf(f, "STRI2INT %s@%s %s@%s %s@%s\n", symbf, (char*)list.Act->instr.adr1, symbf1, (char*)list.Act->instr.adr2, symbf1, (char*)list.Act->instr.adr3);
						break;
					
				}
				break;	
			case IO_DPRINT: // treba doplnit ked je symb premenna
				switchtype(list.Act->instr.ins_type, "DPRINT", f, list.Act->instr.adr1);
				break;
				/*
				switch(list.Act->instr.ins_type){
					case 1:
						fprintf(f, "DPRINT int@%d\n", *((int*)list.Act->instr.adr1));
						break;
					case 2:
						fprintf(f, "DPRINT float@%g\n", *((float*)list.Act->instr.adr1));
						break;
					case 3:
						fprintf(f, "DPRINT string@%s\n", (char*)list.Act->instr.adr1);
						break;
					default:
						fprintf(ftderr, "Type in the instruction is wrong!\n");
						return 99;
						break;
				}
				break;
				*/
			case IO_CALL:
				create_flag = 0;
				fprintf(f, "CALL %s\n", (char*)list.Act->instr.adr1);
				break;
			case IO_LABEL:
				fprintf(f, "LABEL %s\n", (char*)list.Act->instr.adr1);
				break;
			case IO_JUMP:
				fprintf(f, "JUMP %s\n", (char*)list.Act->instr.adr1);
				break;
			case IO_JUMP_IF_EQ_S:
				fprintf(f, "JUMPIFEQS %s\n", (char*)list.Act->instr.adr1);
				break;
			case IO_JUMP_IF_NEQ_S:
				fprintf(f, "JUMPIFNEQS %s\n", (char*)list.Act->instr.adr1);
				break;
			default:
			//	fprintf(stderr, "Unknown instruction!\n");
				return 99;
				break;
		}
		list.Act = list.Act->next;
	}
	// chyba type,getchar,getchar(treba?)  
	fclose(f);
	return 0;
}

int switchtype(int type, char* instruct, FILE *f, void* adr){
	int var;
	double d_var;
	int fr;
	char *symbf;
	//STitem *tmp_item;
	switch(type){
		case 1:
			var = atoi((char*)adr);
			//fprintf (stderr,"var v switch je %d\n", var);
			fprintf(f, "%s int@%d\n", instruct, var);
			break;
		case 2:
			d_var = atof((char*)adr);
			fprintf(f, "%s float@%g\n", instruct, d_var);
			break;
		case 3:
			fprintf(f, "%s string@%s\n", instruct, (char*)adr);
			break;
		case 4:	
			fr = list.Act->instr.frame;
			if (fr == 1){
				symbf = "GF";
			}
			else if (fr == 2){
				symbf = "LF";
			}
			else if (fr == 3){
				symbf = "TF";
			}
			else{
			//	fprintf(stderr, "Neni urcena premenna, ci je LF alebo GF19\n");
				return 99;
			}
			fprintf(f, "%s %s@%s\n", instruct, symbf, (char*)adr);
			break;
		case 5:
			fprintf(f, "%s bool@true\n", instruct);
			break;
		case 6:
			fprintf(f, "%s bool@false\n", instruct);
			break;
		default:
			//fprintf(stderr, "Type in the instruction is wrong!\n");
			return 99;
			break;
	}
	return 0;
}

void instr_generator(int ins_type, void *a1, void *a2, void *a3, int type, int frame, int frame2){
    instr.insOp = ins_type;
    if (ins_type == IO_PUSHS) {
    	//fprintf(stderr,"push\n");
    	
    	//fprintf (stderr,"tmp je %d\n",tmp);
    }

    instr.adr1 = a1;
   // if (ins_type == IO_PUSHS) {
    	//int tmp = atoi((char *) instr.adr1);
     //   fprintf (stderr,"tmp je %d\n", tmp);
   // }    
    instr.adr2 = a2;
    instr.adr3 = a3;
    instr.ins_type = type;
    instr.frame = frame;
    instr.frame2= frame2;
    InsertLast(instr);
}