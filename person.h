

#ifndef PROJET_PERSON_H
#define PROJET_PERSON_H

#include <stdbool.h>

struct Person{
    unsigned int id;
    char* firstname;
    char* lastname;
    unsigned int birthDay;
    unsigned int birthMonth;
    unsigned int birthYear;
    struct Person* padre;
    struct Person* madre;
    unsigned int padreID;
    unsigned int madreID;
    char* region;
    int sex; //0 if female and 1 if male. Will be initied when another function will calculate the parents
};

#define MALE 1
#define FEMALE 0
#define UNKNOWN (-1)

// Creation & setup
// TODO: delete useless commented code
//struct Person* createEmptyPerson(); //Return the "unknown person" with id of 0 //Never used, and useless

/**
 * Create a new person, using the given parameters.
 * Parents must be linked outside of this function when every people is created.
 * @param id ID of this person
 * @param firstname First name of this person
 * @param lastname Last name of this person
 * @param birthday Birthday of this person in a string. Format: DD/MM/YYYY
 * @param padreID ID of this person's father
 * @param madreID ID of this person's mother
 * @param region Birth region of this person
 * @return The created person
 */
struct Person* createPerson(unsigned int id, char* firstname, char* lastname, char* birthday, unsigned int padreID, unsigned int madreID, char* region);


//Access

/**
 * Get the ID of the given person
 * @param person A person
 * @return ID of the given person
 */
unsigned int getID(struct Person* person);

/**
 * Get the first name of the given person
 * @param person A person
 * @return First name of the given person
 */
char* getFirstName(struct Person* person);

/**
 * Get the last name of the given person
 * @param person A person
 * @return Last name of the given person
 */
char* getLastName(struct Person* person);

// TODO: we can simplify return type from unsigned int* to unsigned int[3]
/**
 * Get the birth date of the given person.
 * @param person A person
 * @return Birth date of the given person in an array. Miust be freed. Format: [DD,MM,YYYY]
 */
unsigned int* getBirthday(struct Person* person);

/**
 * Get the father of the given person
 * @param person A person
 * @return Father of the given person
 */
struct Person* getPadre(struct Person* person);

/**
 * Get the mother of the given person
 * @param person A person
 * @return Mother of the given person
 */
struct Person* getMadre(struct Person* person);

/**
 * Set the father of the given person.
 * /!\ Also changes the known sex of the father
 * @param person A person
 * @param padre Father of the person
 */
void setPadre(struct Person* person, struct Person* padre);

/**
 * Set the mother of the given person
 * /!\ Also changes the known sex of the mother
 * @param person A person
 * @param madre Mother of the person
 */
void setMadre(struct Person* person, struct Person* madre);

/**
 * Get ID of the father
 * @param person A person
 * @return ID of the father of person
 */
unsigned int getPadreID(struct Person* person);

/**
 * Get ID of the mother
 * @param person A person
 * @return ID of the mother of person
 */
unsigned int getMadreID(struct Person* person);

/**
 * Get birth region of the given person
 * @param person A person
 * @return Birth region of the given person
 */
char* getRegion(struct Person* person);

// TODO change return type bool to an enum, or use MALE/FEMALE (int)
/**
 * Get gender of the given person
 * Use MALE and FEMALE constants to compare the gender
 * @param person A person
 * @return gender of the given person
 */
int getSex(struct Person* person);

/**
 * Get oldest ancestor of the given person
 * @param person A person
 * @return the oldest ancestor of the given person
 */
struct Person* getOldestParent(struct Person* person);

/**
 * Get oldest person between two people
 * @param person1 A person
 * @param person2 Another person
 * @return Return the oldest person between the two given
 */
struct Person* compareOldest(struct Person* person1, struct Person* person2);

// TODO change bool to int, or use an enum
/**
 * Set gender of the given person
 * @param person A person
 * @param sex New gender of the person. Use MALE and FEMALE
 */
void setSex(struct Person* person, int sex);


// Deletion
/**
 * Delete the given person
 * @param person The person to delete
 */
void deletePerson(struct Person** person);


// Others

// TODO: use return type unsigned int[3] instead. Or even use 3 fonctions: getBirthDay, getBirthMonth, getBirthYear
/**
 * Split the given date string into a formatted array.
 * @param birthday Birth date. Format: "DD/MM/YYYY"
 * @return A array representing the birth date. Format: [day, month, year]
 */
unsigned int* splitBirthday(char* birthday);


#endif //PROJET_PERSON_H
