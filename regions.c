#include "regions.h"

#include "regions.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ctype.h"

/**
 * Create a new region.
 * PRIVATE USAGE ONLY. Use createEmptyRegion instead
 */
struct Region* createRegion(){
    struct Region* region = malloc(sizeof(struct Region));
    if(region == NULL) {
        return NULL;
    }

    for(unsigned int i = 0; i < LETTER_IN_ALPHABET; i++){
        region->regions[i] = createEmptyRegion(); //We setup the array to nothing
    }
    region->isRegion = false;
    region->births = 0;
    return region;
}

struct Region* createEmptyRegion(){
    return NULL;
}


void insertRegion(struct Region** regions, char* regionName) {
    struct Region* r = *regions;
    if (regionName[0] == '\0') {    // End of the region name
        r->isRegion = true;
    }

    else {
        unsigned int index = charToTrieIndex(regionName[0]);
        if (index == LETTER_IN_ALPHABET) {      // If outside of range
            insertRegion(regions,regionName+1); //Skip character if not letter of space
        }
        else {
            insertRegion(&(r->regions[index]), regionName +1);
        }
    }
}

/**
 * Convert c to an index to use with struct Region -> regions
 */
unsigned int charToTrieIndex(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    }
    else {
        if (c >= 'A' && c <= 'Z') {
            return c - 'A' + 26;
        }
        else {
            if (c == ' ') {
                return 52;
            }
            else {
                return LETTER_IN_ALPHABET;  // Not a letter or a space
            }
        }
    }
}

bool isRegionEmpty(struct Region* regions) {
    return regions == NULL;
}

struct Region* findRegion(struct Region* regions, char* regionName){
    if (isRegionEmpty(regions)) {
        return NULL;        // Not found
    }

    if (regionName[0] == '\0') {
        // End of word
        if (regions->isRegion) {
            return regions;    // Found
        }
        else {
            return NULL;    // Not found
        }
    }
    else {
        unsigned int index = charToTrieIndex(regionName[0]);
        if (index == LETTER_IN_ALPHABET) {
            return findRegion(regions, regionName +1);
        }
        return findRegion(regions->regions[index], regionName+1);
    }
}

void deleteRegions(struct Region** regionsTrie){
    if (!isRegionEmpty(*regionsTrie)) {
        for (unsigned int i=0; i<LETTER_IN_ALPHABET; i++) {
            deleteRegions(&((*regionsTrie)->regions[i]));
        }
        free(*regionsTrie);
        *regionsTrie = NULL;
    }
}

unsigned int getBirths(struct Region* regionsTrie, char* regionName){
    struct Region* region = findRegion(regionsTrie, regionName);
    if(region == NULL) {
        return 0;
    }
    return region->births;
}

bool regionExists(struct Region* regionsTrie, char* regionName) {
    struct Region* region = findRegion(regionsTrie, regionName);
    if (region == NULL) {
        return false;
    }
    return region->isRegion;
}

void addBirth(struct Region* regionsTrie, char* regionName) {
    if (! regionExists(regionsTrie, regionName)) {
        insertRegion(&regionsTrie, regionName);
    }
    struct Region* region = findRegion(regionsTrie, regionName);
    region->births += 1;
}