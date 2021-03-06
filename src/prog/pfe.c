#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h> // We are using std99 so this should work
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../utils/fatSupport.h"
#include "../utils/utilities.h"

#define NUM_FAT_SECTORS 9
#define FAT_TABLE_SIZE 512 * NUM_FAT_SECTORS

FILE* FILE_SYSTEM_ID;
int BYTES_PER_SECTOR;

extern int read_sector(int sector_number, char* buffer);
extern unsigned int get_fat_entry(int fat_entry_number, char* fat);

bool checkRange(int x, int y);

void print(ubyte* fat, int x, int y);
void useage();

int main(int argc, char** argv)
{
    int x;
    int y;
    int shmid;
    SharedStuff* stuff;
    BYTES_PER_SECTOR = 512;

    if (argc == 1 || argc > 4)
    {
        useage();
    }

    x = atoi(argv[1]);
    y = atoi(argv[2]);

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

    bool valid = checkRange(x, y);
    if (!valid)
    {
        printf("Values %d and %d are not valid. Exiting.\n", x, y);
        exit(1);
    }

    // At this point x and y should be valid
    ubyte* fat_table = readFatTable(FAT_TABLE_SIZE,NUM_FAT_SECTORS,BYTES_PER_SECTOR);
    print(fat_table, x, y);

    free(fat_table);
    fclose(FILE_SYSTEM_ID);

    return 0;
}

bool checkRange(int x, int y)
{
    // Validate x and y
    if (x > y)
        return false;
    if (x < 2)
        return false;

    return true;
}

void print(ubyte* fat, int x, int y)
{
   for (int i = x; i <= y; i++)
   {
       printf("Entry %d: %x\n", i, get_fat_entry(i, fat));
   }
}

void useage()
{
    printf("Useage: pfe [x] [y]\n");
    printf("Where x and y bounds for a FAT entry\n");
    exit(1);
}
