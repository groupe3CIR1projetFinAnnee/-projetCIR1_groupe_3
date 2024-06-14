#include "person.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct Person* createEmptyPerson(){
    struct Person* person = malloc(sizeof(struct Person));
    if(person == NULL)
        return NULL;

    char* firstname = malloc(sizeof(char) *(1+1));
    char* lastname = malloc(sizeof(char) *(1+1));
    firstname = "-"; //The unknow person have this name and lastname
    lastname = "-";
    person->firstname = firstname;
    person->lastname = lastname;
    person->madre = NULL;
    person->padre = NULL; //The unknow person have no parents
    person->birthDay = 0;
    person->birthMonth = 0;
    person->birthYear = 0;
    return person;

}


struct Person* createPerson(unsigned int id, char* firstname, char* lastname, char* birthday){
    struct Person* person = malloc(sizeof(struct Person));
    if(person == NULL)
        return NULL;

    char* firstnameToInsert = malloc((strlen(firstname)+1) * sizeof(char));
    char* lastnameToInsert = malloc((strlen(lastname)+1) * sizeof(char));
    firstname = "-"; //The unknow person have this name and lastname
    lastname = "-";
    person->firstname = firstname;
    person->lastname = lastname;
    person->madre = NULL;
    person->padre = NULL; //The unknow person have no parents
    person->birthDay = 0;
    person->birthMonth = 0;
    person->birthYear = 0;
    return person;
}
