

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
struct Person* createEmptyPerson(); //Return the "unknown person" with id of 0

struct Person* createPerson(unsigned int id, char* firstname, char* lastname, char* birthday, unsigned int padreID, unsigned int madreID, char* region); //Will initiate a person, but not his/her parents.

// Suppress
void deletePerson(struct Person* person);



#endif //PROJET_PERSON_H
