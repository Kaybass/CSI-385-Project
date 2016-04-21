#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "../utils/fatSupport.h"
#include "../utils/utilities.h"

#define MASH_MEM_KEY 6969696699

#define NUM_SECTORS 14
#define FAT_TOTAL_SIZE 512 * NUM_SECTORS

FILE* FILE_SYSTEM_ID;
int BYTES_PER_SECTOR;

extern unsigned int get_fat_entry(int fat_entry_number, char* fat);

int main(int argc, char *argv[])
{
    ubyte* image;

    if (argc == 1)
    {
        printf("baka\n");
        exit(1);
    }
    else if (argc == 2)
    {
        if(strcmp(argv[1],"-h") == 0)
        {
            printf("One argument, file\n");
            exit(1);
        }
        FILE_SYSTEM_ID = fopen(argv[1], "r+");
        if (FILE_SYSTEM_ID == NULL)
        {
            printf("Could not open the floppy drive or image.\n");
            exit(1);
        }
    }
    else
    {
        printf("baka\n");
        exit(1);
    }

    BYTES_PER_SECTOR = 512;
    ENTRY_SIZE       = 32;

    FileInfo files[16];

    image = (ubyte*)malloc(BYTES_PER_SECTOR * sizeof(ubyte));
    read_sector(19,image);

    for(int i = 0; i < BYTES_PER_SECTOR / ENTRY_SIZE; i++){
        for(int j = 0; j < 8; j++){
            files[i].Filename[j] = image[j + i * 32];
        }
        files[i].Filename[9] = '\0';

        for(int j = 9; j < 12; j++){
            files[i].Filename[j] = image[j + i * 32];
        }
        files[i].Filename[3] = '\0';

        files[i].Attributes = image[12 + i * 32];

        files[i].CreationTime[15]
    }

    free(image);
}
