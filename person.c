#include "person.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct Person* createEmptyPerson(){
    struct Person* person = malloc(sizeof(struct Person));
    if(person == NULL)
        return NULL;

    person->firstname  = malloc(sizeof(char) *(1+1));
    person->lastname = malloc(sizeof(char) *(1+1));

    if(person->firstname == NULL || person->lastname == NULL){
        free(person->firstname);
        free(person->lastname);
        free(person);
        return NULL;
    }

    char* name = "-";
    strcpy(person->firstname,name); //We put the default name for unknown
    strcpy(person->lastname,name);

    person->madre = NULL;
    person->padre = NULL; //The unknown person have no parents
    person->birthDay = 0;
    person->birthMonth = 0;
    person->birthYear = 0;
    return person;

}


struct Person* createPerson(unsigned int id, char* firstname, char* lastname, char* birthday){
    struct Person* person = malloc(sizeof(struct Person));
    if(person == NULL)
        return NULL;

    person->firstname = malloc((strlen(firstname)+1) * sizeof(char)); //We take the space to set up names
    person->lastname = malloc((strlen(lastname)+1) * sizeof(char));

    if(person->firstname == NULL || person->lastname == NULL){ //If one on the two is null we free all
        free(person->firstname);
        free(person->lastname);
        free(person);
        return NULL;
    }

    strcpy(person->firstname,firstname); //Copy the name and lastname
    strcpy(person->lastname,lastname);
    person->madre = NULL;
    person->padre = NULL; //we set parents here with null. The function calling this function must put the parent itself
    unsigned int day, month, year;

    char* birthday_copy = strdup(birthday); // We copy the birthday here to be able to edit it with strtok
    char* token = strtok(birthday_copy, "/");
    //1/12/2025
    unsigned int comp = 1;
    while (token != NULL) {
        switch(comp){
            case 1:
                day = atoi(token);
            case 2:
                month = atoi(token);
            case 3:
                year = atoi(token);
        }

        token = strtok(NULL, "/");
        comp++;
    }

    free(birthday_copy);


    person->birthDay = day;
    person->birthMonth = month;
    person->birthYear = year;
    return person;
}
