#include "regions.h"

#include "regions.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ctype.h"


struct Region* createRegions(){
    struct Region* region = malloc(sizeof(struct Region));
    if(region == NULL)
        return NULL;
    for(unsigned int i = 0; i < LETTER_IN_ALPHABET; i++){
        region->regions[i] = createEmptyRegions(); //We setup the array to nothing
    }
    region->isRegion = false;
    return region;
}

struct Region* createEmptyRegions(){
    return NULL;
}

void insertRegion(struct Region** regions, char* regionName) {
    if (regionName[0] == '\0') { //End of the region name
        (*regions)->isRegion = true;
    }

    else {
        if ((tolower(regionName[0]) >= 'a' && tolower(regionName[0]) <= 'z') || regionName[0] == ' ') {
            // Character is a lowercase letter or a space
            insertRegion(&((*regions)->regions[tolower(regionName[0]) - 'a']), regionName + 1);
        }
        else
            insertRegion(regions,regionName+1); //Skip char
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
        return findRegion(regions->regions[regionName[0]-'a'], regionName+1);
    }
}


void deleteRegions(struct Region** regions){
    if (!isRegionEmpty(*regions)) {
        for (unsigned int i=0; i<LETTER_IN_ALPHABET; i++) {
            deleteRegions(&((*regions)->regions[i]));
        }
        free(*regions);
        *regions = NULL;
    }
}

unsigned int getBirths(struct GigaTree* gigaTree, char* regionName){
    struct Region* region = findRegion(gigaTree->regionsTrie,regionName);
    unsigned int births = 0;
    if(region != NULL)
        births = region->births;
    return births;
}