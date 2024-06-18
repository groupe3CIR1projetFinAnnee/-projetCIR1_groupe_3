#ifndef PROJET_GIGATREE_H
#define PROJET_GIGATREE_H


struct GigaTree{
    struct Person** people;     //Array of struct Person of every persons in the csv file. Empty person is at position 0.
    struct Region* regionsTrie; //Trie of regions. Will be init later
    unsigned int birthday[12][31]; //Array of 12/31 for birthdays
    struct Person* youngest;
    struct Person* oldest;
    char* mostBirthsRegion; //name of the regions with the most birthdays
    unsigned int mostBirths; //may be removed since we can just search in the trie // Is the number of birth in the previous region
    unsigned int numberPersons; //Total number of ppl in the csv
    unsigned int numFamilies;
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
 * Set the youngest person the GigaTree will contains.
 * @param gigaTree The GigaTree to put in.
 * @param youngest From the struct Person* the new youngest
 */
void setYoungest(struct GigaTree* gigaTree,struct Person* youngest);


/**
 * Get the oldest person the GigaTree contains. O(1)
 * @param gigaTree The GigaTree to search in.
 * @return The oldest person of gigaTree
 */
struct Person* getOldest(struct GigaTree* gigaTree);

/**
 * Set the oldest person the GigaTree will contains.
 * @param gigaTree The GigaTree to put in.
 * @param oldest From the struct Person* the new oldest
 */
void setOldest(struct GigaTree* gigaTree,struct Person* oldest);


/**
 * Get the region in which the most people were born, O(1)
 * @param gigaTree The GigaTree to search in.
 * @return Return the name of the region in which the most people were born.
 */
char* mostBirthsRegion(struct GigaTree* gigaTree);

/**
 * Set the region for you. will NOT make the malloc
 * @param gigaTree The GigaTree to get from
 * @param region Name of the region
 */
void setMostBirthsRegion(struct GigaTree* gigaTree, char* region);

/**
 * Get the number of people having a specific birthday. O(1)
 * @param gigaTree The GigaTree to search in.
 * @param month Birthday month 
 * @param day Birthday day
 * @return Returns the number of people born the given day.
 */
unsigned int births(struct GigaTree* gigaTree, unsigned int month, unsigned int day);

/**
 * Set the most number of births in a region. Can be used to initiate the tree
 * @param gigaTree The GigaTree to get from
 * @param births The number of births
 */
void setMostBirths(struct GigaTree* gigaTree, unsigned int births);

/**
 * Free the most birth region
 * @param gigaTree The GigaTree to get from
 */
void freeMostBirthRegion(struct GigaTree* gigaTree);

/**
 * Get the most number of births in a region. Can be used to initiate the tree
 * @param gigaTree The GigaTree to get from
 * @return The number of births as an u.int
 */
unsigned int mostBirths(struct GigaTree* gigaTree);



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
 * Get GigaTree regions trie
 * @param gigaTree A GigaTree
 * @return retions trie of the GigaTrie
 */
struct Region* getRegionTrie(struct GigaTree* gigaTree);

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

unsigned int numberMale(struct GigaTree* gigaTree);
unsigned int numberFemale(struct GigaTree* gigaTree);

unsigned int inbreeding(struct GigaTree* gigaTree);

/**
 * Get number of dinstinct families in the given GigaTree
 * @param gigaTree A GigaTree
 * @return Number of dinstinct families in the given GigaTree
 */
unsigned int numberFamilies(struct GigaTree* gigaTree);

/**
 * Calculate number of dinstinct families in the given GigaTree.
 * Used to initialize gigaTree->numFamilies
 * @param gigaTree A GigaTree
 * @return Number of dinstinct families in the given GigaTree
 */
unsigned int calculateNumberFamilies(struct GigaTree* gigaTree);



void deleteGigaTree(struct GigaTree** gigaTree);


#endif //PROJET_GIGATREE_H
