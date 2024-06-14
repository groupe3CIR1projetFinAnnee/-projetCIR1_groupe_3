#include "gigaTree.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "person.h"

struct GigaTree* createEmptyGigaTree(){
    struct GigaTree* gigaTree = malloc(sizeof(struct GigaTree));
    if(gigaTree == NULL)
        return NULL;

    for(int i = 0; i < 12; i ++){ //Will go over the array for birthday and edit it to put 0 everywhere
        for(int y = 0; y < 31; y++){
            gigaTree->birthday[i][y] = 0;
        }
    }

    gigaTree->people = NULL;
    gigaTree->regionsTrie = NULL;
    gigaTree->oldest = NULL;
    gigaTree->youngest = NULL;


    //We wont setup mostBirthRegions, since it must be done with a malloc. We will leave it empty here
    gigaTree->mostBirths = 0;
    gigaTree->numberPersons = 0;

    return gigaTree;
}


//TODO Implementer avec regions
//void setupGigaTree(struct GigaTree* gigaTree,struct Person** people, struct Region* regionsTrie);

struct Person* getYoungest(struct GigaTree* gigaTree){
    return gigaTree->youngest;
}

struct Person* getOldest(struct GigaTree* gigaTree){
    return gigaTree->oldest;
}

//TODO Implementer avec regions
unsigned int getNumberOfBirths(struct GigaTree* gigaTree, char* region){
    return 0;
}

char* mostBirthsRegion(struct GigaTree* gigaTree){
    return gigaTree->mostBirthsRegion;
}

unsigned int births(struct GigaTree* gigaTree, unsigned int month, unsigned int day){
    return gigaTree->birthday[month][day];
}

struct Person* getPersonByIndex(struct GigaTree* gigaTree, unsigned int index){
    return gigaTree->people[index];
}

unsigned int numberPersons(struct GigaTree* gigaTree){
    return gigaTree->numberPersons;
}

struct Person** getPeople(struct GigaTree* gigaTree){
    return gigaTree->people;
}

void deleteGigaTree(struct GigaTree** gigaTree){
    struct Person** pers = getPeople(*gigaTree);
    unsigned int size = numberPersons(*gigaTree);
    for(int i = 0; i < size; i++){
        deletePerson(&pers[i]); //free every person
    }
    free((*gigaTree)->people);
    free((*gigaTree)->mostBirthsRegion);
    free((*gigaTree));
    //TODO include deleting the trie (regions)


    *gigaTree = NULL;

}