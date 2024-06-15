

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
    bool sex; //0 if female and 1 if male. Will be initied when another function will calculate the parents
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

void setPadre(struct Person* person, struct Person* padre); //set the padre for a person. WARNING : WILL ALSO EDIT THE SEX OF THE DAD TO PUT A 1
void setMadre(struct Person* person, struct Person* madre); //set the madre for a person. WARNING : WILL ALSO EDIT THE SEX OF THE MOM TO PUT A 0


unsigned int getPadreID(struct Person* person);
unsigned int getMadreID(struct Person* person);

char* getRegion(struct Person* person);

bool getSex(struct Person* person);
void setSex(struct Person* person, bool sex); //Reminder : 0 = female and 1 = male


// Suppress
void deletePerson(struct Person** person);


// OThers
unsigned int* splitBirthday(char* birthday); //Transform the char birthday into an array of 3 elements. Dont forget to free it after use.


#endif //PROJET_PERSON_H
