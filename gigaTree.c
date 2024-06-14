#include "gigaTree.h"
#include <stdlib.h>
#include <stdbool.h>

struct GigaTree* create_empty_giga_tree(){
    struct GigaTree* tree = malloc(sizeof(struct GigaTree));
    if(tree == NULL)
        return NULL;


    tree->birthday[12][31] = {0};
}