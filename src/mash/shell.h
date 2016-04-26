
#ifndef MEMEDAGAINSHELL
#define MEMEDAGAINSHELL

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "../utils/fatSupport.h"
#include "../utils/utilities.h"

#define MASH_PWD     "pwd"
#define MASH_HELP    "help"
#define MASH_EXIT    "exit"
#define MASH_MOUNT   "mnt"

int     mashLoop(FILE * theFile, char * filename);
char *  mashRead();
char ** mashSplit(const char* str, const char* delim, int *argc);
int     mashExecute(char ** args, int argc, char * currentDir);



#endif
