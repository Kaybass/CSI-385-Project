#include "shell.h"



int mashLoop(FILE * theFile){
  char *  line;
  char ** args;
  char    stat;
  int     pipefd;

  if(pipe(pipefd) == -1){
    perror("Oh god I couldn't make a pipe.");
    return -1;
  }

  /*
    Open root directory?
  */

  do{
    line = mashRead();
    args = mashSplit(line);

    stat = mashExecute(args,pipefd)
  }while(stat);

  return stat;
}


char *mashRead(){

}


char **mashSplit(char * line){

}


int mashExecute(char ** args, int pipefd){

}
