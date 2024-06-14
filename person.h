

#ifndef PROJET_PERSON_H
#define PROJET_PERSON_H

struct Person{
    unsigned int id;
    char* firstname;
    char* lastname;
    struct Person* padre;
    struct Person* madre;
};





#endif //PROJET_PERSON_H
