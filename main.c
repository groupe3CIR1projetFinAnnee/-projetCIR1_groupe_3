#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gigaTree.h"
#include "regions.h"
#include "person.h"
#include "csvReader.h"


char* path = "../db_temp/mini_db.csv";

int main(){
    /*
    char* firstname = "Jean";
    char* lastname = "Michel";
    char* birthday = "1/11/2000";
    char* region = "Alabama";
    unsigned int id = 1;
    struct Person* p = createPerson(id,firstname,lastname,birthday,0,0,region);
    printf("%s %s %d %d %d %d %s\n",p->firstname, p->lastname,p->id, p->birthDay, p->birthMonth, p->birthYear,p->region);
    //unsigned int* truc = getBirthday(p);
    */

    struct GigaTree* gt = readCSV(path);
/*
    printf("Il y a %d entrée, en comptant la personne nulle.\n",gt->numberPersons);
    printf("The youngest is : %s\n", getFirstName(getYoungest(gt)));
    printf("His/her father is : %s\nHis sex must be 1 : %d\n", getFirstName(getPadre(getYoungest(gt))), getSex(getPadre(getYoungest(gt))));
    printf("The mother father is : %s\nHis sex must be 1 if not unknown : %d\n", getFirstName(getPadre(getMadre(getYoungest(gt)))), getSex(getPadre(getMadre(getYoungest(gt)))));
    printf("The oldest is : %s\n", getFirstName(getOldest(gt)));
    printf("His/her father is : %s\nHis sex may be 0 if unknown : %d\n", getFirstName(getPadre(getOldest(gt))), getSex(getPadre(getOldest(gt))));

*/
    printf("%s\n",gt->oldest->region);
    printf("The youngest is born in %s", getRegion(getYoungest(gt)));




    //deletePerson(&p);
    deleteGigaTree(&gt);
    return 0;
}