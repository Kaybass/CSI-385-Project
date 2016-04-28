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

    SharedStuff* stuff;
    int shmid;
    int hardCodeFLC = 2;
    int length;
    int* sectors;
    ubyte* image;

    if (argc == 1){

        const char *args[1];
        args[0] = "ls";

        //magic string is bad boy points, change later
        if(execv("/bin/ls",args) == -1){ //execute

            //If it gets to this point there was an error
            perror("MASH is probably being run from the wrong directory");
        }
        return 0;
    }
    else if (argc > 2){

        printf("cat takes one argument which is the file\n");
    }
    else if (argc == 2 && strcmp(argv[1],"-h") == 0){

        printf("cat takes one argument which is the file\n");
    }

    shmid = shmget(MASH_MEM_KEY, sizeof(SharedStuff),0666);

    if(shmid < 0){

        //We couldn't create the segment
        perror("Oh my god shared memory didn't work, god save us.");
        exit(EXIT_FAILURE);
    }

    stuff = (SharedStuff *) shmat(shmid,NULL,0);

    //do cat stuff here
    image = readFatTable(512 * 9, 9, 512); 
    sectors = lookupSectors(hardCodeFLC, &length, image); 
    
    for (int i = 0; i < length; i++)
    {
        byte* s = (byte*)malloc(BYTES_PER_SECTOR * sizeof(byte)); 
        read_sector(sectors[i], s);

        for (int j = 0; j < BYTES_PER_SECTOR; j++)
        {
            //printf("%c", s[j]);
        }

        free(s);
    }

    free(image); 
}
