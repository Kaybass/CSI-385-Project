#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "../utils/fatSupport.h"
#include "../utils/utilities.h"

#define NUM_SECTORS 14
#define FAT_TOTAL_SIZE 512 * NUM_SECTORS

FILE* FILE_SYSTEM_ID;
int BYTES_PER_SECTOR;

#define ENTRY_SIZE 32


int main(int argc, char **argv)
{
    byte* image;
    int shmid;
    SharedStuff * stuff;

    if (argc == 2)
    {
        if(strcmp(argv[1],"-h") == 0)
        {
            printf("One argument, file\n");
            exit(1);
        }
    }

    shmid = shmget(MASH_MEM_KEY, sizeof(SharedStuff), 0666);

    if(shmid < 0){

        //We couldn't create the segment
        perror("Oh my god shared memory didn't work.");
        exit(EXIT_FAILURE);
    }

    if((stuff = (SharedStuff *) shmat(shmid,NULL,0)) == (SharedStuff *) -1){
        perror("Oh my god shared memory didn't work.");
        exit(1);
    }

    FILE_SYSTEM_ID = fopen(stuff->filename, "r+");
    if (FILE_SYSTEM_ID == NULL)
    {
        printf("Could not open the floppy drive or image.\n");
        exit(1);
    }

    BYTES_PER_SECTOR = 512;

    short FLC = stuff->FLC;

    ubyte * fatTable = readFatTable(512 * 9, 9, 512);

    int length = 16;



    //printf("%d\n", searchForFolder(0,"/COURSE/ACC"));

    int * thing = lookupSectors(36,&length,fatTable);

    free(fatTable);

    FileInfo* files = (FileInfo*)malloc(length * 16 * sizeof(FileInfo));

    int h,l,j,k;

    for (int z = 0; z < length; z++) {
        image = (byte*)malloc(BYTES_PER_SECTOR * sizeof(ubyte));
        read_sector(thing[z],image);
        for(int i = 0 + (z * 16); i < 16 + (z * 16); i++){
            for(int j = 0; j < 8; j++){
                files[i].Filename[j] = image[j + i * 32];
            }
            files[i].Filename[8] = '\0';

            printf("%s%d\n",files[i].Filename, z);

            for(int j = 0; j < 3; j++){
                files[i].Type[j] = image[j + 8 + i * 32];
            }
            files[i].Type[3] = '\0';

            files[i].Attributes = image[11 + i * 32];

            files[i].CreationTime[0] = image[14 + i * 32];
            files[i].CreationTime[1] = image[15 + i * 32];

            files[i].CreationDate[0] = image[16 + i * 32];
            files[i].CreationDate[1] = image[17 + i * 32];

            files[i].LastAccessDate[0] = image[18 + i * 32];
            files[i].LastAccessDate[1] = image[19 + i * 32];

            files[i].LastWriteTime[0] = image[22 + i * 32];
            files[i].LastWriteTime[1] = image[23 + i * 32];

            files[i].LastWriteDate[0] = image[24 + i * 32];
            files[i].LastWriteDate[1] = image[25 + i * 32];

            h = ( ( (int) image[27 + i * 32] ) << 8 ) & 0x0000ff00;
            l =   ( (int) image[26 + i * 32] )        & 0x000000ff;
            files[i].FirstLogicalCluster = h | l;

            h =   ( ( (int) image[31 + i * 32] ) << 24 ) & 0xff000000;
            l =   ( ( (int) image[30 + i * 32] ) << 16 ) & 0x00ff0000;
            j =   ( ( (int) image[29 + i * 32] ) << 8  ) & 0x0000ff00;
            k =   (   (int) image[28 + i * 32] )         & 0x000000ff;
            files[i].FileSize = h | l | j | k;
        }
        free(image);
    }

    for (int i = 0; i < 48; i++) {
        printf("%s\n", files[i].Filename);
    }

    printf("NAME     TYPE   SIZE   FLC\n");

    for(int i = 0; i < length * 16; i++){
        if(files[i].Filename[0] != '\xe5' &&
            files[i].Filename[0] != '\x0f' &&
            files[i].Filename[0] != -27 &&
            files[i].Filename[0] != 0){
            if((files[i].Attributes & FAT_HIDDEN)  == 0 &&
                (files[i].Attributes & FAT_SYSTEM)  == 0){

                char* type;

                printf("%8s %3s   %5d   %3d\n", files[i].Filename,
                                                 files[i].Type,
                                                 files[i].FileSize,
                                                 files[i].FirstLogicalCluster);

            }
        }
    }

    free(files);

    fclose(FILE_SYSTEM_ID);

    return 0;
}
