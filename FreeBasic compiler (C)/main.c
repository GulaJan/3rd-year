#include "scanner.h"
#include "parser.h"
#include "instruction.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main() {
	//int bul = (strcasecmp("a","A") == 0);
	char *lstring;
	if ((lstring = (char *) malloc((sizeof (char)))) == NULL) {
		fprintf(stderr, "Insufficient memory space!\n");
		return 99;
	}
	
	//char *index = "001abc";
	//char *rest;
	//int ts;
	//int i = argc;
	//FILE *f;
	//f = fopen(argv[0],"r");
	int res = 0;
	res = topLevelTrigger(stdin);
	if (res == 62)
		return 1;
	else if (res == 63)
		return 2;
	else if (res == 64)
		return 3;
	else if (res == 65)
		return 4;
	else if (res == 66)
		return 6;

	//fclose(f);
	generator(&list);
//	while((tok = Token(&lstring, f)) != TOK_EOF) {
//		printf("\n %s \n", lstring);
//		printf ("Token is %d\n",tok);
//	}
	
	free(lstring);
	
	lstring = NULL;
	
	if (res == 61)
		return 0;
}




