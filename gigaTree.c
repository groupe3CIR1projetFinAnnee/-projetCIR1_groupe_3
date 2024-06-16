#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

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


    //We wont setup mostBirthRegions, since it must be done with a malloc. We will leave it empty here
    gigaTree->mostBirths = 0;
    gigaTree->numberPersons = 0;

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
    return gigaTree->birthday[month][day];
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
 * Get the array containing people within the given GigaTree.
 * /!\ The array is not copied. Any changes to this array should be done knowing the way GigaTree works.
 * @param gigaTree The GigaTree containing requested people.
 * @return Array of Person
 */
struct Person** getPeople(struct GigaTree* gigaTree){
    return gigaTree->people;
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
    free((*gigaTree)->mostBirthsRegion);
    free((*gigaTree));
    //TODO include deleting the trie (regions)


    *gigaTree = NULL;
}