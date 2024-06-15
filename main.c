<<<<<<< Updated upstream
#include <stdio.h>
#include <stdlib.h>
#include "gigaTree.h"
#include "regions.h"
#include "person.h"
#include "csvReader.h"
#include "exportLocal.h"


char* path = "../db_temp/bd_petite.csv";

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


    // struct GigaTree* gt = readCSV(path);
    struct GigaTree* gt = createEmptyGigaTree();
    
    // printf("%s\n", getFirstName(gt->youngest));
    // printf("%s\n", getFirstName(gt->oldest));
    //printf("%d",(getBirthday((gt->people)[40]))[0]);
    //printf("%b", isYoungest(p,"2/10/2000"));

    
    //deletePerson(&p);

    exportLocalSite(gt);

    // deleteGigaTree(&gt);
    return 0;

}
=======
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
    printf("%s\n", getFirstName(gt->youngest));
    printf("%s\n", getFirstName(gt->oldest));
    //printf("%d",(getBirthday((gt->people)[40]))[0]);
    //printf("%b", isYoungest(p,"2/10/2000"));



    //deletePerson(&p);
    deleteGigaTree(&gt);
    return 0;

}
>>>>>>> Stashed changes
