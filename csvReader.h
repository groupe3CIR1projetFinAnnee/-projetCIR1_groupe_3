#ifndef PROJET_CSVREADER_H
#define PROJET_CSVREADER_H

#include "person.h"
#include <stdbool.h>

struct GigaTree* readCSV(char* filePath); // Will read the csv and launch request on the struct. Will return the gigaTree struct, done and full of the infos

bool isYoungest(struct Person* person,char* birthday);


#endif //PROJET_CSVREADER_H
