#include "csvReader.h"
#include "person.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "gigaTree.h"
#include "regions.h"

struct GigaTree* readCSV(char* filePath){ //The file path should look like this : ./home/folder/file.txt
    FILE* file = NULL;
    char line[256];
    struct GigaTree* gigaTree = createEmptyGigaTree();

    file = fopen(filePath, "r");
    if(file == NULL){
#ifdef DEBUG
        perror("Error opening the file"); //perror allow us to show the error after the text !
        printf("Tips :\n-Check if your path is right\n-Don't forget to put the extension\n-Example : ./folder/file.csv\n");
#endif
        return NULL;
    }
    printf("Loading the file and setting up the information...\nThis operation can take a while...\n");

    //Setting up everything
    fgets(line, sizeof(line), file); //get the first element, being the number of person in the list
    int numberOfPerson = atoi(line) + 1; //we add the unknown person to the start of the array
    struct Person** people = malloc(sizeof(struct Person*) * numberOfPerson); //Create the struct person array. The size of this array is the number of ppl * the size of a pointer to an array

    for(int i = 0; i < numberOfPerson; i++){ //We are going "number of person" times. This is useful, since a while would end up into an endless loop
        fgets(line, sizeof(line), file);
        unsigned int id, padreID, madreID;
        char* birthday = "";
        char* firstname = "";
        char* lastname = "";
        char* region = "";
        struct Person* padre,madre;

        char* line_copy = strdup(line);
        line_copy = strtok(line_copy,"\r");
        char* token = strtok(line_copy, ","); //strtok is going to read the line for us, stoping at ,. This modify the line, so we have to copy it before
        char* newToken;

        unsigned int comp = 1; //Comp to know were we are
        unsigned int regionBirths;
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
                    if(lastname == NULL)
                        return NULL;
                    strcpy(lastname,token); //We copy the name !
                    break;
                case 5:
                    firstname = malloc((strlen(firstname)+1) * sizeof(char));
                    if(firstname == NULL)
                        return NULL;
                    strcpy(firstname,token);
                    break;
                case 6 :
                    birthday = malloc((strlen(birthday)+1) * sizeof(char));
                    if(birthday == NULL)
                        return NULL;
                    younger = isYoungest(gigaTree->youngest,token);
                    older = isOldest(gigaTree->oldest,token);
                    addBirthday(token,gigaTree); //Launch the function that add the birthday to the gigaTree array
                    strcpy(birthday,token);
                    break;
                case 7:
                    newToken = strtok(strdup(line), ",");
                    for(int w = 1; w < 7; w++)
                        newToken = strtok(NULL,",");
                    newToken = strtok(newToken,"\n");
                    region = malloc((strlen(newToken)+1) * sizeof(char));
                    if(region == NULL)
                        return NULL;
                    strtok(token, "\n");
                    strcpy(region, newToken);

                    addBirth(&(gigaTree->regionsTrie), newToken);
                    regionBirths = getBirths(getRegionTrie(gigaTree), newToken);
                    if (regionBirths > gigaTree->mostBirths) {
                        gigaTree->mostBirths = regionBirths;
                        if (gigaTree->mostBirthsRegion != NULL) {
                            free(gigaTree->mostBirthsRegion);
                        }
                        gigaTree->mostBirthsRegion = malloc((strlen(newToken)+1)*sizeof(char));   // TODO check if NULL
                        if(gigaTree->mostBirthsRegion == NULL)
                            return NULL;
                        strcpy(gigaTree->mostBirthsRegion, newToken);
                    }
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
    addParents(gigaTree); //the final boss
    gigaTree->numFamilies = calculateNumberFamilies(gigaTree);
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


void addBirthday(char* birthday, struct GigaTree* gigaTree){
    unsigned int* birth = splitBirthday(birthday);
    if(birth[0] != 0) {
        (gigaTree->birthday[birth[1] - 1][birth[0] - 1])++; //we add one to this birthday, in the bithday array. Dont forget that the array is going from 0 to 11 and 0 to 30
    }
    free(birth);
}

void addParents(struct GigaTree* gigaTree){ //the final boss
    struct Person** people = getPeople(gigaTree);
    unsigned int size = numberPersons(gigaTree);

    struct Person* child = people[0]; //null person . I'll need to set his parent to himself
    setMadre(child,people[0]);
    setPadre(child,people[0]);

    for(unsigned int i = 1; i < size; i++){ // i here start to one to skip the unkown person.
        child = people[i];
        unsigned int done = 0;
        for(unsigned int y = 0; y < size; y ++){ // We are not skipping the unknown here since it can be someone parent.
            if(getID(people[y]) == getMadreID(child)){
                setMadre(child,people[y]);
                done++;
            }
            if(getID(people[y]) == getPadreID(child)){
                setPadre(child,people[y]);
                done++;
            }
            if(done == 2)
                y = size; //cut the for if we found both parents
        }
    }
}