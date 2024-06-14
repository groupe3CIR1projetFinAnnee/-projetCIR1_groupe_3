#ifndef PROJET_REGIONS_H
#define PROJET_REGIONS_H
#define LETTER_IN_ALPHABET 27
#include <stdbool.h>

struct Region{
    struct Region* regions[LETTER_IN_ALPHABET];
    bool isRegion;
    unsigned int births;
};



#endif //PROJET_REGIONS_H
