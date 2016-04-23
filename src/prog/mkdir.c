#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "../utils/fatSupport.h"
#include "../utils/utilities.h"


int main(int argc, char *argv[]){

    if (argc == 1){

        printf("No arguments given\n");
        exit(1);
    }
    else if (argc > 2){

        printf("mkdir takes one argument which is the folder name\n");
    }

    shmid = shmget(MASH_MEM_KEY, sizeof(SharedStuff),0666);

    if(shmid < 0){

        //We couldn't create the segment
        perror("Oh my god shared memory didn't work, god save us.");
        exit(EXIT_FAILURE);
    }

    stuff = (SharedStuff *) shmat(shmid,NULL,0);

    //do mkdir stuff here
}
