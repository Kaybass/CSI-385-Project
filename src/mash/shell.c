#include "shell.h"

#define DELIM_CHARS " \t\n"

int mashLoop(FILE * theFile,char * filename){
    char *  line;
    char ** args;
    char    dir[100] = "/";
    int     stat = 0;
    key_t   key = MASH_MEM_KEY;
    //char    buf[256];
    int     shmid, buflen;
    char    *shm, *s;
    int     i, j, k;

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

        j      = strlen(filename);
        k      = strlen(dir);

        s = shm;

        for(i = 0; i < j; i++)
            *s++ = filename[i];
        for(i = 0; i < k; i++)
            *s++ = dir[i];
        *s = NULL;

        printf("MASH:%s$ ", dir);

        line = mashRead();
        args = mashSplit(line,DELIM_CHARS);
        stat = mashExecute(args,dir);

        free(line);
        free(args);

        i = 0;
        for (s = shm; *s != '*'; s++){
            dir[i] = *s;
            i++;
        }
        dir[i] = '\0';

    }while(stat == 0);

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
    char dir[30] = "bin/";
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
        printf("MASH: MEMED AGAIN SHELL\n"
                "Comannds:\n"
                "help:  Print help information\n"
                "pwd:   Print working directory\n"
                "exit:  Exit the program\n"
                "pbs:   Print boot sector\n"
                "pfe:   Print fat entries\n"
                "cd:    Change directory\n"
                "cat:   Print file contents\n"
                "ls:    Print directory contents\n"
                "rmdir: Remove directory\n"
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
            perror("Invalid command, type help for list of commands");
        }
        return 0;
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
