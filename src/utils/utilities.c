#include "utilities.h"

extern int read_sector(int sector_number, char* buffer);
extern int write_sector(int sector_number, char* buffer);
extern unsigned int get_fat_entry(int fat_entry_number, char* fat);
extern void set_fat_entry(int fat_entry_number, int value, char* fat);

ubyte* readFatTable(int fatTableSize,int numFatSectors,int bytesPerSector)
{
    ubyte* fat = malloc(fatTableSize);

    for (int i = 0; i < numFatSectors; i++)
    {
        read_sector(i + 1, &fat[i * bytesPerSector]);
    }

    return fat;
}

/*
 * Things assumed by this function:
 * - It's being fed a real directory i.e input checking was done
 * - I'm a bad prgrammer
 * - Like really really bad
 *
*/
short searchForFolder(short currentFLC, char * target){

    int entryCount;

    char ** dirs;
    int     depth;

    //not root
    if(target[0] != '/' && currentFLC != 0){

        //directory above
        if(target[0] == '.'){

            if(target[1] == '.'){

            }
            else{
                return currentFLC;
            }
        }
        //current directory
        else{


        }
    }
    //root
    else{

    }
}

char ** splitDirectoryString(char * directoryName, int entryc){

    char *s = strdup(directoryName);
    size_t tokensAllocated = 1;
    size_t tokensUsed = 0;

    char **tokens = calloc(tokensAllocated, sizeof(char*));
    char *token, *rest = s;

    while((token = strsep(&rest, DIR_CHAR)) != NULL){

        if(tokensUsed == tokensAllocated){

            tokensAllocated *= 2;
            tokens = realloc(tokens, tokensAllocated * sizeof(char*));
        }

        tokens[tokensUsed++] = strdup(token);
    }
    if(tokensUsed == 0){
        free(tokens);
        tokens = NULL;
    }
    else{
        tokens = realloc(tokens, tokensUsed * sizeof(char*));
    }

    entryc = tokensUsed;
    free(s);

    return tokens;
}
