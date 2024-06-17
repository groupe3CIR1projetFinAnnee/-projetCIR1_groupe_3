#include <stdbool.h>
#include <stdio.h>

#ifndef PROJET_REGIONS_H
#define PROJET_REGIONS_H

#define LETTER_IN_ALPHABET 53
#define MAX_REGION_LENGTH 255

struct Region{
    struct Region* regions[LETTER_IN_ALPHABET];
    bool isRegion;
    unsigned int births;
};


// Creation & setup
struct Region* createEmptyRegion();
struct Region* createRegion();

// Access
void insertRegion(struct Region** regions, char* regionName); //WARNING : NO CAPS ALLOWED !!
unsigned int getBirths(struct Region* regionsTrie, char* regionName); //Recupere l'anniversaire du région a partir du gigaTree
struct Region* getChild(struct Region* regionsTrie, char letter);
bool isRegionEmpty(struct Region* regionsTrie);
struct Region* findRegion(struct Region* regionsTrie, char* regionName);
bool regionExists(struct Region* regionsTrie, char* regionName);
void addBirth(struct Region** regionsTrie, char* regionName);

/**
 * Convert c to an index to use with struct Region -> regions
 */
unsigned int charToTrieIndex(char c);

//Suppress
void deleteRegions(struct Region** regionsTrie);


#endif //PROJET_REGIONS_H
