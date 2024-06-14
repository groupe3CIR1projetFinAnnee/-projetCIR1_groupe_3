#include <stdio.h>
#include "gigaTree.h"
#include "regions.h"
#include "person.h"
#include "csvReader.h"


char* path = "../resources/bd_petite.csv";

int main(){
    char* firstname = "Jean";
    char* lastname = "Michel";
    char* birthday = "1/22/2000";
    char* region = "Alabama";
    unsigned int id = 1;
    //struct Person* p = createPerson(id,firstname,lastname,birthday,0,0,region);
    //printf("%s %s %d %d %d %d %s",p->firstname, p->lastname,p->id, p->birthDay, p->birthMonth, p->birthYear,p->region);
    readCSV(path);
    //struct Person* p = createEmptyPerson();
    return 0;

}
