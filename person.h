

#ifndef PROJET_PERSON_H
#define PROJET_PERSON_H

struct Person{
    unsigned int id;
    char* firstname;
    char* lastname;
    struct Person* padre;
    struct Person* madre;
};


// Creation & setup
struct Person* createEmptyPerson(); //Return the "unknown person" with id of 0

struct Person* createPerson(unsigned int id, char* firstname, char* lastname, unsigned int dadID, unsigned int momID); //Will send recursive create perosn until it found an ID of 0

// Suppress
void deletePerson(struct Person* person);



#endif //PROJET_PERSON_H
