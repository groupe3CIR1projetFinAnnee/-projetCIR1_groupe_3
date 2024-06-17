#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "regions.h"
#include "gigaTree.h"
#include "person.h"


/**
 * Create a new GigaTree, and allocates memory. Leaves all attributes empty.
 * Attributes should be set up with the csvReader.
 * @return The created GigaTree
 */
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
    gigaTree->mostBirthsRegion = NULL;


    //We wont setup mostBirthRegions, since it must be done with a malloc. We will leave it empty here
    gigaTree->mostBirths = 0;
    gigaTree->numberPersons = 0;
    gigaTree->regionsTrie = createEmptyRegion();

    return gigaTree;
}


//TODO Implementer avec regions
//void setupGigaTree(struct GigaTree* gigaTree,struct Person** people, struct Region* regionsTrie);

/**
 * Get the youngest person the GigaTree contains. O(1)
 * @param gigaTree The GigaTree to search in.
 * @return The youngest person of gigaTree
 */
struct Person* getYoungest(struct GigaTree* gigaTree){
    return gigaTree->youngest;
}

/**
 * Get the oldest person the GigaTree contains. O(1)
 * @param gigaTree The GigaTree to search in.
 * @return The oldest person of gigaTree
 */
struct Person* getOldest(struct GigaTree* gigaTree){
    return gigaTree->oldest;
}

//TODO Implementer avec regions
/**
 * Get the number of births in the given specific region of a GigaTree. O(|region|)
 * @param gigaTree The GigaTree to search in.
 * @param region Will return number of births in this region.
 * @return Returns the number of births in the given region.
 */
unsigned int getNumberOfBirths(struct GigaTree* gigaTree, char* region){
    return 0;
}

/**
 * Get the region in which the most people were born, O(1)
 * @param gigaTree The GigaTree to search in.
 * @return Return the name of the region in which the most people were born.
 */
char* mostBirthsRegion(struct GigaTree* gigaTree){
    return gigaTree->mostBirthsRegion;
}

/**
 * Get the number of people having a specific birthday. O(1)
 * @param gigaTree The GigaTree to search in.
 * @param month Birthday month 
 * @param day Birthday day
 * @return Returns the number of people born the given day.
 */
unsigned int births(struct GigaTree* gigaTree, unsigned int month, unsigned int day){
    return gigaTree->birthday[month-1][day-1];
}

/**
 * Get a person using the given index. Can be used to loop within all the persons.
 * @param gigaTree The GigaTree to search in.
 * @param index Requested index.
 * @return The person at the given index
 */
struct Person* getPersonByIndex(struct GigaTree* gigaTree, unsigned int index){
    return gigaTree->people[index];
}

/**
 * Get the total number of person within the given GigaTree, including the "unknown" person.
 * @param gigaTree The GigaTree to search in.
 * @return Number of person in gigaTree. /!\ Includes the "unknown" person.
 */
unsigned int numberPersons(struct GigaTree* gigaTree){
    return gigaTree->numberPersons;
}

/**
 * Get GigaTree regions trie
 * @param gigaTree A GigaTree
 * @return retions trie of the GigaTrie
 */
struct Region* getRegionTrie(struct GigaTree* gigaTree) {
    return gigaTree->regionsTrie;
}

/**
 * Get the array containing people within the given GigaTree.
 * /!\ The array is not copied. Any changes to this array should be done knowing the way GigaTree works.
 * @param gigaTree The GigaTree containing requested people.
 * @return Array of Person
 */
struct Person** getPeople(struct GigaTree* gigaTree){
    return gigaTree->people;
}

unsigned int numberMale(struct GigaTree* gigaTree){
    unsigned int count = 0;
    unsigned int numberPerson = numberPersons(gigaTree);
    for(unsigned int index = 0; index < numberPerson; index++){
        struct Person* person = getPersonByIndex(gigaTree,index);
        if(getSex(person) == 1) //1 is for male
            count++;
    }
    return count;
}

unsigned int numberFemale(struct GigaTree* gigaTree){
    unsigned int count = 0;
    unsigned int numberPerson = numberPersons(gigaTree);
    for(unsigned int index = 0; index < numberPerson; index++){
        struct Person* person = getPersonByIndex(gigaTree,index);
        if(getSex(person) == 0) //0 is for female
            count++;
    }
    return count;
}


unsigned int inbreeding(struct GigaTree* gigaTree){
    unsigned int ancestorNumber = 7;
    struct Person** people = getPeople(gigaTree);
    unsigned int numberPeople = numberPersons(gigaTree);
    unsigned int inbreedingCount = 0; //hope it stays here...

    for(int i = 1; i < numberPeople; i++){
        struct Person** dadSide = malloc(sizeof(struct Person*) * ancestorNumber);
        struct Person** momSide = malloc(sizeof(struct Person*) * ancestorNumber);
        if(dadSide == NULL){
            printf("Malloc error : dad side");
            return 0;
        }

        dadSide[0] = getPadre(people[i]);
        dadSide[1] = getPadre(dadSide[0]);
        dadSide[2] = getMadre(dadSide[0]);
        dadSide[3] = getPadre(dadSide[1]);
        dadSide[4] = getMadre(dadSide[1]);
        dadSide[5] = getPadre(dadSide[2]);
        dadSide[6] = getMadre(dadSide[2]);

        momSide[0] = getMadre(people[i]);
        momSide[1] = getPadre(momSide[0]);
        momSide[2] = getMadre(momSide[0]);
        momSide[3] = getPadre(momSide[1]);
        momSide[4] = getMadre(momSide[1]);
        momSide[5] = getPadre(momSide[2]);
        momSide[6] = getMadre(momSide[2]);

        bool doBreak = 0;
        for(unsigned int personD = 0; personD < ancestorNumber; personD++){
            for(unsigned int personM = 0; personM < ancestorNumber; personM++) {
                if ((dadSide[personD] != NULL) && (momSide[personM] != NULL) && (getID(dadSide[personD]) != 0) && (getID(dadSide[personD]) == getID(momSide[personM]))) {
                    inbreedingCount++;
                    doBreak = 1;
                }

            }
            if(doBreak == 1)
                break;
        }


        free(dadSide);
        free(momSide);
    }
    return inbreedingCount;
}


/**
 * Delete the given GigaTree
 * @param gigaTree The GigaTree to delete
 */
void deleteGigaTree(struct GigaTree** gigaTree){
    struct Person** pers = getPeople(*gigaTree);
    unsigned int size = numberPersons(*gigaTree);
    for(int i = 0; i < size; i++){
        deletePerson(&pers[i]); //free every person
    }
    free((*gigaTree)->people);
    if((*gigaTree)->mostBirthsRegion != NULL)
        free((*gigaTree)->mostBirthsRegion);
    deleteRegions(&((*gigaTree)->regionsTrie));
    free((*gigaTree));

    *gigaTree = NULL;
}