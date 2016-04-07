#include "mash/shell.h"



int main(int argc, char ** argv){

    FILE *image;
    int status;

    if(argc == 1){
        printf("MASH needs a file image\n");
        exit(1);
    }
    else if(argc >= 2){
        if(strcmp(argv[1],"-h") == 0){
            printf("Example usage: MASH [image]\n");
            exit(1);
        }
        FILE_SYSTEM_ID = fopen(argv[1], "r+");
        if (FILE_SYSTEM_ID == NULL)
        {
            printf("Could not open the floppy drive or image.\n");
            exit(1);
        }
    }

    status = mashLoop(image);

    return status;
}
