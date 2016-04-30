#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "../utils/fatSupport.h"
#include "../utils/utilities.h"

FILE* FILE_SYSTEM_ID;
int BYTES_PER_SECTOR;

int main(int argc, char *argv[]){

    int shmid;
    SharedStuff * stuff;
    BYTES_PER_SECTOR = 512;
    short newFLC;

    if (argc < 3){

        printf("No arguments given\n");
        exit(EXIT_SUCCESS);
    }
    else if (argc > 3){

        printf("cd takes one argument which is the folder\n");
        exit(EXIT_SUCCESS);
    }
    else if (argc == 3 && strcmp(argv[1],"-h") == 0){

        printf("cd takes one argument which is the folder\n");
        exit(EXIT_SUCCESS);
    }

    shmid = shmget(MASH_MEM_KEY, sizeof(SharedStuff),0666);

    if(shmid < 0){

        //We couldn't create the segment
        perror("Oh my god shared memory didn't work");
        exit(EXIT_FAILURE);
    }

    stuff = (SharedStuff *) shmat(shmid,NULL,0);

    FILE_SYSTEM_ID = fopen(stuff->filename,"r+");

    if(FILE_SYSTEM_ID == NULL){
        perror("Something has gone horribly wrong!");
        exit(EXIT_FAILURE);
    }

    newFLC = searchForFolder(stuff->FLC,argv[1]);

    if(newFLC != -1){
        
        stuff->FLC = newFLC;
        strcpy(stuff->dir,argv[1]);
    }
    else{
        perror("Directory doesn't exist");
    }



    fclose(FILE_SYSTEM_ID);
}
