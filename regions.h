#ifndef PROJET_REGIONS_H
#define PROJET_REGIONS_H
#define LETTER_IN_ALPHABET 27
#include <stdbool.h>
#include <stdio.h>

struct Region{
    struct Region* regions[LETTER_IN_ALPHABET];
    bool isRegion;
    unsigned int births;
};

// Creation & setup
struct Region* createEmptyRegions();
struct Region* createRegions();

// Access
void insertRegion(struct Region** trie, char* regionName);
bool isRegionsEmpty(struct Region* trie);
unsigned int getBirths(struct Region* trie, char* regionName);
struct Region* getChild(struct Region* trie, char letter);
unsigned int numberOfWords(struct Region* trie);

//Suppress
void deleteNodeTrie(struct Region** trie);




#endif //PROJET_REGIONS_H
