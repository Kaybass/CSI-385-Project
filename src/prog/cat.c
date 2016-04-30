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
    int myFLC;
    int length;
    int* sectors;
    BYTES_PER_SECTOR = 512;

    if (argc == 2){
        exit(EXIT_SUCCESS);
    }
    else if (argc > 3){

        printf("cat takes one argument which is the file\n");
        exit(EXIT_SUCCESS);
    }
    else if (argc == 3 && strcmp(argv[1],"-h") == 0){

        printf("cat takes one argument which is the file\n");
        exit(EXIT_SUCCESS);
    }

    shmid = shmget(MASH_MEM_KEY, sizeof(SharedStuff),0666);

    if(shmid < 0){

        //We couldn't create the segment
        perror("Oh my god shared memory didn't work, god save us.");
        exit(EXIT_FAILURE);
    }

    stuff = (SharedStuff *) shmat(shmid,NULL,0);


    FILE_SYSTEM_ID = fopen(stuff->filename, "r+");
    if (FILE_SYSTEM_ID == NULL)
    {
        printf("Could not open the floppy drive or image.\n");
        exit(1);
    }

    myFLC = searchForFile(stuff->FLC, argv[1]);

    ubyte* fatTable = readFatTable(512 * 9, 9, 512);
    sectors = lookupSectors(myFLC, &length, fatTable);
    free(fatTable);

    for (int i = 0; i < length; i++)
    {
        byte* s = (byte*)malloc(BYTES_PER_SECTOR * sizeof(byte));
        read_sector(sectors[i], s);

        for (int j = 0; j < BYTES_PER_SECTOR; j++)
        {
            printf("%c", s[j]);
        }

        free(s);
    }

    fclose(FILE_SYSTEM_ID);

    return 0;
}
