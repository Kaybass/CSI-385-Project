#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h> // We are using std99 so this should work

#include "../utils/fatSupport.h"

#define BYTES_TO_READ_IN_BOOT_SECTOR 512

FILE* FILE_SYSTEM_ID;
int BYTES_PER_SECTOR;

bool checkRange(int x, int y);

void useage();

int main(int argc, char** argv)
{
    unsigned char* boot;
    int x;
    int y;

    // Handle commandline args
    if (argc == 1 || argc > 2)
    {
        useage();
    }
    
    x = atoi(argv[1]);
    y = atoi(argv[2]);

    

    return 0;
}

bool checkRange(int x, int y)
{
    // stub
    return false;
}

void useage()
{
    printf("Useage: pfe [x] [y]\n");
    printf("Where x and y bounds for a FAT entry\n");
    exit(1);
}
