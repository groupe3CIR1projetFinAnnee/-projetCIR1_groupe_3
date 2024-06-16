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


/**
 * Create a new GigaTree, and allocates memory. Leaves all attributes empty.
 * Attributes should be set up with the csvReader.
 * @return The created GigaTree
 */
struct GigaTree* createEmptyGigaTree();

// TODO : code en commentaire qui traine.
//void setupGigaTree(struct GigaTree* gigaTree,struct Person** people, struct Region* regionsTrie); //Will edit the tree using the array of people. Will set all anniversary, youngest/oldest, the regions, etc...


// Access & requests

/**
 * Get the youngest person the GigaTree contains. O(1)
 * @param gigaTree The GigaTree to search in.
 * @return The youngest person of gigaTree
 */
struct Person* getYoungest(struct GigaTree* gigaTree);

/**
 * Get the oldest person the GigaTree contains. O(1)
 * @param gigaTree The GigaTree to search in.
 * @return The oldest person of gigaTree
 */
struct Person* getOldest(struct GigaTree* gigaTree);

/**
 * Get the number of births in the given specific region of a GigaTree. O(|region|)
 * @param gigaTree The GigaTree to search in.
 * @param region Will return number of births in this region.
 * @return Returns the number of births in the given region.
 */
unsigned int getNumberOfBirths(struct GigaTree* gigaTree, char* region);

/**
 * Get the region in which the most people were born, O(1)
 * @param gigaTree The GigaTree to search in.
 * @return Return the name of the region in which the most people were born.
 */
char* mostBirthsRegion(struct GigaTree* gigaTree);

/**
 * Get the number of people having a specific birthday. O(1)
 * @param gigaTree The GigaTree to search in.
 * @param month Birthday month 
 * @param day Birthday day
 * @return Returns the number of people born the given day.
 */
unsigned int births(struct GigaTree* gigaTree, unsigned int month, unsigned int day);

/**
 * Get a person using the given index. Can be used to loop within all the persons.
 * @param gigaTree The GigaTree to search in.
 * @param index Requested index.
 * @return The person at the given index
 */
struct Person* getPersonByIndex(struct GigaTree* gigaTree, unsigned int index);

/**
 * Get the total number of person within the given GigaTree, including the "unknown" person.
 * @param gigaTree The GigaTree to search in.
 * @return Number of person in gigaTree. /!\ Includes the "unknown" person.
 */
unsigned int numberPersons(struct GigaTree* gigaTree);

/**
 * Get the array containing people within the given GigaTree.
 * /!\ The array is not copied. Any changes to this array should be done knowing the way GigaTree works.
 * @param gigaTree The GigaTree containing requested people.
 * @return Array of Person
 */
struct Person** getPeople(struct GigaTree* gigaTree);

/**
 * Delete the given GigaTree
 * @param gigaTree The GigaTree to delete
 */
void deleteGigaTree(struct GigaTree** gigaTree);


#endif //PROJET_GIGATREE_H
