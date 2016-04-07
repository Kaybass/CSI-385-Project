
#ifndef MEMEDAGAINSHELL
#define MEMEDAGAINSHELL

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int     mashLoop(FILE * theFile);
char *  mashRead();
char ** mashSplit(char * line);
int     mashExecute(char ** args, int pipefd);



#endif
