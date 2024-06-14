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
    gigaTree->oldest = createEmptyPerson();
    gigaTree->youngest = createEmptyPerson();

    if(gigaTree->oldest == NULL || gigaTree->youngest == NULL){
        free(gigaTree->oldest);
        free(gigaTree->youngest);
        free(gigaTree);
        return NULL;
    }
    //We wont setup mostBirthRegions, since it must be done with a malloc. We will leave it empty here


    return gigaTree;
}