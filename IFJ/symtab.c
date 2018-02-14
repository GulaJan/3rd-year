#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "symtab.h"

int STHash(char* key){
    unsigned int hash = 0;
    int i;

    int keylen = strlen(key);
    for (i = 0; i < keylen; i++){
        hash = (hash << 5) + hash + key[i]; // alebo (hash << 6) + (hash << 16) - hash + hey[i]// hash * 65599
    }
    return (hash % 1000);
}

STitem* STFind(STable* st, char* key){ // returne ukazatel na hladany prvok ak sa najde, inak null
    STitem* HTsymbol; 
    int i = STHash(key);

    if (st == NULL){
        return NULL;
    }
    else{
        if(st->items[i] != NULL){
            HTsymbol = st->items[i];
            while (HTsymbol != NULL){
                if (!(strcmp(HTsymbol->key, key))) 
                    return HTsymbol;
                HTsymbol = HTsymbol->next;
            }
            return NULL; 
        }
        else
          return NULL;
    }
}

STitem* STAdd(STable* st, char* key, int malloc_type){
    STitem* temp;
    STitem* new ;
    STitem* ptr;
    int i;

    if (st != NULL){
        i = STHash(key);
        if (st->items[i] == NULL){ // taky symbol tam neni
            if ((new = malloc(sizeof(STitem))) == NULL){

                // vyvolat chybu?
                return NULL;
            }
          
            new->key = key;
            new->next = NULL;
            if (malloc_type == 1) {
                if ((new->item_data.function = malloc (sizeof(function_id)))==NULL)
                    return NULL;
            }    
        
            // vlozit na hlavicku listu
            st->items[i] = new;
            return new;
        }
        else{ // ak symbol moze byt v tom liste
            temp = st->items[i];
            while (temp != NULL){
                if (temp->key == key)
                    return NULL;
                else{
                    ptr = temp;
                    temp = temp->next;
                }
            }
            if (temp == NULL){ // ak taky symbol este neni v liste
                if ((new = malloc(sizeof(struct STitem))) == NULL){
                    // vyvolat chybu?
                    return NULL;
                }
                new->key = key;
                new->next = NULL;
                ptr->next = new; // insert last
                return new;
            }
            else
                return NULL;
        }
    }
    else
        return NULL;
}

void STinit (STable *table){
    if (table == NULL)
        return;

    int i;

    for (i = 0; i < 1000; i++)
        table->items[i] = NULL;
}

void STfree (STable *table){
     if (table == NULL)
        return;

    int i;

    for (i = 0; i < 1000; i++){
        STitem *pom = table->items[i];
        while (pom != NULL){
            STitem *pom2 = pom;
            pom = pom->next;
            free(pom2);
            pom2 = NULL;
        }
        table->items[i] = NULL;
	}
}

void Param_Add (STitem *item, char *param, int num_arg, data_type param_type){
    if (num_arg == 1){
        item->item_data.function->args =  malloc (sizeof(func_param));
    }
    else if (num_arg > 1){
        item->item_data.function->args = (func_param *) realloc (item->item_data.function->args, sizeof(func_param) * (num_arg));
    }
    else{
         return;
    }
    item->item_data.function->args[num_arg-1].param_name = param;
    item->item_data.function->args[num_arg-1].param_type = param_type;
}
void Param_Declaration_Add (STitem *item, char *param, int num_arg, data_type param_type){
    if (num_arg == 1){
        item->item_data.function->params =  malloc (sizeof(func_param_declaration));
    }
    else if (num_arg > 1){
        item->item_data.function->params = (func_param_declaration *) realloc (item->item_data.function->params, sizeof(func_param_declaration) * (num_arg));
    }
    else{
         return;
    }
    item->item_data.function->params[num_arg-1].param_name = param;
    item->item_data.function->params[num_arg-1].param_type = param_type;
}

char * f_Param (STitem *item, int num){
    return (item->item_data.function->args[num - 1].param_name);
}
char * f_Param_declaration (STitem *item, int num){
    return (item->item_data.function->params[num - 1].param_name);
}

/*
char *substr(char *vysledok, char *s, int i, int n, int types, int typei, int typen){
    char * tmp;
    char * tmp2;
    char *jedna = "1";
    tmp = (char *) malloc(3 * sizeof(char));
    sprintf(tmp, "%d", i);
    tmp2 = (char *) malloc(3 * sizeof(char));
    sprintf(tmp, "%d", n);

    // if ((s == NULL) || (i < 1))
    instr_generator(IO_PUSHS, s, NULL, NULL, types);
    instr_generator(IO_PUSHS, "", NULL, NULL, 3);
    instr_generator(IO_EQ, NULL, NULL, NULL, 0);
    instr_generator(IO_PUSHS, tmp, NULL, NULL, typei);
    instr_generator(IO_PUSHS, jedna, NULL, NULL, 1);
    instr_generator(IO_L, NULL, NULL, NULL, 0);
    instr_generator(IO_OR_S, NULL, NULL, NULL, 0);
    instr_generator(IO_PUSHS, NULL, NULL, NULL, 10);
    instr_generator(IO_JUMP_IF_NEQ_S, "\045substr_elseif", NULL, NULL, 0);
    // return NULL;
    instr_generator(IO_ASS, vysledok, "", NULL, 3);

    instr_generator(IO_LABEL, "\045substr_elseif", NULL, NULL, 0);
    // else if ((n < 0) || (n > (strlen(s) - i)))   
    instr_generator(IO_LEN, "\045print_temp_variable", s, NULL, types);
    instr_generator(IO_PUSHS,"\045print_temp_variable", NULL, NULL, 4);
    instr_generator(IO_PUSHS, tmp, NULL, NULL, 4);
    instr_generator(IO_MULS, NULL, NULL, NULL, 0);
    instr_generator(IO_POPS, "\045print_temp_variable", NULL, NULL, 0));
    instr_generator(IO_PUSHS, tmp2, NULL, NULL, typen);
    instr_generator(IO_PUSHS, "0", NULL, NULL, 1);
    instr_generator(IO_L, NULL, NULL, NULL, 0);
    instr_generator(IO_PUSHS, tmp2, NULL, NULL, typen);
    instr_generator(IO_PUSHS,"\045print_temp_variable", NULL, NULL, 4);
    instr_generator(IO_G, NULL, NULL, NULL, 0);
    instr_generator(IO_OR_S, NULL, NULL, NULL, 0);
    instr_generator(IO_PUSHS, NULL, NULL, NULL, 10);
    instr_generator(IO_JUMP_IF_NEQ_S, "\045substr_else", NULL, NULL, 0);
        for (j = (i - 1); j < strlen(s); j++){  // pocitanie index od 1 preto i - 1
            sub[k] = s[j];
            k++;
        }
    

    instr_generator(IO_LABEL, "\045substr_else", NULL, NULL, 0);
    else{
        for (j = (i - 1); j < (i + n - 1); j++){ // pocitanie index od 1 preto i - 1
            sub[k] = s[j];
            k++;
        }
    }
    return sub;
}
*/
/*int asc(char *vysledok, char *s, int i, int types, int typei){     // ak i je hodnota tak typei = 1, ak id = 4
    char * tmp;
    char *jedna = "1";
    tmp = (char *) malloc(3 * sizeof(char));
    sprintf(tmp, "%d", i);
    // if ((i < 1) && (i > strlen(s)))
    instr_generator(IO_LEN, "\045print_temp_variable", s, NULL, types);
    instr_generator(IO_PUSHS, tmp, NULL, NULL, typei);
    instr_generator(IO_PUSHS, jedna, NULL, NULL, 1);
    instr_generator(IO_L, NULL, NULL, NULL, 0);
    instr_generator(IO_PUSHS, tmp, NULL, NULL, typei);
    instr_generator(IO_PUSHS, "\045print_temp_variable", NULL, NULL, 4);
    instr_generator(IO_G, NULL, NULL, NULL, 0);
    instr_generator(IO_OR_S, NULL, NULL, NULL, 0);
    instr_generator(IO_PUSHS, jedna, NULL, NULL, 1);
    instr_generator(IO_JUMP_IF_NEQ_S, "\045asclabel", NULL, NULL, 0);
     // return 0
    instr_generator(IO_ASS, vysledok, "0", NULL, 1);   

    instr_generator(IO_LABEL, "\045asclabel", NULL, NULL, 0);
    instr_generator(IO_MUL, "tmp" , tmp , jedna, typei);       // -1 lebo tato instrukcia indexuje od 0 a normalne indexuju od 1
    if ((types == 3) && (typei == 1)) {  // obidve su hodnoty
        instr_generator(IO_STRI2INT, vysledok, s, tmp, 1);       
    }
    else if ((types == 3) && (typei == 4)){
        instr_generator(IO_STRI2INT, vysledok, s, tmp, 2);
    }
    else if ((types == 4) && (typei == 3)){
        instr_generator(IO_STRI2INT, vysledok, s, tmp, 3);
    }
    else { //obide su id
        instr_generator(IO_STRI2INT, vysledok, s, tmp, 4);
    }
    free(tmp);
    return 0;
}

int chr(char *vysledok, int i, int type){
    char * tmp;
    tmp = (char *) malloc(3 * sizeof(char));
    sprintf(tmp, "%d", i);
    instr_generator(IO_INT2CHAR, vysledok, tmp, NULL, type);
    return 0;
}
*/
// osetrit label pri jumpifneqs