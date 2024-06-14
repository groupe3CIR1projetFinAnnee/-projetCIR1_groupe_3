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







#endif //PROJET_GIGATREE_H
