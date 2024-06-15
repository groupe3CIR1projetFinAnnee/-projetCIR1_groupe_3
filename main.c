#include <stdio.h>
#include <stdlib.h>
#include "gigaTree.h"
#include "regions.h"
#include "person.h"
#include "csvReader.h"


char* path = "../db_temp/bd_grande.csv";

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
    printf("%d\n",gt->numberPersons);
    printf("The youngest is : %s\n", getFirstName(getYoungest(gt)));
    printf("His/her mother is : %s\nHer sex must be 0 : %d\n", getFirstName(getMadre(getYoungest(gt))), getSex(getMadre(gt->youngest)));
    printf("The mother father is : %s\nHir sex must be 1 if not unknown : %d\n", getFirstName(getPadre(getMadre(getYoungest(gt)))), getSex(getMadre(gt->youngest)));
    printf("The oldest is : %s\n", getFirstName(getOldest(gt)));
    printf("His/her father is : %s\nHis sex may be 0 if unknown : %d\n", getFirstName(getPadre(getOldest(gt))), getSex(getPadre(gt->oldest)));


    //deletePerson(&p);
    deleteGigaTree(&gt);
    return 0;

}

