

#ifndef PROJET_PERSON_H
#define PROJET_PERSON_H

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
};


// Creation & setup
//struct Person* createEmptyPerson(); //Return the "unknown person" with id of 0 //Never used, and useless

struct Person* createPerson(unsigned int id, char* firstname, char* lastname, char* birthday, unsigned int padreID, unsigned int madreID, char* region); //Will initiate a person, but not his/her parents.

//Access
unsigned int getID(struct Person* person);

char* getFirstName(struct Person* person);
char* getLastName(struct Person* person);

unsigned int* getBirthday(struct Person* person); //Will return the birthday in an array, like this : [JJ,MM,AAAA]

struct Person* getPadre(struct Person* person);
struct Person* getMadre(struct Person* person);

unsigned int getPadreID(struct Person* person);
unsigned int getMadreID(struct Person* person);

char* getRegion(struct Person* person);

// Suppress
void deletePerson(struct Person** person);



#endif //PROJET_PERSON_H
