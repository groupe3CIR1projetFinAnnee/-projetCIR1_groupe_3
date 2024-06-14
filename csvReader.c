#include "csvReader.h"
#include "person.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "gigaTree.h"



struct GigaTree* readCSV(char* filePath){ //The file path should look like this : ./home/folder/file.txt
    FILE* file = NULL;
    char line[256];
    struct GigaTree* gigaTree = createEmptyGigaTree();

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
    int numberOfPerson = atoi(line) + 1; //we add the unknown person to the start of the array
    struct Person** people = malloc(sizeof(struct Person*) * numberOfPerson); //Create the struct person array. The size of this array is the number of ppl * the size of a pointer to an array
    //fgets(line, sizeof(line), file);


    for(int i = 0; i < numberOfPerson; i++){ //We are going "number of person" times. This is useful, since a while would end up into an endless loop
        fgets(line, sizeof(line), file);
        //printf("%s",line);
        unsigned int id, padreID, madreID;
        char* birthday = "";
        char* firstname = "";
        char* lastname = "";
        char* region = "";
        struct Person* padre,madre;

        char* line_copy = strdup(line);
        char* token = strtok(line_copy, ","); //strtok is going to read the line for us, stoping at ,. This modify the line, so we have to copy it before

        unsigned int comp = 1; //Comp to know were we are
        bool younger;
        bool older;
        while (token != NULL) {
            switch(comp){
                case 1:
                    id = atoi(token);
                    break;
                case 2:
                    padreID = atoi(token);
                    break;
                case 3:
                    madreID = atoi(token);
                    break;
                case 4:
                    lastname = malloc((strlen(token)+1) * sizeof(char));
                    strcpy(lastname,token); //We copy the name !
                    break;
                case 5:
                    firstname = malloc((strlen(firstname)+1) * sizeof(char));
                    strcpy(firstname,token);
                    break;
                case 6 :
                    birthday = malloc((strlen(birthday)+1) * sizeof(char));
                    younger = isYoungest(gigaTree->youngest,token);
                    older = isOldest(gigaTree->oldest,token);
                    strcpy(birthday,token);
                    break;
                case 7:
                    region = malloc((strlen(region)+1) * sizeof(char));
                    strtok(token,"\n");
                    strcpy(region,token);
                    break;
            }

            token = strtok(NULL, ","); //To tell the token to go forward
            comp++;
        }
        people[i] = createPerson(id,firstname,lastname,birthday,padreID,madreID,region); //We create the person and put it in the array
        free(firstname);
        free(lastname);
        free(birthday);
        free(region);
        if(younger == true)
            gigaTree->youngest = people[i];
        if(older == true)
            gigaTree->oldest = people[i];
        free(line_copy);
    }


    fclose(file);
    //printf("%d",people[0]->birthDay); //Warning : the array goes from 0 to 40 if the number of ppl is 40 !! The person 0 is the unknow one
    gigaTree->people = people; //Setting up the array into the tree
    gigaTree->numberPersons = numberOfPerson; //Remember here !! the unknown person is counted, at the position 0, everytime.
    return gigaTree;
}


bool isYoungest(struct Person* youngest, char* birthday){
    if(youngest == NULL || youngest->birthDay == 0){ //The first time, youngest will be null & can be the null person
        return true;
    }
    unsigned int* youngestBirthday = getBirthday(youngest);
    unsigned int* personBirthday = splitBirthday(birthday);
    bool result = (youngestBirthday[2] < personBirthday[2]) ||
            (youngestBirthday[2] == personBirthday[2] && youngestBirthday[1] < personBirthday[1]) ||
            (youngestBirthday[2] == personBirthday[2] && youngestBirthday[1] == personBirthday[1] && youngestBirthday[0] < personBirthday[0]);
    free(youngestBirthday);
    free(personBirthday);
    return result;
}

bool isOldest(struct Person* oldest, char* birthday){
    if(oldest == NULL || oldest->birthDay == 0){ //the first time, oldest will be null & can be the null person
        return true;
    }
    unsigned int* oldestBirthday = getBirthday(oldest);
    unsigned int* personBirthday = splitBirthday(birthday);
    bool result = (oldestBirthday[2] > personBirthday[2]) ||
           (oldestBirthday[2] == personBirthday[2] && oldestBirthday[1] > personBirthday[1]) ||
           (oldestBirthday[2] == personBirthday[2] && oldestBirthday[1] == personBirthday[1] && oldestBirthday[0] > personBirthday[0]);
    free(oldestBirthday);
    free(personBirthday);
    return result;
}
