#include "shell.h"

#define DELIM_CHARS " \t\n"

int mashLoop(char * filename){
    char *  line;
    char ** args;
    char *  dir = "/";
    int     stat = 0;
    int     shmid;
    int     argc = 0;

    SharedStuff * stuff;

    shmid = shmget(MASH_MEM_KEY, sizeof(SharedStuff), IPC_CREAT | 0666);

    if(shmid < 0){

        //We couldn't create the segment
        perror("Oh my god shared memory didn't work, god save us.");
        exit(EXIT_FAILURE);
    }

    stuff = (SharedStuff *) shmat(shmid,NULL,0);

    stuff->FLC  = 0;
    strcpy(stuff->dir, dir);
    strcpy(stuff->filename,filename);

    /*
     * Main loop
    */
    do{
        printf("MASH:%s$ ", stuff->dir);

        line = mashRead();
        if(strcmp(line,"\n") != 0){
            args = mashSplit(line,DELIM_CHARS,&argc);
            stat = mashExecute(args,argc,stuff);
            free(args);
        }

        free(line);


    }while(stat == 0);

    shmctl(shmid, IPC_RMID, NULL);
    return stat;
}


char *mashRead(){
    char *line = NULL;
    int buflength = 0;
    getline(&line, &buflength, stdin);
    return line;
}


char **mashSplit(const char* args, const char* delim, int *argc){

    char *myCopy = strdup(args);
    int tokensAllocated = 1;
    int tokensUsed = 0;

    char **tokens = calloc(tokensAllocated, sizeof(char*));
    char *token, *rest = myCopy;

    while((token = strsep(&rest, delim)) != NULL){

        if (tokensUsed == tokensAllocated){

            tokensAllocated *= 2;
            tokens = realloc(tokens, tokensAllocated * sizeof(char*));
        }

        tokens[tokensUsed++] = strdup(token);
    }

    if(tokensUsed == 0){
        free(tokens);
        tokens = NULL;
    }
    else{
        tokens = realloc(tokens, tokensUsed * sizeof(char*));
    }

    *argc = tokensUsed - 1;
    free(myCopy);

    return tokens;
}

int mashExecute(char ** args, int argc, SharedStuff * stuff){
    int pid, wapid;
    char dir[30] = "bin/";
    int status;

    /*
     * Built in functions
    */

    // Print working directory
    if(strcmp(args[0], MASH_PWD) == 0){
        printf("%s\n",stuff->dir);
        return 0;
    }
    // Help
    else if(strcmp(args[0],MASH_HELP) == 0){
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
                "mnt:   Mount new image\n");
        return 0;
    }
    // Exit
    else if(strcmp(args[0],MASH_EXIT) == 0){
        return 1;
    }

    // Exit with shared memory intact (for debugging)
    else if(strcmp(args[0],MASH_EXITSHM) == 0){
        exit(EXIT_SUCCESS);
    }

    // Print shared memory
    else if(strcmp(args[0],MASH_PSHM) == 0){
        printf("%s, %d, %s\n", stuff->dir, stuff->FLC, stuff->filename);
        return 0;
    }

    // Mount
    else if(strcmp(args[0],MASH_MOUNT) == 0){

        if (argc == 1){

            printf("No arguments given\n");
            return 0;
        }
        else if (argc > 2){

            printf("mnt takes one argument which is the image\n");
            return 0;
        }
        else if (argc == 2 && strcmp(args[1],"-h") == 0){

            printf("mnt takes one argument which is the image\n");
            return 0;
        }

        FILE * theFile = fopen(args[1], "r+");

        if(theFile == NULL){

            perror("Image doesn't exist");
        }
        else{

            fclose(theFile);
            stuff->FLC = 0;
            strcpy(stuff->dir, "/");
            strcpy(stuff->filename, args[1]);
        }
        return 0;
    }

    /*
     * Execution of non built in program
    */
    else{
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
    }

    return 0;
}
