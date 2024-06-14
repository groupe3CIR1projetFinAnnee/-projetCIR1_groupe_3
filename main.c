#include <stdio.h>
#include "gigaTree.h"
#include "regions.h"
#include "person.h"
#include "csvReader.h"


char* path = "../resources/bd_petite.csv";

int main(){
    readCSV(path);
    //struct Person* p = createEmptyPerson();
    return 0;

}
