#include "shell.h"

#define DELIM_CHARS " \t\n"

int mashLoop(FILE * theFile){
    char *  line;
    char ** args;
    char    dir[100] = "/";
    char    stat = 'f';
    int     key = MASH_MEM_KEY;
    int     argc = 0;


    do{
        printf("MASH:%s$ ", dir);
        line = mashRead();
        args = mashSplit(line,DELIM_CHARS,&argc);
        stat = mashExecute(args,dir);
        free(line);
        free(args);
    }while(stat = 't');



    return stat;
}


char *mashRead(){
    char *line = NULL;
    ssize_t buflength = 0;
    getline(&line, &buflength, stdin);
    return line;
}


char **mashSplit(const char* str, const char* delim, int* numtokens){
    char *s = strdup(str);
    size_t tokens_alloc = 1;
    size_t tokens_used = 0;
    char **tokens = calloc(tokens_alloc, sizeof(char*));
    char *token, *rest = s;
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
    *numtokens = tokens_used;
    free(s);
    return tokens;
}


char mashExecute(char ** args, char * currentDir){
    int pid, wapid;
    char dir[5] = "bin/";
    int status;

    if(strcmp(args[0], MASH_PWD) == 0){
        printf("%s\n",currentDir);
        return 0;
    }

    if(strcmp(args[0],MASH_HELP) == 0){
        printf("Hep\n");
        return 0;
    }

    if(strcmp(args[0],MASH_EXIT) == 0){
        exit(EXIT_SUCCESS);
    }


    pid = fork();
    if(pid == 0){
        if(execv(strcat(dir,args[0]),args) == -1){
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

    return 't';
}
