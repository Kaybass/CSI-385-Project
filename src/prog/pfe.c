#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h> // We are using std99 so this should work

#include "../utils/fatSupport.h"

#define BYTES_PER_SECTOR 512
#define NUM_FAT_SECTORS 9
#define FAT_TABLE_SIZE BYTES_PER_SECTOR * NUM_FAT_SECTORS  

typedef unsigned char ubyte;
typedef char byte;

FILE* FILE_SYSTEM_ID;
int BYTES_PER_SECTOR;

bool checkRange(int x, int y);
ubyte* checkFatTable();

void useage();

int main(int argc, char** argv)
{
    int x;
    int y;

    // Handle commandline args
    if (argc == 1 || argc > 2)
    {
        useage();
    }

    x = atoi(argv[1]);
    y = atoi(argv[2]);

    bool valid = checkRange(x, y);
    if (!valid)
    {
        printf("Values %d and %d are not valid. Exiting.\n", x, y);
        exit(1);
    }
    
    // At this point x and y should be valid
    

    return 0;
}

bool checkRange(int x, int y)
{
    // Validate x and y
    if (x > y)
        return false;
    if (x < 2)
        return false;


    // stub
    return false;
}

ubyte* checkFatTable()
{
    ubyte* fat = malloc(FAT_TABLE_SIZE);

    for (int i = 0; i < NUM_FAT_SECTORS; i++)
    {
        read_sector(i + 1, &fat[i * BYTES_PER_SECTOR]);
    }

    return;
}

void useage()
{
    printf("Useage: pfe [x] [y]\n");
    printf("Where x and y bounds for a FAT entry\n");
    exit(1);
}
