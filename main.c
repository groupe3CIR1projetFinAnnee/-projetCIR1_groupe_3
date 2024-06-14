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
    unsigned int id = 1;
    //struct Person* p = createPerson(id,firstname,lastname,birthday);
    //printf("%d %d %d",p->birthDay, p->birthMonth, p->birthYear);
    readCSV(path);
    //struct Person* p = createEmptyPerson();
    return 0;

}
