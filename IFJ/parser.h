#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
int commandsCycle(int whileFlag, int ifFlag, int elseFlag, int functionFlag, FILE *f, char *frameName, int while_counter);
int ListOfParams (FILE *f,char *funcName, int declaration);
int topLevelTrigger (FILE *f);
int varDefinition(FILE *f, int funcFlag, char *funcName);
int printAnalysis(FILE *f);
int whileAnalysis(FILE *f, char *frameNameLoc, int while_counter);
int semanticAnalysis(FILE *f);
#endif
