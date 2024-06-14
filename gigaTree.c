#include "gigaTree.h"
#include <stdlib.h>
#include <stdbool.h>

struct GigaTree* createEmptyGigaTree(){
    struct GigaTree* tree = malloc(sizeof(struct GigaTree));
    if(tree == NULL)
        return NULL;


    for(int i = 0; i < 12; i ++){ //Will go over the array for birthday and edit it to put 0 everywhere
        for(int y = 0; y < 31; y++){
            tree->birthday[i][y] = 0;
        }
    }
}