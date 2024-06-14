#include "csvReader.h"
#include "person.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


struct GigaTree* readCSV(char* filePath){ //The file path should look like this : ./home/folder/file.txt
    FILE* file = NULL;
    char line[256];

    file = fopen(filePath, "r");
    if(file == NULL){
        perror("Error opening the file"); //perror allow us to show the error after the text !
        printf("Tips :\n-Check if your path is right\n-Don't forget to put the extension\n-Example : ./folder/file.csv\n");
        return NULL;
    }
    printf("Loading the file and setting up the information...\nThis operation can take a while...\n");

    //Setting up everything
    int character;
    fgets(line, sizeof(line), file); //get the first element, being the number of person in the list
    int numberOfPerson = atoi(line);
    struct Person** people = malloc(sizeof(struct Person*) * numberOfPerson); //Create the struct person array. The size of this array is the number of ppl * the size of a pointer to an array
    fgets(line, sizeof(line), file); //I'm jumping the next line, being the "unknow person". We will start with persons
    //TODO : read and create persons by line. now, i'm working on person.c , so come back later
    for(int i=0; i < numberOfPerson; i++){
        struct Person* p = createEmptyPerson();
        p->birthYear = i+1;
        people[i] = p; //To change, its temporary
    }

/*
    while(character != EOF){
        character = fgetc(file);
        printf("%c",character);
    }
*/

    fclose(file);
}