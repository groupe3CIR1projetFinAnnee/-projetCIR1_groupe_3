#ifndef PROJET_REGIONS_H
#define PROJET_REGIONS_H
#define LETTER_IN_ALPHABET 27
#define MAX_REGION_LENGTH
#include <stdbool.h>
#include <stdio.h>
#include "gigaTree.h"

struct Region{
    struct Region* regions[LETTER_IN_ALPHABET];
    bool isRegion;
    unsigned int births;
};


// Creation & setup
struct Region* createEmptyRegions();
struct Region* createRegions();

// Access
void insertRegion(struct Region** regions, char* regionName); //WARNING : NO CAPS ALLOWED !!
unsigned int getBirths(struct GigaTree* gigaTree, char* regionName); //Recupere l'anniversaire du région a partir du gigaTree
struct Region* getChild(struct Region* regions, char letter);
bool isRegionEmpty(struct Region* regions);
struct Region* findRegion(struct Region* region, char* regionName);


//Suppress
void deleteRegions(struct Region** regions);




#endif //PROJET_REGIONS_H
