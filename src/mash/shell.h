
#ifndef MEMEDAGAINSHELL
#define MEMEDAGAINSHELL

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int     mashLoop();
char *  mashRead();
char ** mashSplit(char * line);
int     mashExecute(char ** args);
int     mashExit();



#endif
