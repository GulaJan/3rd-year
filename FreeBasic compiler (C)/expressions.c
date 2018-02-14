#include "expressions.h"
#include <string.h>
STitem *tmp_item;
int only_one = 1;
int dividing = 0;
char pt[11][10] = {
                        //     */   \     +    -   log  ,)    (   str   i    $
    /* *,/ */                 {'R', 'R', 'R', 'R', 'R', 'R', '<', '<', '<', 'R'},
    /*  \  */                 {'<', 'R', 'R', 'R', 'R', 'R', '<', '<', '<', 'R'},
    /*  +  */                 {'<', '<', 'R', 'R', 'R', 'R', '<', '<', '<', 'R'},
    /*  -  */                 {'<', '<', 'R', 'R', 'R', 'R', '<', '<', '<', 'R'},
    /* log(<,>,=,>=,<=,<>) */ {'<', '<', '<', '<', 'R', 'R', '<', '<', '<', 'R'},
    /*  )  */                 {'R', 'R', 'R', 'R', 'R', 'R', 'X', 'X', 'X', 'R'}, // treba to fixnut maybe
    /*  (  */                 {'<', '<', '<', '<', '<', '=', '<', '<', '<', 'X'},
    /* str */                 {'R', 'R', 'R', 'R', '<', 'R', 'X', 'X', 'X', 'R'},
    /*  i  */                 {'R', 'R', 'R', 'R', 'R', 'R', 'X', 'X', 'X', 'R'},
    /*  $  */                 {'<', '<', '<', '<', '<', 'X', '<', '<', '<', 'X'}
};

char *token;
char *token_prev;
tStack stack;
int pt_decode(int tokNum){
    switch(tokNum){
        case TOK_MULTIPLY: case TOK_DIVIDE:
            return 0;
        case TOK_WHOLENUMBER_DIVIDE:
            return 1;
        case TOK_PLUS:
            return 2;
        case TOK_MINUS:
            return 3;
        case TOK_ASSIGN_OR_EQUALS: case TOK_LOWER: case TOK_GREATER: case TOK_GREATER_EQ: case TOK_LOWER_EQ: case TOK_NOT_EQUAL:
            return 4;
        case TOK_RIGHT_RB:
            return 5;
        case TOK_LEFT_RB:
            return 6;
        case TOK_STRING_STREAM:
            return 7;
        case TOK_ID: case TOK_INT: case TOK_DOUBLE: 
            return 8;
        case TOK_EOF:
            return 9;
        default:
            return 12;
    }
}

bool rules(int num, ...){
    va_list list;
    va_start(list, num);
    int term, i;
    int e = va_arg(list, int);
    for (i = 0; i < num - 1; i++){
        term = va_arg(list, int);
        if (stack.data[stack.top - i] != term){
            va_end(list);
            return false;
        }
    }
    if (num < 4){
        for (i = 0; i < num; i++)
        STopPop(&stack);
    }
    if ((term == TOK_INT) || (term == TOK_DOUBLE) || (term == TOK_STRING_STREAM))
        SPush(&stack, e, term);
    va_end(list);
    return true;
}


int expressionAnalysis(FILE *f, int tokNum, char *input, int exp_type, char *variable, int if_flag, int func_flag) {
    SInit(&stack);
   // fprintf (stderr,"aj tu\n");
    SPush(&stack, TOK_EOF, TOK);
    int stack_token;
    int result;
    int tokNum_pom;
    //fprintf (stderr,"token je %d\n",tokNum);
    int counter = 0;
    do {
        SPrint(&stack);
        if (tokNum == TOK_LINE_COMMENT || tokNum == TOK_BLOCK_COMMENT) {
            while (tokNum == TOK_LINE_COMMENT || tokNum == TOK_BLOCK_COMMENT) {
                tokNum = Token(&input, f);
            }
        }
        if (tokNum == TOK_EOL) {

            tokNum = TOK_EOF;
            if (exp_type == 4) {
                return ERR_SYN;

            }
        }
        if ((tokNum == TOK_SEMICOLON) && (exp_type == 4)) {
            tokNum = TOK_EOF;
        }
        if ((tokNum == KW_THEN) && (if_flag == 1))
            tokNum = TOK_EOF;
        stack_token = STop(&stack);
        if (stack_token == E_NT)
            stack_token = STopSec(&stack);
        switch(pt[pt_decode(stack_token)][pt_decode(tokNum)]) {
            case '<':
               if (tokNum == TOK_MINUS ) {
                    if ((stack.data[stack.top] == TOK_EOF) || (stack.data[stack.top] == TOK_LEFT_RB)) {

                    while((tokNum = Token(&input, f)) == TOK_MINUS);
                    if (tokNum == ERR_LEX)
                        return ERR_LEX;
                    char *min_num = (char *) malloc((strlen(input) + 2) * sizeof(char));
                    min_num[0] = '-';
                    int pom = 0;
                    while(input[pom] != '\0') {
                        min_num[pom+1] = input[pom];
                        pom++;
                    }
                    input = min_num;
                    break;
                 }
             }
                //fprintf(stderr,"push s <\n");
                if (STop(&stack) == E_NT) {
                    int not_null = 0;
                    if (stack.value[stack.top] != NULL) {
                        not_null = 1;
                    }
                    int type = stack.data_type[stack.top];
                    int var = stack.is_variable[stack.top];
                    STopPop(&stack);
                    SPush(&stack, P_L, TOK);
                    SPush(&stack, E_NT, type);
                    stack.is_variable[stack.top] = var;
                    if (not_null == 1) {
                        stack.value[stack.top] = (void *) token_prev;
                    }
                    SPush(&stack, tokNum, TOK);
                    SPrint(&stack);
                }
                else {
                    SPush(&stack, P_L, TOK);
                    SPush(&stack, tokNum, TOK);
                    SPrint(&stack);
                }
                token_prev = input;
                tokNum_pom = tokNum;
                tokNum = Token(&input,f);
                 if (tokNum_pom == TOK_PLUS || tokNum_pom == TOK_MINUS || tokNum_pom == TOK_MULTIPLY || tokNum_pom == TOK_DIVIDE) {
                    if (tokNum == TOK_PLUS || tokNum == TOK_MINUS || tokNum == TOK_MULTIPLY || tokNum == TOK_DIVIDE)
                        return ERR_SYN;
                 }
                //fprintf (stderr,"Token v push je %d\n", tokNum);
                if (tokNum == ERR_LEX)
                    return ERR_LEX;
                if ((tokNum == TOK_LEFT_RB) && (counter == 0) && (tokNum_pom == TOK_ID)) {
                    int f_result = function_call(token_prev, f, exp_type);
                    if (exp_type == 2)
                        return f_result;
                    if (f_result != OK) {
                        return f_result;
                    }
                    else {
                        char *tmp_c;
                        if (exp_type != 4) {
                            tmp_c = (char *) malloc ((strlen(variable) + 1) * sizeof(char));
                            strcpy(tmp_c, variable);
                        }    
                        //fprintf (stderr,"funkcia je stale %s\n", token_prev);
                        STitem *tmp_f;
                        tmp_f = STFind(&GST, token_prev);
                        if (tmp_f == NULL) {
                            //fprintf(stderr,"undeclared function\n");
                            return ERR_SYN;
                        }
                        if (exp_type == 4) {
                            if (tmp_f->item_data.function->data_type == integer) {
                                tmp_item = STFind(&GST, "\045print_temp_variable");
                            }
                            else if (tmp_f->item_data.function->data_type == d_double)
                                tmp_item = STFind(&GST, "\045print_temp_variable1");
                            else if (tmp_f->item_data.function->data_type == string)
                                tmp_item = STFind(&GST, "\045print_temp_variable2");
                        } 
                        else {  
                        if (func_name != NULL) { 
                            tmp_item = STFind(&(func_name->item_data.function->local_table), variable);
                        if (tmp_item == NULL) {
                            tmp_item = STFind(&GST, variable);
                            if (tmp_item == NULL) {
                                //fprintf(stderr,"undeclared variable for asigment\n");
                                return ERR_SEM;
                            } 
                        }
                      }
                      else {
                        tmp_item = STFind(&GST, variable);
                        if (tmp_item == NULL) {
                                //fprintf(stderr,"undeclared variable for asigment\n");
                                return ERR_SEM;
                            } 
                      } 
                    }
                        //fprintf(stderr,"Zde\n");
                        if (tmp_f->item_data.function->data_type == integer) {
                            if (tmp_item->item_data.variable.data_type == integer) {
                                if (exp_type == 4) {
                                instr_generator(IO_POPS, (void *) "\045print_temp_variable", NULL, NULL, 1, 1, 0);
                                instr_generator(IO_PRINT,"\045print_temp_variable", NULL, NULL, 4, 1, 0);
                            }
                            else {
                                instr_generator(IO_POPS, (void *) tmp_c, NULL, NULL, 1, tmp_item->item_data.variable.varFrame, 0);
                                }
                            return f_result;
                            }
                            else if (tmp_item->item_data.variable.data_type == d_double) {
                                instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 2, 0, 0);
                                 if (exp_type == 4) {
                                instr_generator(IO_POPS, (void *) "\045print_temp_variable1", NULL, NULL, 2, 1, 0);
                                instr_generator(IO_PRINT,"\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                            }
                            else {
                                instr_generator(IO_POPS, (void *) tmp_c, NULL, NULL, 2, tmp_item->item_data.variable.varFrame, 0);
                            }
                            return f_result;
                            }
                            else {
                                //fprintf(stderr,"incompatible types in assigment\n");
                                return ERR_SEM_TYPE;
                            }    
                        }
                        else if (tmp_f->item_data.function->data_type == d_double) {
                            if (tmp_item->item_data.variable.data_type == integer) {
                                instr_generator(IO_FLOAT2R2EINTS, NULL, NULL, NULL, 1, 0, 0);
                                 if (exp_type == 4) {
                                instr_generator(IO_POPS, (void *) "\045print_temp_variable", NULL, NULL, 1, 1, 0);
                                instr_generator(IO_PRINT,"\045print_temp_variable", NULL, NULL, 4, 1, 0);
                            }
                            else {
                                    instr_generator(IO_POPS, (void *) tmp_c, NULL, NULL, 1, tmp_item->item_data.variable.varFrame, 0);
                            }
                                 return f_result;
                            }
                            else if (tmp_item->item_data.variable.data_type == d_double) {
                                 if (exp_type == 4) {
                                instr_generator(IO_POPS, (void *) "\045print_temp_variable1", NULL, NULL, 2, 1, 0);
                                instr_generator(IO_PRINT,"\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                            }
                            else {
                                instr_generator(IO_POPS, (void *) tmp_c, NULL, NULL, 2, tmp_item->item_data.variable.varFrame, 0);
                            }    
                            return f_result;
                            }
                            else {
                               // fprintf(stderr,"incompatible types in assigment\n");
                                return ERR_SEM_TYPE;
                            }    
                        }
                        else if (tmp_f->item_data.function->data_type == string) {
                            if (tmp_item->item_data.variable.data_type == string) {

                                if (tmp_f->item_data.function->emptyRet != 1) {
                                     if (exp_type == 4) {
                                        instr_generator(IO_POPS, (void *) "\045print_temp_variable2", NULL, NULL, 3, 1, 0);
                                        instr_generator(IO_PRINT,"\045print_temp_variable2", NULL, NULL, 4, 1, 0);
                                    }
                                    else {
                                        instr_generator(IO_POPS, (void *) tmp_c, NULL, NULL, 3, tmp_item->item_data.variable.varFrame, 0);
                                    }
                                }    
                                return f_result;
                            }    
                            else {
                               // fprintf(stderr,"incompatible types in assigment\n");
                                return ERR_SEM_TYPE;
                            }
                        }    
                        else {
                           // fprintf(stderr,"Err in assigment types\n");
                            return ERR_SEM_TYPE;
                        }        

                    }
                }    
                break;
            case 'R':
                //fprintf(stderr,"reduce\n");
                result = reduce (exp_type);
                only_one = 1;
                if (result == ERR_SEM_TYPE) {
                    return ERR_SEM_TYPE;
                }
                else if (result == ERR_SYN) {
                    return ERR_SYN;
                }
                else if (result == ERR_SEM)
                    return ERR_SEM;
                else {
                    break;
                }
                SPrint(&stack);
            case '=':
                SPush(&stack, tokNum, TOK);
                token_prev = input;
                tokNum = Token(&input,f);
                if (tokNum == ERR_LEX)
                    return ERR_LEX;
                SPrint(&stack);
                break;
            case 'X':
               // fprintf(stderr,"err\n");
                SFree(&stack);
            default:
                //fprintf (stderr,"decode je %d %d \n", pt_decode(stack_token), pt_decode(tokNum));
                return ERR_SYN;
        }
        //fprintf(stderr,"vypis po pravidle\n");
        SPrint(&stack);
       // fprintf(stderr,"tok num je %d\n",tokNum);
        counter++;

    }while((tokNum != TOK_EOF) || ((STopSec(&stack)) != TOK_EOF));
   // fprintf(stderr,"dosiel\n");
    if (exp_type == 0) {
        if (only_one == 1 && (stack.data_type[stack.top] == TOK_STRING_STREAM)) {
           // fprintf(stderr,"Return ERR SEM TyPE\n");
            return ERR_SEM_TYPE;
        }
        else if (only_one == 1 && (stack.data_type[stack.top] == TOK_DOUBLE)) {
           // fprintf(stderr,"Return ERR SEM TyPE\n");
            return ERR_SEM_TYPE;
        }
        else if (only_one == 1 && (stack.data_type[stack.top] == TOK_INT)) {
            if (stack.is_variable[stack.top] == 0) {
                int push_res = instr_push(1, 3);
                if (push_res == ERR_SEM_TYPE)
                    return ERR_SEM_TYPE;

                }
            else
                 instr_push(1, 0);   
            
        }
    }
    if (exp_type == 1) {
        if (only_one == 1) {
            if ((stack.data_type[stack.top]) == TOK_INT) {
                instr_push(1, 0);
            }
            else if ((stack.data_type[stack.top]) == TOK_DOUBLE) {
                instr_push(2, 0);
            }
            else if (stack.data_type[stack.top] == TOK_STRING_STREAM)
                instr_push(3, 0);
        }
        int type = stack.data_type[stack.top];
       // fprintf (stderr,"pop\n");
       // fprintf (stderr,"variable je %s\n", variable);
        char *tmp_c;
       if (func_name != NULL) { 
            tmp_item = STFind(&(func_name->item_data.function->local_table), variable);
            if (tmp_item == NULL) {
                tmp_item = STFind(&GST, variable);
                if (tmp_item == NULL) {
                    fprintf(stderr,"undeclared variable for asigment\n");
                    return ERR_SEM;
                } 
            }
        }
        else {
            tmp_item = STFind(&GST, variable);
            if (tmp_item == NULL) {
              //  fprintf(stderr,"undeclared variable for asigment\n");
                return ERR_SEM;
            } 
        } 
        if ((type == TOK_INT) && (tmp_item->item_data.variable.data_type == integer)) {
           // fprintf (stderr,"int pushs\n");
            tmp_c = (char *) malloc((strlen((char *) variable) + 1) * sizeof(char));
            strcpy(tmp_c, variable);
            instr_generator(IO_POPS, tmp_c, NULL, NULL, 1, tmp_item->item_data.variable.varFrame, 0);
        }
        else if ((type == TOK_INT) && (tmp_item->item_data.variable.data_type == d_double)) {
            tmp_c = (char *) malloc((strlen((char *) variable) + 1) * sizeof(char));
            strcpy(tmp_c, variable);
            instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 2, 0, 0);
            instr_generator(IO_POPS, tmp_c, NULL, NULL, 2, tmp_item->item_data.variable.varFrame, 0);
        } 
        else if ((type == TOK_DOUBLE) && (tmp_item->item_data.variable.data_type == d_double)) {
            tmp_c = (char *) malloc((strlen((char *) variable) + 1) * sizeof(char));
            strcpy(tmp_c, variable);
            instr_generator(IO_POPS, tmp_c, NULL, NULL, 2, tmp_item->item_data.variable.varFrame, 0);
        }
        else if ((type == TOK_DOUBLE) && (tmp_item->item_data.variable.data_type == integer)) {
            tmp_c = (char *) malloc((strlen((char *) variable) + 1) * sizeof(char));
            strcpy(tmp_c, variable);
            instr_generator(IO_FLOAT2R2EINTS, variable, variable, NULL, 1, 0, 0);
            instr_generator(IO_POPS, tmp_c, NULL, NULL, 1, tmp_item->item_data.variable.varFrame, 0);
        }
        else if ((type == TOK_STRING_STREAM) && (tmp_item->item_data.variable.data_type == string )) {
             tmp_c = (char *) malloc((strlen((char *) variable) + 1) * sizeof(char));
             strcpy(tmp_c, variable);
             instr_generator(IO_POPS, tmp_c, NULL, NULL, 3, tmp_item->item_data.variable.varFrame, 0);
        }
        else
            return ERR_SEM_TYPE;

    }
    if (exp_type == 2) {
        if (only_one == 1) {
            if ((stack.data_type[stack.top]) == TOK_INT) {
                instr_push(1, 0);
            }
            else if ((stack.data_type[stack.top]) == TOK_DOUBLE) {
                instr_push(2, 0);
            }
            else if (stack.data_type[stack.top] == TOK_STRING_STREAM)
                instr_push(3, 0);
        }
        int type = stack.data_type[stack.top];
       // fprintf (stderr,"pop\n");
        if ((type == TOK_INT) && (func_flag == 2)) {
           // fprintf (stderr,"int pushs\n");
            instr_generator(IO_INT2FLOATS, variable, variable, NULL, 1, 0, 0);
        }
        else if ((type == TOK_INT) && (func_flag == 1)) {
           // fprintf (stderr,"nic konverzia\n");
        }
        else if ((type == TOK_DOUBLE) && (func_flag == 2)) {
           // fprintf(stderr,"niic konverzia\n");
        }        
        else if ((type == TOK_DOUBLE) && (func_flag == 1)) {
            instr_generator(IO_FLOAT2R2EINTS, variable, variable, NULL, 2, 0, 0);
        }  
        else if ((type == TOK_STRING_STREAM) && (func_flag == 3)) {
            //fprintf(stderr,"niic konverzia\n");
        }
        else {
          //  fprintf(stderr,"incompatible types in assigment result\n");
            return ERR_SEM_TYPE;
        }
    }      
    if ((exp_type == 4) && (func_flag == 0)) {
        if (only_one == 1) {
            if ((stack.data_type[stack.top]) == TOK_INT) {
                instr_push(1, 0);
            }
            else if ((stack.data_type[stack.top]) == TOK_DOUBLE) {
                instr_push(2, 0);
            }
            else if (stack.data_type[stack.top] == TOK_STRING_STREAM) {
                char *s_var;
                if (stack.is_variable[stack.top] == 1) {
                    s_var = (char *) stack.value[stack.top];
                    if (func_name != NULL) { 
            tmp_item = STFind(&(func_name->item_data.function->local_table), s_var);
            if (tmp_item == NULL) {
                tmp_item = STFind(&GST, s_var);
                if (tmp_item == NULL) {
                  //  fprintf(stderr,"undeclared variable for asigment\n");
                    return ERR_SEM;
                } 
            }
        }
        else {
            tmp_item = STFind(&GST, s_var);
            if (tmp_item == NULL) {
              //  fprintf(stderr,"undeclared variable for asigment\n");
                return ERR_SEM;
            } 
        } 
                    s_var = (char *) stack.value[stack.top];
                    tmp_ins = (char *) malloc((strlen((char *) s_var) + 1) * sizeof(char));
                    strcpy(tmp_ins, (char*) s_var);
                    instr_generator(IO_ASS, "\045print_temp_variable2", tmp_ins, NULL, 4, 1, tmp_item->item_data.variable.varFrame);
                }
                else if (stack.value[stack.top] != NULL) {
                  //  fprintf(stderr,"generovanie print\n");
                    s_var = (char *) stack.value[stack.top];
                    tmp_ins = (char *) malloc((strlen((char *) s_var) + 1) * sizeof(char));
                    strcpy(tmp_ins, (char*) s_var);
                    instr_generator(IO_ASS, "\045print_temp_variable2", tmp_ins, NULL, 3, 1, 0);
                }
            }    
        }
        int type = stack.data_type[stack.top];
      //  fprintf (stderr,"pop\n");
        if (type == TOK_INT) {
          //  fprintf (stderr,"int pushs\n");
            instr_generator(IO_POPS, "\045print_temp_variable", NULL, NULL, 1, 1, 0);
            instr_generator(IO_PRINT,"\045print_temp_variable", NULL, NULL, 4, 1, 0);
        }
        else if (type == TOK_DOUBLE) {
            instr_generator(IO_POPS, "\045print_temp_variable1", NULL, NULL, 2, 1, 0);
            instr_generator(IO_PRINT,"\045print_temp_variable1", NULL, NULL, 4, 1, 0);
        }
            
        else if (type == TOK_STRING_STREAM) {
            instr_generator(IO_PRINT,"\045print_temp_variable2", NULL, NULL, 4, 1, 0);
        }        
        else {
            fprintf(stderr,"incompatible types in assigment result\n");
            return ERR_SEM_TYPE;
        }  



    } 
   // fprintf(stderr,"dosiel3\n"); 
    SFree(&stack);
  //  fprintf(stderr,"stderr,ko\n");
    return OK;

}

int instr_push(int type, int logical){ // 1 je int, 2 je double, 3 je string, 4 je ked 1.operand je double 2.operand je int, 5 je ked 1.operand je int, druhy je double
   // fprintf(stderr,"push\n");
    char *s_var;
    int not_null_control = 1;
    if (only_one == 1)  {
        not_null_control = 0;
    }
    if (only_one != 1) {
    if (stack.value[stack.top] != NULL)
        not_null_control = 0;
    if (stack.value[stack.top - 2] == NULL)
        not_null_control = 1;
    if (stack.value[stack.top-2] != NULL) {
    if (stack.is_variable[stack.top - 2] == 1){
      //  fprintf(stderr,"variable fasa\n");
        s_var = (char *) stack.value[stack.top - 2];
        if (func_name != NULL) { 
            tmp_item = STFind(&(func_name->item_data.function->local_table), s_var);
            if (tmp_item == NULL) {
                tmp_item = STFind(&GST, s_var);
                if (tmp_item == NULL) {
                   // fprintf(stderr,"undeclared variable for asigment\n");
                    return ERR_SEM;
                } 
            }
        }
        else {
            tmp_item = STFind(&GST, s_var);
            if (tmp_item == NULL) {
              //  fprintf(stderr,"undeclared variable for asigment\n");
                return ERR_SEM;
            } 
        } 
        switch(type){
            case 1:
                //var = tmp_item->item_data.variable.value.v_int;
                tmp_ins = (char *) malloc((strlen((char *) s_var) + 1) * sizeof(char));
                strcpy(tmp_ins, (char *) s_var);
                if (logical == 1 && not_null_control == 1) {
                    instr_generator(IO_POPS, "\045print_temp_variable", NULL,NULL,1,1,0);
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable", NULL, NULL, 4, 1, 0);
                }
                else if (logical == 2 && not_null_control == 1) {
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable", NULL, NULL, 4, 1, 0);
                }
                else
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                if (dividing == 1) {
                    instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 1, 0, 0);
                }
                break;
            case 2:
                //d_var = tmp_item->item_data.variable.value.v_double;
                tmp_ins = (char *) malloc((strlen((char *) s_var) + 1) * sizeof(char));
                strcpy(tmp_ins, (char *) s_var);
                if (logical == 1 && not_null_control == 1) {
                    instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                }
                else if (logical == 2 && not_null_control == 1) {
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                }
                else 
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                break;
            case 3:
                //s_var = tmp_item->item_data.variable.value.v_string;
                tmp_ins = (char *) malloc((strlen((char *) s_var) + 1) * sizeof(char));
                strcpy(tmp_ins, (char *) s_var);
                if (logical == 1 && not_null_control == 1) {
                    instr_generator(IO_POPS, "\045print_temp_variable2", NULL,NULL,3,1,0);
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable2", NULL, NULL, 4, 1, 0);
                }
                else if (logical == 2 && not_null_control == 1) {
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable2", NULL, NULL, 4, 1, 0);
                }
                else
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);  
                break;  
            case 4:
                //var = tmp_item->item_data.variable.value.v_int;
                tmp_ins = (char *) malloc((strlen((char *) s_var) + 1) * sizeof(char));
                strcpy(tmp_ins, (char *) s_var);
                if (logical == 1 && not_null_control == 1) {
                    instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                    instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 2, 0, 0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                }
                else if (logical == 2 && not_null_control == 1) {
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                    instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 2, 0, 0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                }
                else {
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                    instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 2, 0, 0); // 2 tam je ale v podstate nic nespravi lebo instrukcia INT2FLOATS neziada typ
                }    
                break;
            case 5:
                //d_var = tmp_item->item_data.variable.value.v_double;
                tmp_ins = (char *) malloc((strlen((char *) s_var) + 1) * sizeof(char));
                strcpy(tmp_ins, (char *) s_var);
                if (logical == 1 && not_null_control == 1) {
                    instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 1, 0, 0);
                    instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                }
                else if (logical == 2 && not_null_control == 1) {
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                }
                else if (not_null_control == 1) {
                    instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 1, 0, 0);
                    instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                }
                else
                   instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0); 
                break;
        }
    }
    else {
        if (stack.value[stack.top - 2] != NULL) {
         //   fprintf(stderr,"tu push\n");
           // fprintf (stderr,"Type v exp je %d\n", type);
            switch(type){
                case 1:
                   // fprintf (stderr,"tu case\n");
                    //fprintf (stderr,"premenna je %d\n",var);
                    tmp_ins = (char *) malloc((strlen((char *) stack.value[stack.top - 2]) + 1) * sizeof(char));
                    strcpy(tmp_ins, (char *) stack.value[stack.top - 2]);
                    //fprintf (stderr,"tmp je %s\n",tmp_ins);
                    if (logical == 1 && not_null_control == 1) {
                        instr_generator(IO_POPS, "\045print_temp_variable", NULL,NULL,1,1,0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 1, 0, 0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable", NULL, NULL, 4, 1, 0);
                    }
                    else if (logical == 2 && not_null_control == 1) {
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 1, 0, 0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable", NULL, NULL, 4, 1, 0);
                    }
                    else
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 1, 0, 0);
                    if (dividing == 1) {
                    instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 1, 0, 0);
                }
                   // fprintf (stderr,"premenna po generatore je %d\n", atoi((char*)list.Last->instr.adr1));
                    list.Act = list.First;
                    do {
                        if (list.Act->instr.insOp == IO_PUSHS) {
                            //fprintf (stderr,"premenna po generatore 2 je %d\n", atoi((char*)list.Act->instr.adr1));
                        }
                        list.Act = list.Act->next;
                    }while(list.Act != NULL);
                    break;
                case 2:
                    tmp_ins = (char *) malloc((strlen((char *) stack.value[stack.top - 2]) + 1) * sizeof(char));
                    strcpy(tmp_ins, (char *) stack.value[stack.top - 2]);
                     if (logical == 1 && not_null_control == 1) {
                        instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 2, 0, 0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                    }
                    else if (logical == 2 && not_null_control == 1) {
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 2, 0, 0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                    }
                    else
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 2, 0, 0);
                    break;
                case 3:
                    s_var = (char *)stack.value[stack.top - 2];
                    tmp_ins = (char *) malloc((strlen((char *) stack.value[stack.top - 2]) + 1) * sizeof(char));
                    strcpy(tmp_ins, (char *) stack.value[stack.top - 2]);
                    if (logical == 1 && not_null_control == 1) {
                        instr_generator(IO_POPS, "\045print_temp_variable2", NULL,NULL,3,1,0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 3, 0, 0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable2", NULL, NULL, 4, 1, 0);
                    }
                    else if (logical == 2 && not_null_control == 1) {
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 3, 0, 0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable2", NULL, NULL, 4, 1, 0);
                    }
                    else
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 3, 0, 0);
                    break;
                case 4:
                    tmp_ins = (char *) malloc((strlen((char *) stack.value[stack.top - 2]) + 1) * sizeof(char));
                    strcpy(tmp_ins, (char *) stack.value[stack.top - 2]);
                     if (logical == 1 && not_null_control == 1) {
                        instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 1, 0, 0);
                        instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 2, 0, 0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                    }
                    else if (logical == 2 && not_null_control == 1) {
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 1, 0, 0);
                        instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 2, 0, 0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                    }
                    else {
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 1, 0, 0);
                        instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 2, 0, 0);
                    }
                    break;
                case 5:
                    tmp_ins = (char *) malloc((strlen((char *) stack.value[stack.top - 2]) + 1) * sizeof(char));
                    strcpy(tmp_ins, (char *) stack.value[stack.top - 2]);
                    if (logical == 1 && not_null_control == 1) {
                        instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 0, 0, 0);
                        instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                         instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 2, 0, 0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                    }
                    else if (logical == 2 && not_null_control == 1) {
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 2, 0, 0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                    }
                     else if (not_null_control == 1) {
                        instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 1, 0, 0);
                        instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 2, tmp_item->item_data.variable.varFrame, 0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                    }
                    else
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 2, 0, 0);
                    break;
            }
        }    
    }
}
}
        //fprintf("stderr,only one\n");
  
    if (stack.value[stack.top] != NULL) {
    if (stack.is_variable[stack.top] == 1){
        s_var = (char *) stack.value[stack.top];
        if (func_name != NULL) { 
            tmp_item = STFind(&(func_name->item_data.function->local_table), s_var);
            if (tmp_item == NULL) {
                tmp_item = STFind(&GST, s_var);
                if (tmp_item == NULL) {
                  //  fprintf(stderr,"undeclared variable for asigment\n");
                    return ERR_SEM;
                } 
            }
        }
        else {
            tmp_item = STFind(&GST, s_var);
            if (tmp_item == NULL) {
              //  fprintf(stderr,"undeclared variable for asigment\n");
                return ERR_SEM;
            } 
        } 
        switch(type){
            case 1:
                //var = tmp_item->item_data.variable.value.v_int;
                //fprintf (stderr,"premenna je %d\n",var);
                tmp_ins = (char *) malloc((strlen((char *) s_var) + 1) * sizeof(char));
                strcpy(tmp_ins, (char *) s_var);
                if (logical == 1 && not_null_control == 1) {
                    instr_generator(IO_POPS, "\045print_temp_variable", NULL,NULL,1,1,0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable", NULL, NULL, 4, 1, 0);
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                }
                else if (logical == 2 && not_null_control == 1) {
                    instr_generator(IO_PUSHS, "\045print_temp_variable", NULL, NULL, 4, 1, 0);
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                }
                else
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                if (dividing == 1) {
                    instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 1, 0, 0);
                }
                break;
            case 2:
                //d_var = tmp_item->item_data.variable.value.v_double;
                tmp_ins = (char *) malloc((strlen((char *) s_var) + 1) * sizeof(char));
                strcpy(tmp_ins, (char *) s_var);
                if (logical == 1 && not_null_control == 1) {
                    instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                    
                }
                else if (logical == 2 && not_null_control == 1) {
                    instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                }
                else 
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                break;
            case 3:
                //s_var = tmp_item->item_data.variable.value.v_string; 
                tmp_ins = (char *) malloc((strlen((char *) s_var) + 1) * sizeof(char));
                strcpy(tmp_ins, (char *) s_var);
                if (logical == 1 && not_null_control == 1) {
                    instr_generator(IO_POPS, "\045print_temp_variable2", NULL,NULL,3,1,0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable2", NULL, NULL, 4, 1, 0);
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                }
                else if (logical == 2 && not_null_control == 1) {
                    instr_generator(IO_PUSHS, "\045print_temp_variable2", NULL, NULL, 4, 1, 0);
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                }
                else
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);  
                break;
            case 4:
                //d_var = tmp_item->item_data.variable.value.v_double;
                tmp_ins = (char *) malloc((strlen((char *) s_var) + 1) * sizeof(char));
                strcpy(tmp_ins, (char *) s_var);
                if (logical == 1 && not_null_control == 1) {
                    instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 1, 0, 0);
                    instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                }
                else if (logical == 2 && not_null_control == 1) {
                    instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                }
                 else if (not_null_control == 1) {
                    instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 1, 0, 0);
                    instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                    instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                }
                else {
                    instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, tmp_item->item_data.variable.varFrame, 0);
                }    
                break;
            case 5:
                //var = tmp_item->item_data.variable.value.v_int;
                tmp_ins = (char *) malloc((strlen((char *) s_var) + 1) * sizeof(char));
                strcpy(tmp_ins, (char *) s_var);
                 if (logical == 1 && not_null_control == 1) {
                        instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, 1, 0);
                        instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 2, 0, 0);
                    }
                    else if (logical == 2 && not_null_control == 1) {
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, 1, 0);
                        instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 2, 0, 0);
                    }
                    else {
                        fprintf(stderr,"Tu pushujem %s\n", tmp_ins);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 4, 1, 0);
                        instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 2, 0, 0);
                    }
                break;
        }
    }
    else {
        if (stack.value[stack.top] != NULL) {
            switch(type){
                case 1:
                   // fprintf (stderr,"premenna je %d\n",var);
                    tmp_ins = (char *) malloc((strlen((char *) stack.value[stack.top]) + 1) * sizeof(char));
                    strcpy(tmp_ins, (char *) stack.value[stack.top]);
                    if (only_one == 1 && logical == 3) {
                        if ((strcmp(tmp_ins, "1")) != 0) {
                            if ((strcmp(tmp_ins, "0")) != 0) {
                                return ERR_SEM_TYPE;
                            }
                        }
                    }
                    if (logical == 1 && not_null_control == 1) {
                        instr_generator(IO_POPS, "\045print_temp_variable", NULL,NULL,1,1,0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable", NULL, NULL, 4, 1, 0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 1, 0, 0);
                    }
                    else if (logical == 2 && not_null_control == 1) {
                        instr_generator(IO_PUSHS, "\045print_temp_variable", NULL, NULL, 4, 1, 0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 1, 0, 0);
                    }
                    else
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 1, 0, 0);
                    if (dividing == 1) {
                    instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 1, 0, 0);
                }
                    break;
                case 2:
                    tmp_ins = (char *) malloc((strlen((char *) stack.value[stack.top]) + 1) * sizeof(char));
                    strcpy(tmp_ins, (char *) stack.value[stack.top]);
                    if (logical == 1 && not_null_control == 1) {
                        instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 2, 0, 0);
                    }
                    else if (logical == 2 && not_null_control == 1) {
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 2, 0, 0);
                    }
                    else
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 2, 0, 0);
                    break;
                case 3:
                    s_var = (char *)stack.value[stack.top];
                    tmp_ins = (char *) malloc((strlen((char *) stack.value[stack.top]) + 1) * sizeof(char));
                    strcpy(tmp_ins, (char *) stack.value[stack.top]);
                    if (logical == 1 && not_null_control == 1) {
                        instr_generator(IO_POPS, "\045print_temp_variable2", NULL,NULL,3,1,0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable2", NULL, NULL, 4, 1, 0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 3, 0, 0);
                    }
                    else if (logical == 2 && not_null_control == 1) {
                        instr_generator(IO_PUSHS, "\045print_temp_variable2", NULL, NULL, 4, 1, 0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 3, 0, 0);
                    }
                    else
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 3, 0, 0);
                    break;
                case 4:
                    tmp_ins = (char *) malloc((strlen((char *) stack.value[stack.top]) + 1) * sizeof(char));
                    strcpy(tmp_ins, (char *) stack.value[stack.top]);
                    if (logical == 1 && not_null_control == 1) {
                        instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 1, 0, 0);
                        instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 2, 0, 0);
                    }
                    else if (logical == 2 && not_null_control == 1) {
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 2, 0, 0);
                    }
                     else if (not_null_control == 1) {
                        instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 1, 0, 0);
                        instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 2, 0, 0);
                    }
                    else {
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 2, 0, 0);
                    }
                    break;
                case 5:
                    tmp_ins = (char *) malloc((strlen((char *) stack.value[stack.top]) + 1) * sizeof(char));
                    strcpy(tmp_ins, (char *) stack.value[stack.top]);
                    if (logical == 1 && not_null_control == 1) {
                        instr_generator(IO_POPS, "\045print_temp_variable1", NULL,NULL,2,1,0);
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 1, 0, 0);
                        instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 2, 0, 0);
                    }
                    else if (logical == 2 && not_null_control == 1) {
                        instr_generator(IO_PUSHS, "\045print_temp_variable1", NULL, NULL, 4, 1, 0);
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 1, 0, 0);
                        instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 2, 0, 0);
                    }
                    else {
                        instr_generator(IO_PUSHS, tmp_ins, NULL, NULL, 1, 0, 0);
                        instr_generator(IO_INT2FLOATS, NULL, NULL, NULL, 2, 0, 0);
                    }
                    break;
            }
       } 
    }
}

    return OK;
    
}


int reduce (int exp_type) {
    int i = 0;
    char *s_var;
    char *s1_var;
    if (rules(4, E_NT, E_NT, TOK_PLUS, E_NT)){
        only_one = 0;
        if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top - 2] == TOK_INT)){
            instr_push(1, 1);
            instr_generator(IO_ADDS, NULL, NULL, NULL, 1, 0, 0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_INT);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
            // treba vyprazdnit stack
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top - 2] == TOK_DOUBLE)){
            instr_push(2, 1);
            instr_generator(IO_ADDS, NULL, NULL, NULL, 2, 0, 0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_DOUBLE);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_STRING_STREAM) && (stack.data_type[stack.top - 2] == TOK_STRING_STREAM)){
             if (stack.is_variable[stack.top] == 1) {
                s_var = (char *) stack.value[stack.top];
                if (func_name != NULL) { 
            tmp_item = STFind(&(func_name->item_data.function->local_table), s_var);
            if (tmp_item == NULL) {
                tmp_item = STFind(&GST, s_var);
                if (tmp_item == NULL) {
                   // fprintf(stderr,"undeclared variable for asigment\n");
                    return ERR_SEM;
                } 
            }
        }
        else {
            tmp_item = STFind(&GST, s_var);
            if (tmp_item == NULL) {
               // fprintf(stderr,"undeclared variable for asigment\n");
                return ERR_SEM;
            } 
        } 
                s_var = tmp_item->item_data.variable.value.v_string;
            }
            else {
                s_var = (char *) stack.value[stack.top];
            }
            if (stack.is_variable[stack.top - 2] == 1) {
                s1_var = (char *) stack.value[stack.top - 2];
                if (func_name != NULL) { 
            tmp_item = STFind(&(func_name->item_data.function->local_table), s_var);
            if (tmp_item == NULL) {
                tmp_item = STFind(&GST, s_var);
                if (tmp_item == NULL) {
                  //  fprintf(stderr,"undeclared variable for asigment\n");
                    return ERR_SEM;
                } 
            }
        }
        else {
            tmp_item = STFind(&GST, s_var);
            if (tmp_item == NULL) {
               // fprintf(stderr,"undeclared variable for asigment\n");
                return ERR_SEM;
            } 
        } 
                s1_var = tmp_item->item_data.variable.value.v_string;
            }
            else {
                s1_var = (char *) stack.value[stack.top - 2];
            }
            instr_generator(IO_CONCAT, s_var, s1_var, NULL, /*"s_var"*/ 3, 0, 0); // treba vymysliet -> maybe vymysliet specialnu premennu a ulozit do globalsymtab ->sluzi iba pre concat 
            //instr_generator(IO_PUSHS, s_var, NULL, NULL, 3);
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top - 2] == TOK_INT)){
            instr_push(4, 1);
            instr_generator(IO_ADDS, NULL, NULL, NULL, 2, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_DOUBLE);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top - 2] == TOK_DOUBLE)){
            instr_push(5, 1);
            instr_generator(IO_ADDS, NULL, NULL, NULL, 2, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_DOUBLE);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else
            return ERR_SEM_TYPE;
    }


    if (rules(4, E_NT, E_NT, TOK_MINUS, E_NT)){ 
        only_one = 0;
        if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top - 2] == TOK_INT)){
            instr_push(1, 1);
            instr_generator(IO_SUBS, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_INT);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top - 2] == TOK_DOUBLE)){
            instr_push(2, 1);
            instr_generator(IO_SUBS, NULL, NULL, NULL, 2, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_DOUBLE);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top - 2] == TOK_INT)){
            instr_push(4, 1);
            instr_generator(IO_SUBS, NULL, NULL, NULL, 2, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_DOUBLE);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top - 2] == TOK_DOUBLE)){
            instr_push(5, 1);
            instr_generator(IO_SUBS, NULL, NULL, NULL, 2, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_DOUBLE);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else
            return ERR_SEM_TYPE;
    }

    if (rules(4, E_NT, E_NT, TOK_MULTIPLY, E_NT)){ // treba pretypovat?
        only_one = 0;
        if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top - 2] == TOK_INT)){
            instr_push(1, 1);
            instr_generator(IO_MULS, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_INT);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top - 2] == TOK_DOUBLE)){
            instr_push(2, 1);
            instr_generator(IO_MULS, NULL, NULL, NULL, 2, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_DOUBLE);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top - 2] == TOK_INT)){
            instr_push(4, 1);
            instr_generator(IO_MULS, NULL, NULL, NULL, 2, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_DOUBLE);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top - 2] == TOK_DOUBLE)){
            instr_push(5, 1);
            instr_generator(IO_MULS, NULL, NULL, NULL, 2, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_DOUBLE);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else
            return ERR_SEM_TYPE;
    }

    if (rules(4, E_NT, E_NT, TOK_DIVIDE, E_NT)){
        only_one = 0;
        dividing = 1;
        if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top - 2] == TOK_INT)){
            instr_push(1, 1);
            instr_generator(IO_DIVS, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_DOUBLE);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            dividing = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top - 2] == TOK_DOUBLE)){
            instr_push(2, 1);
            instr_generator(IO_DIVS, NULL, NULL, NULL, 2, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_DOUBLE);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            dividing = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top - 2] == TOK_INT)){
            instr_push(4,1);
            instr_generator(IO_DIVS, NULL, NULL, NULL, 2, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_DOUBLE);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            dividing = 0;
            return OK;
        }

        else if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top - 2] == TOK_DOUBLE)){
            instr_push(5, 1);
            instr_generator(IO_DIVS, NULL, NULL, NULL, 2, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_DOUBLE);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            dividing = 0;
            return OK;
        }
        else
            return ERR_SEM_TYPE;
    }

    if (rules(4, E_NT, E_NT, TOK_WHOLENUMBER_DIVIDE, E_NT)){
        only_one = 0;
        dividing = 1;
        if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top - 2] == TOK_INT)){
            instr_push(1, 1);
            instr_generator(IO_DIVS, NULL, NULL, NULL, 1, 0 ,0);
            instr_generator(IO_FLOAT2R2EINTS, NULL, NULL, NULL, 1, 0, 0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, TOK_INT);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            dividing = 0;
            return OK;
        }
        else
            return ERR_SEM_TYPE;
    }

    else if (rules(4, E_NT, TOK_RIGHT_RB, E_NT, TOK_LEFT_RB)) {
        int e_value = stack.data_type[stack.top-1];
        void *tmp_void = stack.value[stack.top-1];
        int tmp_is_variable = stack.is_variable[stack.top-1];
        for (i = 0;i < 4;i++)
            STopPop(&stack);
        SPush(&stack, E_NT, e_value);
        stack.value[stack.top] = tmp_void;
        stack.is_variable[stack.top] = tmp_is_variable;

        return OK;
    }
    else if (rules(4, E_NT, E_NT, TOK_LOWER, E_NT)) {
        only_one = 0;
        if ((exp_type == 1) || (exp_type == 2)) {
            fprintf(stderr,"syntax err\n");
            return ERR_SYN;
        }
        if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top-2] == TOK_INT)) {
                instr_push(1, 1);
                instr_generator(IO_L, NULL, NULL, NULL, 1, 0 ,0);
                for (i = 0;i < 4;i++)
                    STopPop(&stack);
                SPush(&stack, E_NT, STATE_BOOL);
                stack.value[stack.top] = NULL;
                stack.is_variable[stack.top] = 0;
                return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top-2] == TOK_DOUBLE)) {
            instr_push(2, 1);
            instr_generator(IO_L, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top - 2] == TOK_INT)) {
            instr_push(4, 1);
            instr_generator(IO_L, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top - 2] == TOK_DOUBLE)) {
            instr_push(5, 1);
            instr_generator(IO_L, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_STRING_STREAM) && (stack.data_type[stack.top - 2] == TOK_STRING_STREAM)) {
            instr_push(3, 1);
            instr_generator(IO_L, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else {
          //  fprintf(stderr,"Error semanticky expressions\n");
            return ERR_SEM_TYPE;
        }
    }    
    else if (rules(4, E_NT, E_NT, TOK_GREATER, E_NT)) {
        only_one = 0;
        if ((exp_type == 1) || (exp_type == 2)) {
           // fprintf(stderr,"syntax err\n");
            return ERR_SYN;
        }
        if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top-2] == TOK_INT)) {
                instr_push(1, 1);
                instr_generator(IO_G, NULL, NULL, NULL, 1, 0 ,0);
                for (i = 0;i < 4;i++)
                    STopPop(&stack);
                SPush(&stack, E_NT, STATE_BOOL);
                stack.value[stack.top] = NULL;
                stack.is_variable[stack.top] = 0;
                return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top-2] == TOK_DOUBLE)) {
            instr_push(2, 1);
            instr_generator(IO_G, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top - 2] == TOK_INT)) {
            instr_push(4, 1);
            instr_generator(IO_G, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top - 2] == TOK_DOUBLE)) {
            instr_push(5, 1);
            instr_generator(IO_G, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_STRING_STREAM) && (stack.data_type[stack.top - 2] == TOK_STRING_STREAM)) {
            instr_push(3, 1);
            instr_generator(IO_G, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else {
          //  fprintf(stderr,"Error semanticky expressions\n");
            return ERR_SEM_TYPE;
        }
       
    }   
    else if (rules(4, E_NT, E_NT, TOK_GREATER_EQ, E_NT)) {
        only_one = 0;
         if ((exp_type == 1) || (exp_type == 2)) {
          //  fprintf(stderr,"syntax err\n");
            return ERR_SYN;
        }
        if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top-2] == TOK_INT)) {
              //  fprintf(stderr,"Padine operatory\n");
                instr_push(1, 1);
                instr_generator(IO_G, NULL, NULL, NULL, 1, 0 ,0);
                instr_push(1, 2);
                instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
                instr_generator(IO_OR_S, NULL, NULL, NULL, 1, 0 ,0);
                for (i = 0;i < 4;i++)
                    STopPop(&stack);
                SPush(&stack, E_NT, STATE_BOOL);
                stack.value[stack.top] = NULL;
                stack.is_variable[stack.top] = 0;
                return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top-2] == TOK_DOUBLE)) {
            instr_push(2, 1);
            instr_generator(IO_G, NULL, NULL, NULL, 1, 0 ,0);
            instr_push(2, 2);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            instr_generator(IO_OR_S, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top - 2] == TOK_INT)) {
            instr_push(4, 1);
            instr_generator(IO_G, NULL, NULL, NULL, 1, 0 ,0);
            instr_push(4, 2);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            instr_generator(IO_OR_S, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top - 2] == TOK_DOUBLE)) {
            instr_push(5, 1);
            instr_generator(IO_G, NULL, NULL, NULL, 1, 0 ,0);
            instr_push(5, 2);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            instr_generator(IO_OR_S, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_STRING_STREAM) && (stack.data_type[stack.top - 2] == TOK_STRING_STREAM)) {
            instr_push(3, 1);
            instr_generator(IO_G, NULL, NULL, NULL, 1, 0 ,0);
            instr_push(3, 2);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            instr_generator(IO_OR_S, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else {
          //  fprintf(stderr,"Error semanticky expressions\n");
            return ERR_SEM_TYPE;
        }
    }   
    else if (rules(4, E_NT, E_NT, TOK_LOWER_EQ, E_NT)) {
        only_one = 0;
         if ((exp_type == 1) || (exp_type == 2)) {
          //  fprintf(stderr,"syntax err\n");
            return ERR_SYN;
        }
        if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top-2] == TOK_INT)) {
                instr_push(1, 1);
                instr_generator(IO_L, NULL, NULL, NULL, 1, 0 ,0);
                instr_push(1, 2);
                instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
                instr_generator(IO_OR_S, NULL, NULL, NULL, 1, 0 ,0);
                for (i = 0;i < 4;i++)
                    STopPop(&stack);
                SPush(&stack, E_NT, STATE_BOOL);
                stack.value[stack.top] = NULL;
                stack.is_variable[stack.top] = 0;
                return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top-2] == TOK_DOUBLE)) {
            instr_push(2, 1);
            instr_generator(IO_L, NULL, NULL, NULL, 1, 0 ,0);
            instr_push(2, 2);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            instr_generator(IO_OR_S, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top - 2] == TOK_INT)) {
            instr_push(4, 1);
            instr_generator(IO_L, NULL, NULL, NULL, 1, 0 ,0);
            instr_push(4, 2);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            instr_generator(IO_OR_S, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top - 2] == TOK_DOUBLE)) {
            instr_push(5, 1);
            instr_generator(IO_L, NULL, NULL, NULL, 1, 0 ,0);
            instr_push(5, 2);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            instr_generator(IO_OR_S, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_STRING_STREAM) && (stack.data_type[stack.top - 2] == TOK_STRING_STREAM)) {
            instr_push(3, 1);
            instr_generator(IO_L, NULL, NULL, NULL, 1, 0 ,0);
            instr_push(3, 2);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            instr_generator(IO_OR_S, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else {
           // fprintf(stderr,"Error semanticky expressions\n");
            return ERR_SEM_TYPE;
        }
    }   
    else if (rules(4, E_NT, E_NT, TOK_NOT_EQUAL, E_NT)) {
        only_one = 0;
        if ((exp_type == 1) || (exp_type == 2)) {
          //  fprintf(stderr,"syntax err\n");
            return ERR_SYN;
        }
        if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top-2] == TOK_INT)) {
                instr_push(1, 0);
                instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
                instr_generator(IO_NOT_S, NULL, NULL, NULL, 1, 0 ,0);
                for (i = 0;i < 4;i++)
                    STopPop(&stack);
                SPush(&stack, E_NT, STATE_BOOL);
                stack.value[stack.top] = NULL;
                stack.is_variable[stack.top] = 0;
                return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top-2] == TOK_DOUBLE)) {
            instr_push(2, 0);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            instr_generator(IO_NOT_S, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top - 2] == TOK_INT)) {
            instr_push(4, 0);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            instr_generator(IO_NOT_S, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top - 2] == TOK_DOUBLE)) {
            instr_push(5, 0);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            instr_generator(IO_NOT_S, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_STRING_STREAM) && (stack.data_type[stack.top - 2] == TOK_STRING_STREAM)) {
            instr_push(3, 0);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            instr_generator(IO_NOT_S, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else {
          //  fprintf(stderr,"Error semanticky expressions\n");
            return ERR_SEM_TYPE;
        }
        
    }   
    else if (rules(4, E_NT, E_NT, TOK_ASSIGN_OR_EQUALS, E_NT)) {
        only_one = 0;
       if ((exp_type == 1) || (exp_type == 2)) {
          //  fprintf(stderr,"syntax err\n");
            return ERR_SYN;
        }
        if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top-2] == TOK_INT)) {
                instr_push(1, 0);
                instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
                for (i = 0;i < 4;i++)
                    STopPop(&stack);
                SPush(&stack, E_NT, STATE_BOOL);
                stack.value[stack.top] = NULL;
                stack.is_variable[stack.top] = 0;
                return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top-2] == TOK_DOUBLE)) {
            instr_push(2, 0);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_DOUBLE) && (stack.data_type[stack.top - 2] == TOK_INT)) {
            instr_push(4, 0);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_INT) && (stack.data_type[stack.top - 2] == TOK_DOUBLE)) {
            instr_push(5, 0);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else if ((stack.data_type[stack.top] == TOK_STRING_STREAM) && (stack.data_type[stack.top - 2] == TOK_STRING_STREAM)) {
            instr_push(3, 0);
            instr_generator(IO_EQ, NULL, NULL, NULL, 1, 0 ,0);
            for (i = 0;i < 4;i++)
                STopPop(&stack);
            SPush(&stack, E_NT, STATE_BOOL);
            stack.value[stack.top] = NULL;
            stack.is_variable[stack.top] = 0;
            return OK;
        }
        else {
           // fprintf(stderr,"Error semanticky expressions\n");
            return ERR_SEM_TYPE;
        }
    }
    else if (rules(2, E_NT, TOK_INT)) {
         stack.value[stack.top] = (void *) token_prev;
        return OK;
    }
    else if (rules(2, E_NT, TOK_DOUBLE)) {
        stack.value[stack.top] = (void *) token_prev;
        return OK;
    }
    else if (rules(2, E_NT, TOK_STRING_STREAM)) {
        stack.value[stack.top] = (void *) token_prev;
        return OK;
    }
    else if (rules(2 , E_NT, TOK_ID)) {
      //  fprintf (stderr,"tu exp\n");
      //  fprintf (stderr,"token prev je %s\n", token_prev);
        if (func_name != NULL) { 
          //  fprintf(stderr,"hladam vo func\n");
            tmp_item = STFind(&(func_name->item_data.function->local_table), token_prev);
            if (tmp_item == NULL) {
                tmp_item = STFind(&GST, token_prev);
                if (tmp_item == NULL) {
                   // fprintf(stderr,"undeclared variable for asigment\n");
                    return ERR_SEM;
                } 
            }
        }
        else {
            tmp_item = STFind(&GST, token_prev);
            if (tmp_item == NULL) {
              //  fprintf(stderr,"undeclared variable for asigment\n");
                return ERR_SEM;
            } 
        } 
        if (tmp_item -> item_type != variable)
            return ERR_SEM;
        if (tmp_item->item_data.variable.initialized == 0)
            return ERR_SEM;
       // fprintf(stderr,"tu exp2\n");
        int type = tmp_item->item_data.variable.data_type;
        if (type == integer) {    
        SPush(&stack, E_NT, TOK_INT);
        }
        else if (type == d_double) {
          SPush(&stack, E_NT, TOK_DOUBLE);  
        }
        else if (type == string) {
            SPush(&stack, E_NT, TOK_STRING_STREAM);
        }
        else {
          //  fprintf (stderr,"fasa id je sem err\n");
            return ERR_SEM;
        }

        stack.value[stack.top] = (void *) token_prev;
        stack.is_variable[stack.top] = 1;
        return OK;
    }



   
    return OK;
}       


int function_call(char *func, FILE *f, int exp_type) {
    STitem *tmp_func;
  //  fprintf (stderr,"Func call\n");
    tmp_item = STFind(&GST, func);
    if (tmp_item == NULL) {
     //   fprintf (stderr,"sem err 1\n");
        return ERR_SEM;
    }
    else if (tmp_item->item_type != function) {
      //  fprintf (stderr,"sem err 2\n");
        return ERR_SEM;
    }
    else if ((tmp_item->item_data.function->declared == 0) && (tmp_item->item_data.function->initialized == 0)) {
      //  fprintf (stderr,"sem err 3\n");
        return ERR_SEM;
    }    
    else {
        if (tmp_item->item_data.function->declared == 1 && tmp_item->item_data.function->initialized == 0)
            tmp_item->item_data.function->declare_call = 1;
        int counter_params = 0;
        int coma_counter = 0;
        int tokNum;
        instr_generator(IO_CREATE_FRAME, NULL, NULL, NULL, 0, 0 ,0);
        while(tokNum != TOK_EOL) {
        tokNum = Token(&token, f);
        if (tokNum == ERR_LEX)
            return ERR_LEX;
        switch(tokNum) {
            case TOK_RIGHT_RB:
                if (counter_params != coma_counter + 1) {
                    if (counter_params != 0 || coma_counter != 0) {
                  //  fprintf (stderr,"counter params je %d coma je %d\n",counter_params, coma_counter);
                  //  fprintf (stderr,"err syn\n");
                    return ERR_SYN;
                    } 
                }
                tokNum = Token(&token, f);
                if (tokNum == ERR_LEX)
                    return ERR_LEX;
                if (tokNum != TOK_EOL) {
                    if (tokNum == TOK_SEMICOLON ) {
                        if (exp_type != 4) {
                  //  fprintf (stderr,"Eol syn func\n");
                    return ERR_SYN;
                    }
                 }
                 else {
                  //  fprintf (stderr,"Eol syn func\n");
                    return ERR_SYN;
                 }
                }
                if (tmp_item->item_data.function->num_of_args != counter_params)
                    return ERR_SEM_TYPE;
                char *call_ins;
                call_ins = (char *) malloc ((strlen(func) + 2) * sizeof(char));
                sprintf(call_ins, "%%%s", func);
              //  fprintf (stderr,"call_ins je %s\n", call_ins);
                instr_generator(IO_CALL, (void *) call_ins, NULL, NULL, 0, 0 ,0);
              //  fprintf (stderr,"v poriadku\n");
                return OK;
            case TOK_INT:
                if ((counter_params + 1) > (tmp_item->item_data.function->num_of_args))
                    return ERR_SEM_TYPE;
                tmp_func = STFind(&(tmp_item->item_data.function->local_table), tmp_item->item_data.function->args[counter_params].param_name);
                tmp_func->item_data.variable.varFrame = 2;
                if (tmp_func == NULL) {
                   // fprintf(stderr,"Error with parameters\n");
                    return ERR_SEM;
                }
                if (coma_counter != counter_params) {
                  //  fprintf (stderr,"Err counters func\n");
                    return ERR_SYN;
                }
                if (tmp_item->item_data.function->num_of_args == counter_params) {
                  //  fprintf (stderr,"spatny pocet parametru funkce \n");
                    return ERR_SEM_TYPE;
                }
                else {
                    if (tmp_item->item_data.function->args[counter_params].param_type == integer) {
                        instr_generator(IO_VAR_DEF, (void *) tmp_item->item_data.function->args[counter_params].param_name, NULL, NULL, 1, 3, 0);
                        tmp_ins = (char *) malloc((strlen((char *) token) + 1) * sizeof(char));
                        strcpy(tmp_ins, (char *) token);
                        instr_generator(IO_ASS, (void *) tmp_item->item_data.function->args[counter_params].param_name, (void *) tmp_ins, NULL, 1, 3, 0);
                    
                    }    
                    else if (tmp_item->item_data.function->args[counter_params].param_type == d_double) {
                        instr_generator(IO_VAR_DEF, (void *) tmp_item->item_data.function->args[counter_params].param_name, NULL, NULL, 2, 3, 0);
                        tmp_ins = (char *) malloc((strlen((char *) token) + 1) * sizeof(char));
                        strcpy(tmp_ins, (char *) token);
                        instr_generator(IO_INT2FLOAT, (void *) tmp_item->item_data.function->args[counter_params].param_name, (void *) tmp_ins, NULL, 1, 3, 0);
                        
                    } 
                    else {
                      //  fprintf (stderr,"Spatny typ parametru funkce\n");
                        return ERR_SEM_TYPE;
                    }   
                    counter_params++;
                    break;

                }
            case TOK_DOUBLE:
                if ((counter_params + 1) > (tmp_item->item_data.function->num_of_args))
                    return ERR_SEM_TYPE;
                tmp_func = STFind(&(tmp_item->item_data.function->local_table), tmp_item->item_data.function->args[counter_params].param_name);
                if (tmp_func == NULL) {
                  //  fprintf(stderr,"Error with parameters\n");
                    return ERR_SEM;
                }
                tmp_func->item_data.variable.varFrame = 2;
                if (coma_counter != counter_params) {
                  //  fprintf (stderr,"Errs counter function 2\n");
                    return ERR_SYN;
                }
                if (tmp_item->item_data.function->num_of_args == counter_params) {
                   // fprintf (stderr,"spatny pocet parametru funkce \n");
                    return ERR_SEM_TYPE;
                }
                else {
                    if (tmp_item->item_data.function->args[counter_params].param_type == integer) {
                        instr_generator(IO_VAR_DEF, (void *) tmp_item->item_data.function->args[counter_params].param_name, NULL, NULL, 1, 3, 0);
                        tmp_ins = (char *) malloc((strlen((char *) token) + 1) * sizeof(char));
                        strcpy(tmp_ins, (char *) token);
                        instr_generator(IO_FLOAT2R2EINT, (void *) tmp_item->item_data.function->args[counter_params].param_name, (void *) tmp_ins, NULL, 1, 3, 0);
                       
                    }    
                    else if (tmp_item->item_data.function->args[counter_params].param_type == d_double) {
                        instr_generator(IO_VAR_DEF, (void *) tmp_item->item_data.function->args[counter_params].param_name, NULL, NULL, 2, 3, 0);
                        tmp_ins = (char *) malloc((strlen((char *) token) + 1) * sizeof(char));
                        strcpy(tmp_ins, (char *) token);
                        instr_generator(IO_ASS, (void *) tmp_item->item_data.function->args[counter_params].param_name, (void *) tmp_ins, NULL, 2, 3, 0);
                        
                    } 
                    else {
                      //  fprintf (stderr,"Spatny typ parametru funkce\n");
                        return ERR_SEM_TYPE;
                    }   
                    counter_params++;
                    break;
                }       
            case TOK_STRING_STREAM:
                if ((counter_params + 1) > (tmp_item->item_data.function->num_of_args))
                    return ERR_SEM_TYPE;
                tmp_func = STFind(&(tmp_item->item_data.function->local_table), tmp_item->item_data.function->args[counter_params].param_name);
                if (tmp_func == NULL) {
                  //  fprintf(stderr,"Error with parameters\n");
                    return ERR_SEM;
                }
                tmp_func->item_data.variable.varFrame = 2;
                if (coma_counter != counter_params) {
                  //  fprintf (stderr,"Errs counter func 3\n");
                    return ERR_SYN;
                }
                if (tmp_item->item_data.function->num_of_args == counter_params) {
                  //  fprintf (stderr,"spatny pocet parametru funkce \n");
                    return ERR_SEM_TYPE;
                }
                else {
                    if (tmp_item->item_data.function->args[counter_params].param_type == string) {
                        instr_generator(IO_VAR_DEF, (void *) tmp_item->item_data.function->args[counter_params].param_name, NULL, NULL, 3, 3, 0);
                        tmp_ins = (char *) malloc((strlen((char *) token) + 1) * sizeof(char));
                        strcpy(tmp_ins, (char *) token);
                        instr_generator(IO_ASS, (void *) tmp_item->item_data.function->args[counter_params].param_name, (void *) tmp_ins, NULL, 3, 3, 0);
                      
                    }    
                    else {
                      //  fprintf (stderr,"Spatny typ parametru funkce\n");
                        return ERR_SEM_TYPE;
                    }   
                    counter_params++;
                    break;
                }
            case TOK_ID:
                if ((counter_params + 1) > (tmp_item->item_data.function->num_of_args))
                    return ERR_SEM_TYPE;
              //  fprintf(stderr,"ajd\n");
                if (coma_counter != counter_params) {
                   // fprintf (stderr,"Errs counters func 4\n");
                    return ERR_SYN;
                }
                if (tmp_item->item_data.function->num_of_args == counter_params) {
                  //  fprintf (stderr,"Spatny pocet parametru funkce\n");
                    return ERR_SEM_TYPE;
                }
                 tmp_func = STFind(&(tmp_item->item_data.function->local_table), tmp_item->item_data.function->args[counter_params].param_name);
                if (tmp_func == NULL) {
                   // fprintf(stderr,"Error with parameters\n");
                    return ERR_SEM;
                }
                tmp_func->item_data.variable.varFrame = 2;
                STitem *tmp_id;
                if (func_name != NULL) { 
                    tmp_id = STFind(&(func_name->item_data.function->local_table), token);
                     if (tmp_id == NULL) {
                        tmp_id = STFind(&GST, token);
                        if (tmp_id == NULL) {
                          //  fprintf(stderr,"undeclared variable for asigment\n");
                            return ERR_SEM;
                        } 
                     }
                }
                 else {
                    tmp_id = STFind(&GST, token);
                    if (tmp_id == NULL) {
                       // fprintf(stderr,"undeclared variable for asigment\n");
                        return ERR_SEM;
                    } 
                 } 
                if (tmp_id->item_type == function) {
                   // fprintf (stderr,"variable is function\n");
                    return ERR_SEM;
                }
                else if (tmp_id->item_data.variable.initialized == 0) {
                   // fprintf (stderr,"uninitialized variable in func param\n");
                    return ERR_SEM;
                }
                else {
                    if (tmp_id->item_data.variable.data_type == integer) {
                        if (tmp_item->item_data.function->args[counter_params].param_type == integer) {
                            instr_generator(IO_VAR_DEF, (void *) tmp_item->item_data.function->args[counter_params].param_name, NULL, NULL, 1, 3, 0);
                            tmp_ins = (char *) malloc((strlen((char *) token) + 1) * sizeof(char));
                            strcpy(tmp_ins, (char *) token);
                            instr_generator(IO_ASS, (void *) tmp_item->item_data.function->args[counter_params].param_name, (void *) tmp_ins, NULL, 4, 3, tmp_id->item_data.variable.varFrame);
                            
                        }           
                        else if (tmp_item->item_data.function->args[counter_params].param_type == d_double) {
                            instr_generator(IO_VAR_DEF, (void *) tmp_item->item_data.function->args[counter_params].param_name, NULL, NULL, 2, 3, 0);
                            tmp_ins = (char *) malloc((strlen((char *) token) + 1) * sizeof(char));
                            strcpy(tmp_ins, (char *) token);
                            instr_generator(IO_INT2FLOAT, (void *) tmp_item->item_data.function->args[counter_params].param_name, (void *) tmp_ins, NULL, 1, 3, tmp_id->item_data.variable.varFrame);
                        
                        } 
                        else {
                          //  fprintf (stderr,"Spatny typ parametru funkce\n");
                            return ERR_SEM_TYPE;
                        } 
                        counter_params++;
                        break;  

                    }
                    else if (tmp_id->item_data.variable.data_type == d_double) {
                        if (tmp_item->item_data.function->args[counter_params].param_type == integer) {
                            instr_generator(IO_VAR_DEF, (void *) tmp_item->item_data.function->args[counter_params].param_name, NULL, NULL, 1, 3, 0);
                            tmp_ins = (char *) malloc((strlen((char *) token) + 1) * sizeof(char));
                            strcpy(tmp_ins, (char *) token);
                            instr_generator(IO_FLOAT2R2EINT, (void *) tmp_item->item_data.function->args[counter_params].param_name, (void *) tmp_ins, NULL, 1, 3, tmp_id->item_data.variable.varFrame);
                            
                        }    
                        else if (tmp_item->item_data.function->args[counter_params].param_type == d_double) {
                            instr_generator(IO_VAR_DEF, (void *) tmp_item->item_data.function->args[counter_params].param_name, NULL, NULL, 2, 3, 0);
                            tmp_ins = (char *) malloc((strlen((char *) token) + 1) * sizeof(char));
                            strcpy(tmp_ins, (char *) token);
                            instr_generator(IO_ASS, (void *) tmp_item->item_data.function->args[counter_params].param_name, (void *) tmp_ins, NULL, 4, 3, tmp_id->item_data.variable.varFrame);
                            
                        } 
                        else {
                         //   fprintf (stderr,"Spatny typ parametru funkce\n");
                            return ERR_SEM_TYPE;
                        }   
                        counter_params++;
                        break;

                    }
                    else if (tmp_id->item_data.variable.data_type == string) {
                     //   fprintf(stderr,"niic\n");
                        if (tmp_item->item_data.function->args[counter_params].param_type == string) {
                            instr_generator(IO_VAR_DEF, (void *) tmp_item->item_data.function->args[counter_params].param_name, NULL, NULL, 3, 3, 0);
                          //  fprintf(stderr,"niic\n");
                            tmp_ins = (char *) malloc((strlen((char *) token) + 1) * sizeof(char));
                            strcpy(tmp_ins, (char *) token);
                            instr_generator(IO_ASS, (void *) tmp_item->item_data.function->args[counter_params].param_name, (void *) tmp_ins, NULL, 4, 3, tmp_id->item_data.variable.varFrame);
                         //   fprintf(stderr,"niic\n");
                
                        }    
                        else {
                          //  fprintf (stderr,"Spatny typ parametru funkce\n");
                            return ERR_SEM_TYPE;
                        }
                        counter_params++;
                        break;   
                    }
                    else {
                      //  fprintf (stderr,"niic\n");
                        return ERR_SYN;
                    }

                }

             case TOK_COMMA:
                    coma_counter++;
                    if (coma_counter != counter_params) {
                      //  fprintf (stderr,"syntax err\n");
                        return ERR_SYN;
                    }       


                     
                
        }
    }

    }
    return OK;
}