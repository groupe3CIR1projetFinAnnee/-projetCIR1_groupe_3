#include "csvReader.h"
#include <stdio.h>
#include <stdbool.h>



struct GigaTree* readCSV(char* filePath){ //The file path should look like this : ./home/folder/file.txt
    FILE* file = NULL;

    file = fopen(("%s",filePath), "r+");

    if(file == NULL){
        printf("Error opening the file. Make sure you entered the right path and the extension. \nExample : ./home/folder/file.txt\n");
        return NULL;
    }
    printf("yaay");
    return 0;
}