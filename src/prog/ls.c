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
    int * thing;
    short FLC;
    int length = 1;
    char tmp[40];

    if(argc > 3 || (argc == 3 && strcmp(argv[1],"-h") == 0) ){

        printf("ls either takes one or zero arguments\n");
    }

    shmid = shmget(MASH_MEM_KEY, sizeof(SharedStuff), 0666);

    if(shmid < 0){

        //We couldn't create the segment
        perror("Oh my god shared memory didn't work.");
        exit(EXIT_FAILURE);
    }

    if((stuff = (SharedStuff *) shmat(shmid,NULL,0)) == (SharedStuff *) -1){
        perror("Oh my god shared memory didn't work.");
        exit(EXIT_FAILURE);
    }

    FILE_SYSTEM_ID = fopen(stuff->filename, "r+");
    if (FILE_SYSTEM_ID == NULL)
    {
        printf("Could not open the floppy drive or image.\n");
        exit(EXIT_FAILURE);
    }

    BYTES_PER_SECTOR = 512;
    if(argc == 2){
        FLC = stuff->FLC;
    }
    else{
        FLC = searchForFolder(stuff->FLC, argv[1]);
        if(FLC == -1){
            printf("No Such Directory\n");
            exit(EXIT_FAILURE);
        }
    }

    if(FLC != 0){
        ubyte * fatTable = readFatTable(512 * 9, 9, 512);
        thing = lookupSectors(FLC,&length,fatTable);
        free(fatTable);
    }

    FileInfo* files = (FileInfo*)malloc(length * 16 * sizeof(FileInfo));

    int h,l,j,k;

    if(FLC != 0){
        for (int z = 0; z < length; z++) {
            image = (byte*)malloc(BYTES_PER_SECTOR * sizeof(ubyte));
            read_sector(thing[z],image);

            for(int i = 0 + (z * 16); i < 16 + (z * 16); i++){
                for(int j = 0; j < 8; j++){
                    files[i].Filename[j] = image[j + (i - z * 16) * 32];
                }
                files[i].Filename[8] = '\0';

                for(int j = 0; j < 3; j++){
                    files[i].Type[j] = image[j + 8 + (i - z * 16) * 32];
                }
                files[i].Type[3] = '\0';

                files[i].Attributes = image[11 + (i - z * 16) * 32];

                files[i].CreationTime[0] = image[14 + (i - z * 16) * 32];
                files[i].CreationTime[1] = image[15 + (i - z * 16) * 32];

                files[i].CreationDate[0] = image[16 + (i - z * 16) * 32];
                files[i].CreationDate[1] = image[17 + (i - z * 16) * 32];

                files[i].LastAccessDate[0] = image[18 + (i - z * 16) * 32];
                files[i].LastAccessDate[1] = image[19 + (i - z * 16) * 32];

                files[i].LastWriteTime[0] = image[22 + (i - z * 16) * 32];
                files[i].LastWriteTime[1] = image[23 + (i - z * 16) * 32];

                files[i].LastWriteDate[0] = image[24 + (i - z * 16) * 32];
                files[i].LastWriteDate[1] = image[25 + (i - z * 16) * 32];

                h = ( ( (int) image[27 + (i - z * 16) * 32] ) << 8 ) & 0x0000ff00;
                l =   ( (int) image[26 + (i - z * 16) * 32] )        & 0x000000ff;
                files[i].FirstLogicalCluster = h | l;

                h =   ( ( (int) image[31 + (i - z * 16) * 32] ) << 24 ) & 0xff000000;
                l =   ( ( (int) image[30 + (i - z * 16) * 32] ) << 16 ) & 0x00ff0000;
                j =   ( ( (int) image[29 + (i - z * 16) * 32] ) << 8  ) & 0x0000ff00;
                k =   (   (int) image[28 + (i - z * 16) * 32] )         & 0x000000ff;
                files[i].FileSize = h | l | j | k;
            }
            free(image);
        }
    }
    else{
        // Read in sectors used in Root
        int secRead = getRootSize();

        for (int z = 0; z < secRead; z++) {
            image = (byte*)malloc(BYTES_PER_SECTOR * sizeof(ubyte));
            read_sector(z + 19, image);

            for(int i = 0 + (z * 16); i < 16 + (z * 16); i++){
                for(int j = 0; j < 8; j++){
                    files[i].Filename[j] = image[j + (i - z * 16) * 32];
                }
                files[i].Filename[8] = '\0';

                for(int j = 0; j < 3; j++){
                    files[i].Type[j] = image[j + 8 + (i - z * 16) * 32];
                }
                files[i].Type[3] = '\0';

                files[i].Attributes = image[11 + (i - z * 16) * 32];

                files[i].CreationTime[0] = image[14 + (i - z * 16) * 32];
                files[i].CreationTime[1] = image[15 + (i - z * 16) * 32];

                files[i].CreationDate[0] = image[16 + (i - z * 16) * 32];
                files[i].CreationDate[1] = image[17 + (i - z * 16) * 32];

                files[i].LastAccessDate[0] = image[18 + (i - z * 16) * 32];
                files[i].LastAccessDate[1] = image[19 + (i - z * 16) * 32];

                files[i].LastWriteTime[0] = image[22 + (i - z * 16) * 32];
                files[i].LastWriteTime[1] = image[23 + (i - z * 16) * 32];

                files[i].LastWriteDate[0] = image[24 + (i - z * 16) * 32];
                files[i].LastWriteDate[1] = image[25 + (i - z * 16) * 32];

                h = ( ( (int) image[27 + (i - z * 16) * 32] ) << 8 ) & 0x0000ff00;
                l =   ( (int) image[26 + (i - z * 16) * 32] )        & 0x000000ff;
                files[i].FirstLogicalCluster = h | l;

                h =   ( ( (int) image[31 + (i - z * 16) * 32] ) << 24 ) & 0xff000000;
                l =   ( ( (int) image[30 + (i - z * 16) * 32] ) << 16 ) & 0x00ff0000;
                j =   ( ( (int) image[29 + (i - z * 16) * 32] ) << 8  ) & 0x0000ff00;
                k =   (   (int) image[28 + (i - z * 16) * 32] )         & 0x000000ff;
                files[i].FileSize = h | l | j | k;
            }

            free(image);
        }
    }

    printf("  NAME        TYPE     SIZE  FLC\n");

    for(int i = 0; i < length * 16; i++){
        if(files[i].Filename[0] != '\xe5' &&
            files[i].Filename[0] != '\x0f' &&
            files[i].Filename[0] != -27 &&
            files[i].Filename[0] != 0){
            if((files[i].Attributes & FAT_HIDDEN)  == 0 &&
                (files[i].Attributes & FAT_SYSTEM)  == 0){

                char* type;
                if(strcmp(files[i].Filename,"LARGEDIR") != 0){
                    if((files[i].Attributes & FAT_SUBDIRECTORY)  != 0){
                        printf("%12s %4s   %5d   %3d\n", files[i].Filename,
                                                         "DIR",
                                                         files[i].FileSize,
                                                         files[i].FirstLogicalCluster);
                    }
                    else{
                        strcpy(tmp,"");

                        for(int j = 0; j < 8; j++){
                            if(files[i].Filename[j] == ' '){
                                files[i].Filename[j] = '\0';
                            }
                        }
                        for(int j = 0; j < 3; j++){
                            if(files[i].Type[j] == ' '){
                                files[i].Type[j] = '\0';
                            }
                        }

                        strcat(tmp,files[i].Filename);
                        strcat(tmp,".");
                        strcat(tmp,files[i].Type);
                        printf("%12s  %4s   %5d   %3d\n", tmp,
                                                         "FILE",
                                                         files[i].FileSize,
                                                         files[i].FirstLogicalCluster);
                    }
                }
            }
        }
    }

    free(files);

    fclose(FILE_SYSTEM_ID);

    return 0;
}
