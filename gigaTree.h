#include <stdbool.h>

#define LETTER_IN_ALPHABET 27

#ifndef PROJET_GIGATREE_H
#define PROJET_GIGATREE_H


struct GigaTree{
    struct Person** person;
    struct Region* regionsTrie;
    unsigned int birthday[12][31];
    struct Person* youngest;
    struct Person* oldest;
    char* mostBirthsRegion;
    unsigned int mostBirths;
    unsigned int numberPersons;
};

struct Region{
    struct Region* regions[LETTER_IN_ALPHABET];
    bool isRegion;
    unsigned int births;
};


struct Person{
    unsigned int id;
    char* firstname;
    char* lastname;
    struct Person* padre;
    struct Person* madre;
};











#endif //PROJET_GIGATREE_H
