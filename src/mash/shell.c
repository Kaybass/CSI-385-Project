#include "shell.h"

#define DELIM_CHARS " "

int mashLoop(FILE * theFile){
    char *  line;
    char ** args;
    char    dir[100] = "/";
    int     stat = 0;
    key_t   key = MASH_MEM_KEY;
    char    buf[256];
    int     shmid, buflen;
    char    *shm, *s;

    shmid = shmget(key, 27, IPC_CREAT | 0666);

    if(shm < 0){

        //We couldn't create the segment
        perror("Oh my god shared memory didn't work, god save us.");
        exit(EXIT_FAILURE);
    }

    shm = shmat(shmid,NULL,0);

    if(shm == (char*) - 1){

        //we couldn't put the memory on the stack
        perror("Stuff didn't work");
        exit(EXIT_FAILURE);
    }

    /*
     * Main loop
    */
    do{

        /* Do shared memory here
        */

        printf("MASH:%s$ ", dir);

        line = mashRead();
        args = mashSplit(line,DELIM_CHARS);
        stat = mashExecute(args,dir);

        free(line);
        free(args);

        /* Do shared memory here
        */

    }while(stat = 0);

    return stat;
}


char *mashRead(){
    char *line = NULL;
    ssize_t buflength = 0;
    getline(&line, &buflength, stdin);
    return line;
}


char **mashSplit(const char* str, const char* delim){

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
    }
    else {
        tokens = realloc(tokens, tokens_used * sizeof(char*));
    }
    free(s);

    return tokens;
}


int mashExecute(char ** args, char * currentDir){
    int pid, wapid;
    char dir[5] = "bin/";
    int status;

    /*
     * Built in functions
    */

    // Print working directory
    if(strcmp(args[0], MASH_PWD) == 0){
        printf("%s\n",currentDir);
        return 0;
    }
    // Help
    if(strcmp(args[0],MASH_HELP) == 0){
        printf("MASH: MEMED AGAIN SHELL"
                "Comannds:"
                "help:  Print help information"
                "pwd:   Print working directory"
                "exit:  Exit the program"
                "pbs:   Print boot sector"
                "pfe:   Print fat entries"
                "cd:    Change directory"
                "cat:   Print file contents"
                "ls:    Print directory contents"
                "rmdir: Remove directory"
                "mkdir: Create directory\n");
        return 0;
    }
    // Exit
    if(strcmp(args[0],MASH_EXIT) == 0){
        exit(EXIT_SUCCESS);
    }


    /*
     * Execution of non built in program
    */
    pid = fork();
    if(pid == 0){
        if(execv(strcat(dir,args[0]),args) == -1){ //execute

            //If it gets to this point there was an error
            perror("Invalid command, type help for list of commands :)");
        }
        exit(EXIT_FAILURE);
    }
    else if(pid < 0){

        //forking failed
        perror("Computer did us wrong");
    }
    else{

        //Wait for the program to finish
        do {
            wapid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 0;
}
