#include "shell.h"



int mashLoop(FILE * theFile){
    char *  line;
    char ** args;
    char    dir[100] = "/";
    char    stat;
    char    bufferino[150], buf;
    int     i;
    int     key = MASH_MEM_KEY;


    do{
        printf("MASH:%s$", dir);
        line = mashRead();
        args = mashSplit(line);

        stat = mashExecute(args)
        free(line);
        free(args);
    }while(stat = 0);



    return stat;
}


char *mashRead(){
    char *line = NULL;
    ssize_t buflength = 0;
    getline(&line, &buflength, stdin);
    return line;
}


char **mashSplit(char * line){

    char *s = strdup(str);
    size_t tokens_alloc = 1;
    size_t tokens_used = 0;
    char **tokens = calloc(tokens_alloc, sizeof(char*));
    char *token, *rest = s;
    char delim = ' '

    while ((token = strsep(&rest, delim)) != NULL) {
        if (tokens_used == tokens_alloc) {
            tokens_alloc *= 2;
            tokens = realloc(tokens, tokens_alloc * sizeof(char*));
        }
        tokens[tokens_used++] = strdup(token);
    }
    if (tokens_used == 0) {
        free(tokens);
        tokens = NULL;
    } else {
        tokens = realloc(tokens, tokens_used * sizeof(char*));
    }
    free(s);
    return tokens;
}


char mashExecute(char ** args){
    pit_t pid, wapid;
    char dir[30] = "prog/"
    int status;

    if(strcmp(arg[0], MASH_PWD) == 0){
      printf("The current directory\n");
      return 0;
    }

    if(strcmp(arg[0],MASH_HELP == 0)){
      printf("Hep\n");
      return 0;
    }


    pid = fork();
    if(pid == 0){
        if(execv(strcat(dir,arg[0]),args) == -1){
            perror("Invalid command, type help for list of commands :)");
        }
        exit(EXIT_FAILURE);
    }
    else if(pid < 0){
        perror("Computer did us wrong");
    }
    else{
        do {
          wapid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}
