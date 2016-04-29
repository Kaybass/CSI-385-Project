#include "utilities.h"

extern int read_sector(int sector_number, char* buffer);
extern int write_sector(int sector_number, char* buffer);
extern unsigned int get_fat_entry(int fat_entry_number, char* fat);
extern void set_fat_entry(int fat_entry_number, int value, char* fat);

extern FILE* FILE_SYSTEM_ID;
extern int BYTES_PER_SECTOR;

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
*/
short searchForFolder(short currentFLC, char * target){

    unsigned char * sector;

    char ** dirs;
    int     depth;

    int index = 0;

    if(target[0] == '/' && strlen(target) == 1)
        return 0;

    if(target[0] == '/' || currentFLC == 0){

        dirs = splitDirectoryString(target,&depth);

        //the way string splitting works in our case /dir makes depth 2

        if(target[0] == '/'){
            index++;
        }


        sector = (byte*)malloc(BYTES_PER_SECTOR * sizeof(ubyte));

        ShortFileInfo entries[16];

        /*  Massive bad boy points here, we know that
            every example image has a root folder that
            is only one sector long and we're taking
            advantage of that here.*/
        read_sector(19,sector);

        for(int i = 0; i < BYTES_PER_SECTOR / 32; i++){
            for(int j = 0; j < 8; j++){
                entries[i].Filename[j] = sector[j + i * 32];
            }
            entries[i].Filename[8] = '\0';

            entries[i].Attributes = sector[11 + i * 32];

            int h = ( ( (int) sector[27 + i * 32] ) << 8 ) & 0x0000ff00;
            int l =   ( (int) sector[26 + i * 32] )        & 0x000000ff;
            entries[i].FirstLogicalCluster = h | l;
        }

        free(sector);

        for(int i = 0; i < 16; i++){

            int h = strlen(entries[i].Filename);
            int j = strlen(dirs[index]);

            if(h > j){
                entries[i].Filename[j] = '\0';
            }
            if(strcmp(dirs[index],entries[i].Filename) == 0 &&
                (FAT_SUBDIRECTORY & entries[i].Attributes) != 0){

                if(depth > index + 1){
                    return searchHarderForFolder(entries[i].FirstLogicalCluster,dirs,index + 1,depth);
                }
                else{

                    return entries[i].FirstLogicalCluster;
                }
            }
        }
        return -1;

    }
    else{
        int  length = 0;
        int *weTheSectors;



        dirs = splitDirectoryString(target,&depth);

        ubyte *fatTable = readFatTable(512 * 9, 9 , 512);
        weTheSectors = lookupSectors(currentFLC, &length, fatTable);

        free(fatTable);

        ShortFileInfo* entries = (ShortFileInfo*)malloc(length * 16 * sizeof(ShortFileInfo));

        for(int i = 0; i < length; i++){
            sector = (byte*)malloc(BYTES_PER_SECTOR * sizeof(ubyte));

            read_sector(weTheSectors[i],sector);

            for(int h = 0 + (i * 16); h < 16 + (i * 16); h++){
                for(int j = 0; j < 8; j++){
                    entries[h].Filename[j] = sector[j + (h - i * 16) * 32];
                }
                entries[h].Filename[8] = '\0';
                entries[h].Attributes = sector[11 + (h - i * 16) * 32];

                int e = ( ( (int) sector[27 + (h - i * 16) * 32] ) << 8 ) & 0x0000ff00;
                int l =   ( (int) sector[26 + (h - i * 16) * 32] )        & 0x000000ff;
                entries[h].FirstLogicalCluster = e | l;
            }

            free(sector);
        }
        free(weTheSectors);

        for(int i = 0; i < 16 * length; i++){

            int h = strlen(entries[i].Filename);
            int j = strlen(dirs[index]);

            if(h > j){
                entries[i].Filename[j] = '\0';
            }

            if(strcmp(dirs[index],entries[i].Filename) == 0 &&
                (FAT_SUBDIRECTORY & entries[i].Attributes) != 0){

                if(depth != index){

                    short tmp = entries[i].FirstLogicalCluster;
                    free(entries);
                    return searchHarderForFolder(tmp,dirs,index + 1,depth);
                }
                else{

                    short tmp = entries[i].FirstLogicalCluster;
                    free(entries);
                    return tmp;
                }
            }
        }
        free(entries);
        return -1;
    }
}

short searchHarderForFolder(short currentFLC, char ** dirs, int index, int depth){

    unsigned char * sector;

    if(currentFLC == 0){

        sector = (byte*)malloc(BYTES_PER_SECTOR * sizeof(ubyte));

        ShortFileInfo entries[16];

        /*  Massive bad boy points here, we know that
            every example image has a root folder that
            is only one sector long and we're taking
            advantage of that here.*/
        read_sector(19,sector);

        for(int i = 0; i < 16; i++){
            for(int j = 0; j < 8; j++){
                entries[i].Filename[j] = sector[j + i * 32];
            }
            entries[i].Filename[8] = '\0';

            entries[i].Attributes = sector[11 + i * 32];

            int h = ( ( (int) sector[27 + i * 32] ) << 8 ) & 0x0000ff00;
            int l =   ( (int) sector[26 + i * 32] )        & 0x000000ff;
            entries[i].FirstLogicalCluster = h | l;
        }

        free(sector);

        for(int i = 0; i < 16; i++){

            int h = strlen(entries[i].Filename);
            int j = strlen(dirs[index]);

            if(h > j){
                entries[i].Filename[j] = '\0';
            }

            if(strcmp(dirs[1],entries[i].Filename) == 0 &&
                (FAT_SUBDIRECTORY & entries[i].Attributes) != 0){

                if(depth > index + 1){

                    return searchHarderForFolder(entries[i].FirstLogicalCluster,dirs,index + 1,depth);
                }
                else{

                    return entries[i].FirstLogicalCluster;
                }
            }
        }
        return -1;

    }
    else{
        int  length = 0;
        int *weTheSectors;

        ubyte *fatTable = readFatTable(512 * 9, 9 , 512);
        weTheSectors = lookupSectors(currentFLC, &length, fatTable);

        free(fatTable);

        ShortFileInfo* entries = (ShortFileInfo*)malloc(length * 16 * sizeof(ShortFileInfo));

        for(int i = 0; i < length; i++){
            sector = (byte*)malloc(BYTES_PER_SECTOR * sizeof(ubyte));
            read_sector(weTheSectors[i],sector);
            for(int h = 0 + (i * 16); h < 16 + (i * 16); h++){
                for(int j = 0; j < 8; j++){
                    entries[h].Filename[j] = sector[j + (h - i * 16) * 32];
                }
                entries[h].Filename[8] = '\0';

                entries[h].Attributes = sector[11 + (h - i * 16) * 32];

                int e = ( ( (int) sector[27 + (h - i * 16) * 32] ) << 8 ) & 0x0000ff00;
                int l =   ( (int) sector[26 + (h - i * 16) * 32] )        & 0x000000ff;
                entries[h].FirstLogicalCluster = e | l;
            }

            free(sector);
        }
        free(weTheSectors);

        for(int i = 0; i < 16 * length; i++){
            int h = strlen(entries[i].Filename);
            int j = strlen(dirs[index]);

            if(h > j){
                entries[i].Filename[j] = '\0';
            }
            if(strcmp(dirs[index],entries[i].Filename) == 0 &&
                (FAT_SUBDIRECTORY & entries[i].Attributes) != 0){

                if(depth > index + 1){

                    short tmp = entries[i].FirstLogicalCluster;
                    free(entries);
                    return searchHarderForFolder(tmp,dirs,index + 1,depth);
                }
                else{

                    short tmp = entries[i].FirstLogicalCluster;
                    free(entries);
                    return tmp;
                }
            }
        }
        free(entries);
        return -1;
    }
}

short searchForFile(short currentFLC, char * target){

        unsigned char * sector;

        char ** dirs;
        int     depth;

        int index = 0;

        if(target[0] == '/' && strlen(target) == 1)
            return 0;

        if(target[0] == '/' || currentFLC == 0){

            dirs = splitDirectoryString(target,&depth);

            //the way string splitting works in our case /dir makes depth 2

            if(target[0] == '/'){
                index++;
            }


            sector = (byte*)malloc(BYTES_PER_SECTOR * sizeof(ubyte));

            ShortFileInfo entries[16];

            /*  Massive bad boy points here, we know that
                every example image has a root folder that
                is only one sector long and we're taking
                advantage of that here.*/
            read_sector(19,sector);

            for(int i = 0; i < BYTES_PER_SECTOR / 32; i++){
                for(int j = 0; j < 8; j++){
                    entries[i].Filename[j] = sector[j + i * 32];
                }
                entries[i].Filename[8] = '\0';

                entries[i].Attributes = sector[11 + i * 32];

                int h = ( ( (int) sector[27 + i * 32] ) << 8 ) & 0x0000ff00;
                int l =   ( (int) sector[26 + i * 32] )        & 0x000000ff;
                entries[i].FirstLogicalCluster = h | l;
            }

            free(sector);

            for(int i = 0; i < 16; i++){

                if(depth > index + 1){
                    int h = strlen(entries[i].Filename);
                    int j = strlen(dirs[index]);

                    if(h > j){
                        entries[i].Filename[j] = '\0';
                    }
                    if(strcmp(dirs[1],entries[i].Filename) == 0 &&
                        (FAT_SUBDIRECTORY & entries[i].Attributes) != 0){

                        return searchHarderForFile(entries[i].FirstLogicalCluster,dirs,index + 1,depth);

                    }
                }
                else{
                    int godhelpusifthisisnttwo;
                    char ** theSplit = splitFilenameExtension(dirs[index], &godhelpusifthisisnttwo);

                    if(godhelpusifthisisnttwo != 2)
                        return -1;

                    int h = strlen(entries[i].Filename);
                    int j = strlen(theSplit[0]);

                    if(h > j){
                        entries[i].Filename[j] = '\0';
                    }

                    h = strlen(entries[i].Type);
                    j = strlen(theSplit[1]);

                    if(h > j){
                        entries[i].Type[j] = '\0';
                    }

                    char ctmp[20] = "";

                    strcat(ctmp,entries[i].Filename);
                    strcat(ctmp,".");
                    strcat(ctmp,entries[i].Type);

                    if(strcmp(dirs[index],ctmp) == 0 &&
                        (FAT_SUBDIRECTORY & entries[i].Attributes) == 0){

                        return entries[i].FirstLogicalCluster;

                    }
                }
            }
            return -1;

        }
        else{
            int  length = 0;
            int *weTheSectors;



            dirs = splitDirectoryString(target,&depth);

            ubyte *fatTable = readFatTable(512 * 9, 9 , 512);
            weTheSectors = lookupSectors(currentFLC, &length, fatTable);

            free(fatTable);

            ShortFileInfo* entries = (ShortFileInfo*)malloc(length * 16 * sizeof(ShortFileInfo));

            for(int i = 0; i < length; i++){
                sector = (byte*)malloc(BYTES_PER_SECTOR * sizeof(ubyte));

                read_sector(weTheSectors[i],sector);

                for(int h = 0 + (i * 16); h < 16 + (i * 16); h++){
                    for(int j = 0; j < 8; j++){
                        entries[h].Filename[j] = sector[j + (h - i * 16) * 32];
                    }
                    entries[h].Filename[8] = '\0';
                    entries[h].Attributes = sector[11 + (h - i * 16) * 32];

                    int e = ( ( (int) sector[27 + (h - i * 16) * 32] ) << 8 ) & 0x0000ff00;
                    int l =   ( (int) sector[26 + (h - i * 16) * 32] )        & 0x000000ff;
                    entries[h].FirstLogicalCluster = e | l;
                }

                free(sector);
            }
            free(weTheSectors);

            for(int i = 0; i < 16 * length; i++){

                if(depth > index + 1){
                    int h = strlen(entries[i].Filename);
                    int j = strlen(dirs[index]);

                    if(h > j){
                        entries[i].Filename[j] = '\0';
                    }
                    if(strcmp(dirs[1],entries[i].Filename) == 0 &&
                        (FAT_SUBDIRECTORY & entries[i].Attributes) != 0){

                        short tmp = entries[i].FirstLogicalCluster;
                        free(entries);
                        return searchHarderForFile(tmp,dirs,index + 1,depth);

                    }
                }
                else{
                    int godhelpusifthisisnttwo;
                    char ** theSplit = splitFilenameExtension(dirs[index], &godhelpusifthisisnttwo);

                    if(godhelpusifthisisnttwo != 2)
                        return -1;

                    int h = strlen(entries[i].Filename);
                    int j = strlen(theSplit[0]);

                    if(h > j){
                        entries[i].Filename[j] = '\0';
                    }

                    h = strlen(entries[i].Type);
                    j = strlen(theSplit[1]);

                    if(h > j){
                        entries[i].Type[j] = '\0';
                    }

                    char ctmp[20] = "";

                    strcat(ctmp,entries[i].Filename);
                    strcat(ctmp,".");
                    strcat(ctmp,entries[i].Type);

                    if(strcmp(dirs[index],ctmp) == 0 &&
                        (FAT_SUBDIRECTORY & entries[i].Attributes) == 0){

                        short tmp = entries[i].FirstLogicalCluster;
                        free(entries);
                        return tmp;
                    }
                }
            }
            free(entries);
            return -1;
        }
}

short searchHarderForFile(short currentFLC, char ** dirs, int index, int depth){

        unsigned char * sector;

        if(currentFLC == 0){

            sector = (byte*)malloc(BYTES_PER_SECTOR * sizeof(ubyte));

            ShortFileInfo entries[16];

            /*  Massive bad boy points here, we know that
                every example image has a root folder that
                is only one sector long and we're taking
                advantage of that here.*/
            read_sector(19,sector);

            for(int i = 0; i < 16; i++){
                for(int j = 0; j < 8; j++){
                    entries[i].Filename[j] = sector[j + i * 32];
                }
                entries[i].Filename[8] = '\0';

                entries[i].Attributes = sector[11 + i * 32];

                int h = ( ( (int) sector[27 + i * 32] ) << 8 ) & 0x0000ff00;
                int l =   ( (int) sector[26 + i * 32] )        & 0x000000ff;
                entries[i].FirstLogicalCluster = h | l;
            }

            free(sector);

            for(int i = 0; i < 16; i++){

                if(depth > index + 1){
                    int h = strlen(entries[i].Filename);
                    int j = strlen(dirs[index]);

                    if(h > j){
                        entries[i].Filename[j] = '\0';
                    }
                    if(strcmp(dirs[1],entries[i].Filename) == 0 &&
                        (FAT_SUBDIRECTORY & entries[i].Attributes) != 0){

                        return searchHarderForFile(entries[i].FirstLogicalCluster,dirs,index + 1,depth);

                    }
                }
                else{
                    int godhelpusifthisisnttwo;
                    char ** theSplit = splitFilenameExtension(dirs[index], &godhelpusifthisisnttwo);

                    if(godhelpusifthisisnttwo != 2)
                        return -1;

                    int h = strlen(entries[i].Filename);
                    int j = strlen(theSplit[0]);

                    if(h > j){
                        entries[i].Filename[j] = '\0';
                    }

                    h = strlen(entries[i].Type);
                    j = strlen(theSplit[1]);

                    if(h > j){
                        entries[i].Type[j] = '\0';
                    }

                    char ctmp[20] = "";

                    strcat(ctmp,entries[i].Filename);
                    strcat(ctmp,".");
                    strcat(ctmp,entries[i].Type);

                    if(strcmp(dirs[index],ctmp) == 0 &&
                        (FAT_SUBDIRECTORY & entries[i].Attributes) == 0){

                        return entries[i].FirstLogicalCluster;

                    }
                }
            }
            return -1;

        }
        else{
            int  length = 0;
            int *weTheSectors;

            ubyte *fatTable = readFatTable(512 * 9, 9 , 512);
            weTheSectors = lookupSectors(currentFLC, &length, fatTable);

            free(fatTable);

            ShortFileInfo* entries = (ShortFileInfo*)malloc(length * 16 * sizeof(ShortFileInfo));

            for(int i = 0; i < length; i++){
                sector = (byte*)malloc(BYTES_PER_SECTOR * sizeof(ubyte));
                read_sector(weTheSectors[i],sector);
                for(int h = 0 + (i * 16); h < 16 + (i * 16); h++){
                    for(int j = 0; j < 8; j++){
                        entries[h].Filename[j] = sector[j + (h - i * 16) * 32];
                    }
                    entries[h].Filename[8] = '\0';

                    entries[h].Attributes = sector[11 + (h - i * 16) * 32];

                    int e = ( ( (int) sector[27 + (h - i * 16) * 32] ) << 8 ) & 0x0000ff00;
                    int l =   ( (int) sector[26 + (h - i * 16) * 32] )        & 0x000000ff;
                    entries[h].FirstLogicalCluster = e | l;
                }

                free(sector);
            }
            free(weTheSectors);

            for(int i = 0; i < 16 * length; i++){
                if(depth > index + 1){
                    int h = strlen(entries[i].Filename);
                    int j = strlen(dirs[index]);

                    if(h > j){
                        entries[i].Filename[j] = '\0';
                    }
                    if(strcmp(dirs[1],entries[i].Filename) == 0 &&
                        (FAT_SUBDIRECTORY & entries[i].Attributes) != 0){

                        short tmp = entries[i].FirstLogicalCluster;
                        free(entries);
                        return searchHarderForFile(tmp,dirs,index + 1,depth);

                    }
                }
                else{
                    int godhelpusifthisisnttwo;
                    char ** theSplit = splitFilenameExtension(dirs[index], &godhelpusifthisisnttwo);

                    if(godhelpusifthisisnttwo != 2)
                        return -1;

                    int h = strlen(entries[i].Filename);
                    int j = strlen(theSplit[0]);

                    if(h > j){
                        entries[i].Filename[j] = '\0';
                    }

                    h = strlen(entries[i].Type);
                    j = strlen(theSplit[1]);

                    if(h > j){
                        entries[i].Type[j] = '\0';
                    }

                    char ctmp[20] = "";

                    strcat(ctmp,entries[i].Filename);
                    strcat(ctmp,".");
                    strcat(ctmp,entries[i].Type);

                    if(strcmp(dirs[index],ctmp) == 0 &&
                        (FAT_SUBDIRECTORY & entries[i].Attributes) == 0){

                        short tmp = entries[i].FirstLogicalCluster;
                        free(entries);
                        return tmp;
                    }
                }
            }
            free(entries);
            return -1;
        }
}

char ** splitDirectoryString(char * directoryName, int *entryc){

    char *s = strdup(directoryName);
    int tokensAllocated = 1;
    int tokensUsed = 0;

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

    *entryc = tokensUsed;
    free(s);

    return tokens;
}

char ** splitFilenameExtension(char * file, int *shouldbetwo){

    char *s = strdup(file);
    int tokensAllocated = 1;
    int tokensUsed = 0;

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

    *shouldbetwo = tokensUsed;
    free(s);

    return tokens;
}

int* lookupSectors(int FLC, int * length, ubyte* image){

    int* sectors = (int*)malloc(10 * sizeof(int));
    int currEntry = FLC;
    int end = 0;
    *length = 1;

    sectors[0] = currEntry + 31;

    while (!end && *length <= 10)
    {
        currEntry = get_fat_entry(currEntry, (byte*)image);
        if(currEntry < 0xFF8){
            sectors[*length] = currEntry + 31;
            (*length)++;
        }
        else{
            end = 1;
        }
    }

    return sectors;
}
