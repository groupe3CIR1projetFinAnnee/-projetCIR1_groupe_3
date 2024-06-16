#include "person.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
struct Person* createEmptyPerson(){
    struct Person* person = malloc(sizeof(struct Person));
    if(person == NULL)
        return NULL;

    person->firstname  = malloc(sizeof(char) *(1+1));
    person->lastname = malloc(sizeof(char) *(1+1));
    person->region = malloc(sizeof(char) *(1+1));

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
    person->madreID = 0;
    person->padreID = 0;
    return person;

}
*/

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
struct Person* createPerson(unsigned int id, char* firstname, char* lastname, char* birthday, unsigned int padreID, unsigned int madreID, char* region){
    struct Person* person = malloc(sizeof(struct Person));
    if(person == NULL)
        return NULL;
    person->id = id;
    person->firstname = malloc((strlen(firstname)+1) * sizeof(char)); //We take the space to set up names
    person->lastname = malloc((strlen(lastname)+1) * sizeof(char));
    person->region = malloc((strlen(region)+1) * sizeof(char));


    if(person->firstname == NULL || person->lastname == NULL || person->region == NULL){ //If one on the two is null we free all
        free(person->firstname);
        free(person->lastname);
        free(person->region);
        free(person);
        return NULL;
    }

    strcpy(person->firstname,firstname); //Copy the name and lastname
    strcpy(person->lastname,lastname);
    strcpy(person->region,region);
    person->madre = NULL;
    person->padre = NULL; //we set parents here with null. The function calling this function must put the parent itself

    unsigned int* birthdayArray = splitBirthday(birthday);

    person->birthDay = birthdayArray[0];
    person->birthMonth = birthdayArray[1];
    person->birthYear = birthdayArray[2];
    free(birthdayArray); //We dont forget to free this temporary array after use
    person->madreID = madreID;
    person->padreID = padreID;
    return person;
}


/**
 * Get the ID of the given person
 * @param person A person
 * @return ID of the given person
 */
unsigned int getID(struct Person* person){
    return person->id;
}

/**
 * Get the first name of the given person
 * @param person A person
 * @return First name of the given person
 */
char* getFirstName(struct Person* person){
    return person->firstname;
}

/**
 * Get the last name of the given person
 * @param person A person
 * @return Last name of the given person
 */
char* getLastName(struct Person* person){
    return person->lastname;
}


/**
 * Get the birth date of the given person
 * @param person A person
 * @return Birth date of the given person in an array. Format: [DD,MM,YYYY]
 */
unsigned int* getBirthday(struct Person* person){
    unsigned int* birthday = malloc(sizeof(unsigned int) * 3); //3 ints to save
    if(birthday == NULL){
        return NULL;
    }
    birthday[0] = person->birthDay;
    birthday[1] = person->birthMonth;
    birthday[2] = person->birthYear;
    return birthday;
}


/**
 * Get the father of the given person
 * @param person A person
 * @return Father of the given person
 */
struct Person* getPadre(struct Person* person){
    return person->padre;
}

/**
 * Get the mother of the given person
 * @param person A person
 * @return Mother of the given person
 */
struct Person* getMadre(struct Person* person){
    return person->madre;
}

/**
 * Get ID of the father
 * @param person A person
 * @return ID of the father of person
 */
unsigned int getPadreID(struct Person* person){
    return person->padreID;
}

/**
 * Get ID of the mother
 * @param person A person
 * @return ID of the mother of person
 */
unsigned int getMadreID(struct Person* person){
    return person->madreID;
}

/**
 * Set the father of the given person.
 * /!\ Also changes the known sex of the father
 * @param person A person
 * @param padre Father of the person
 */
void setPadre(struct Person* person, struct Person* padre){
    person->padre = padre;
    if(getID(padre) != 0)
        person->padre->sex = MALE;  // We set the dad sex to male only if the padre is not the unknown person
}

/**
 * Set the mother of the given person
 * /!\ Also changes the known sex of the mother
 * @param person A person
 * @param madre Mother of the person
 */
void setMadre(struct Person* person, struct Person* madre){
    person->madre = madre;
    if(getID(madre) != 0)
        person->madre->sex = FEMALE; //We set the mom sex to female only if the madre is not the unknown person
}

/**
 * Get birth region of the given person
 * @param person A person
 * @return Birth region of the given person
 */
char* getRegion(struct Person* person){
    return person->region;
}

/**
 * Get gender of the given person
 * Use MALE and FEMALE constants to compare the gender
 * @param person A person
 * @return gender of the given person
 */
bool getSex(struct Person* person){
    return person->sex;
}

/**
 * Set gender of the given person
 * @param person A person
 * @param sex New gender of the person. Use MALE and FEMALE
 */
void setSex(struct Person* person, bool sex){
    person->sex = sex;
}


/**
 * Delete the given person
 * @param person The person to delete
 */
void deletePerson(struct Person** person){
    free((*person)->firstname);
    free((*person)->lastname);
    free((*person)->region);
    free(*person);
    *person = NULL;
}


/**
 * Split the given date string into a formatted array.
 * @param birthday Birth date. Format: "DD/MM/YYYY"
 * @return A array representing the birth date. Format: [day, month, year]
 */
unsigned int* splitBirthday(char* birthday){

    char* birthday_copy = strdup(birthday); // We copy the birthday here to be able to edit it with strtok
    char* token = strtok(birthday_copy, "/");

    unsigned int comp = 1; //Comp to know were we are
    unsigned int day, month, year;
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
    unsigned int* birthdayArray = malloc(sizeof(unsigned int) * 3); //3 int to save
    if(birthdayArray == NULL){
        return NULL;
    }
    birthdayArray[0] = day;
    birthdayArray[1] = month;
    birthdayArray[2] = year;
    return birthdayArray;
}