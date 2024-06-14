#include "csvReader.h"
#include <stdio.h>
#include <stdbool.h>



struct GigaTree* readCSV(char* filePath){ //The file path should look like this : ./home/folder/file.txt
    FILE* file = NULL;

    file = fopen(filePath, "r");
    if(file == NULL){
        perror("Error opening the file"); //perror allow us to show the error after the text !
        printf("Tips :\n-Check if your path is right\n-Don't forget to put the extension\n-Example : ./folder/file.csv\n");
        return NULL;
    }
    printf("Loading the file and setting up the information...\nThis operation can take a while...\n");
    int cursorCharacter = 0; //Creating the cursor count
    do{
        int cursorCharacter = fgetc(file);
        printf("%c",cursorCharacter);
    } while (cursorCharacter != EOF);


    fclose(file);
}