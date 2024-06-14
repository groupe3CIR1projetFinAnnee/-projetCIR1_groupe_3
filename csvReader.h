#ifndef PROJET_CSVREADER_H
#define PROJET_CSVREADER_H

#include "person.h"
#include <stdbool.h>

struct GigaTree* readCSV(char* filePath); // Will read the csv and launch request on the struct. Will return the gigaTree struct, done and full of the infos

bool isYoungest(struct Person* youngest,char* birthday); //Will check if the birthday (char) is less than the actual youngest. If yes, it will return true
bool isOldest(struct Person* youngest,char* birthday);

#endif //PROJET_CSVREADER_H
