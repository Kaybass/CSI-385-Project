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
int ENTRY_SIZE;

extern unsigned int get_fat_entry(int fat_entry_number, char* fat);

int main(int argc, char *argv[])
{
    byte* image;

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

    image = (byte*)malloc(BYTES_PER_SECTOR * sizeof(ubyte));
    read_sector(19, image);

    for(int i = 0; i < BYTES_PER_SECTOR / ENTRY_SIZE; i++){
        for(int j = 0; j < 8; j++){
            files[i].Filename[j] = image[j + i * 32];
        }
        files[i].Filename[8] = '\0';

        for(int j = 9; j < 12; j++){
            files[i].Type[j] = image[j + i * 32];
        }
        files[i].Type[3] = '\0';

        files[i].Attributes = image[12 + i * 32];
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

        // Bit shifting yay :D
        int one, two, three, four;
        int mostSignificantBits, leastSignificantBits;
        
        mostSignificantBits  = ( ( (int) image[27 + i * 32] ) << 8 ) & 0x0000ff00;     
        leastSignificantBits = ( ( (int) image[26 + i * 32] )      ) & 0x000000ff;
        files[i].FirstLogicalCluster = mostSignificantBits | leastSignificantBits;

        one =   ( ( (int) image[31 + i * 32] ) << 24 ) & 0xff000000;
        two =   ( ( (int) image[30 + i * 32] ) << 16 ) & 0x00ff0000;
        three = ( ( (int) image[29 + i * 32] ) << 8  ) & 0x0000ff00;
        four =  (   (int) image[28 + i * 32] )         & 0x000000ff;
        files[i].FileSize = one | two | three | four;
    }

    free(image);
}
