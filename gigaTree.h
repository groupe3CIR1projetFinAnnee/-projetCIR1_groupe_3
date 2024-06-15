#ifndef PROJET_GIGATREE_H
#define PROJET_GIGATREE_H


struct GigaTree{
    struct Person** people; //Array of struct Person of every persons in the csv file. Will be init. later
    struct Region* regionsTrie; //Trie of regions. Will be init later
    unsigned int birthday[12][31]; //Array of 12/31 for birthdays
    struct Person* youngest;
    struct Person* oldest;
    char* mostBirthsRegion; //name of the regions with the most birthdays
    unsigned int mostBirths; //may be removed since we can just search in the trie // Is the number of birth in the previous region
    unsigned int numberPersons; //Total number of ppl in the csv
};


//Creation & setup
struct GigaTree* createEmptyGigaTree(); //Will only make the malloc for the tree, and leave the parameters empty

//void setupGigaTree(struct GigaTree* gigaTree,struct Person** people, struct Region* regionsTrie); //Will edit the tree using the array of people. Will set all anniversary, youngest/oldest, the regions, etc...


// Access & requests
struct Person* getYoungest(struct GigaTree* gigaTree);//return struct of youngest person

struct Person* getOldest(struct GigaTree* gigaTree); //return struct of oldest person

unsigned int getNumberOfBirths(struct GigaTree* gigaTree, char* region); //Get number of birth of a region using the trie

char* mostBirthsRegion(struct GigaTree* gigaTree); //return the name of the region with the most births, save in parameters of gigaTree

unsigned int births(struct GigaTree* gigaTree, unsigned int month, unsigned int day); //Get the number of ppl born one day. Will use the array in parameters

struct Person* getPersonByIndex(struct GigaTree* gigaTree, unsigned int index); //Will get the person by the index in the array of person

unsigned int numberPersons(struct GigaTree* gigaTree); //return the total number of person PLUS ONE !! we counting the "unknow" person. If the db contain 40 person, remember that the struc will have 41 person saved

struct Person** getPeople(struct GigaTree* gigaTree); //Return the full array

//Suppress
void deleteGigaTree(struct GigaTree** gigaTree);


#endif //PROJET_GIGATREE_H
