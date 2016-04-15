
#ifndef MEMEDAGAINSHELL
#define MEMEDAGAINSHELL

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "../utils/fatSupport.h"
#include "../utils/utilities.h"

#define MASH_PWD     "pwd"
#define MASH_HELP    "help"
#define MASH_MEM_KEY 6969696699

int     mashLoop(FILE * theFile);
char *  mashRead();
char ** mashSplit(char * line);
char    mashExecute(char ** args);



#endif
