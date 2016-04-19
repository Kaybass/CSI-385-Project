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

    image = (ubyte*)malloc(BYTES_PER_SECTOR * sizeof(ubyte));
    image = readFatTable(FAT_TOTAL_SIZE, NUM_SECTORS, BYTES_PER_SECTOR); 

    // what do halp

    free(image);
}
