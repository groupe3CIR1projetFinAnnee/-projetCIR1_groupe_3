#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gigaTree.h"
#include "regions.h"
#include "person.h"
#include "csvReader.h"


char* path = "../db_temp/mini_db.csv";

int main(){
    /*
    char* firstname = "Jean";
    char* lastname = "Michel";
    char* birthday = "1/11/2000";
    char* region = "Alabama";
    unsigned int id = 1;
    struct Person* p = createPerson(id,firstname,lastname,birthday,0,0,region);
    printf("%s %s %d %d %d %d %s\n",p->firstname, p->lastname,p->id, p->birthDay, p->birthMonth, p->birthYear,p->region);
    //unsigned int* truc = getBirthday(p);
    */

    struct GigaTree* gt = readCSV(path);
/*
    printf("Il y a %d entrée, en comptant la personne nulle.\n",gt->numberPersons);
    printf("The youngest is : %s\n", getFirstName(getYoungest(gt)));
    printf("His/her father is : %s\nHis sex must be 1 : %d\n", getFirstName(getPadre(getYoungest(gt))), getSex(getPadre(getYoungest(gt))));
    printf("The mother father is : %s\nHis sex must be 1 if not unknown : %d\n", getFirstName(getPadre(getMadre(getYoungest(gt)))), getSex(getPadre(getMadre(getYoungest(gt)))));
    printf("The oldest is : %s\n", getFirstName(getOldest(gt)));
    printf("His/her father is : %s\nHis sex may be 0 if unknown : %d\n", getFirstName(getPadre(getOldest(gt))), getSex(getPadre(getOldest(gt))));

*/
    printf("%s\n",gt->oldest->region);
    printf("The youngest is born in %s", getRegion(getYoungest(gt)));




    //deletePerson(&p);
    // deleteGigaTree(&gt);
    return 0;
}

void csvMenu() {
    bool exit = false;
    char skip[200];     // Buffer to skip some characters

    // Ask for choice
    char c;
    while (!exit) {
        c = '1';
        do {
            if (c < '1' || c > '2') {
                printf("Invalid number. Please enter a valid number.\n");
            }
            printf("\n");
            printf("What do you want to do?\n");
            printf("__________________________\n");
            printf("1- Read CSV file\n");
            printf("2- Exit\n");
            printf("__________________________\n");
            printf("Your choice: ");
            c = getc(stdin);
            fgets(skip, sizeof(skip), stdin);   // Skip end-line
        } while (c < '1' || c > '2');

        switch (c) {
            case '1':
                csvNameMenu();
                break;
            
            default:
                exit = true;
                break;
        }
    }
}

void csvNameMenu() {
    char csvFilename[MAX_FILENAME_LENGTH];
    printf("\n");
    printf("Read CSV file\n");
    printf("__________________________\n");
    printf("Enter CSV file name: ");
    fgets(csvFilename, MAX_FILENAME_LENGTH, stdin);

    unsigned int path_len = strlen(csvFilename);
    csvFilename[path_len-1] = '\0';       // Remove end-line character
    path_len -= 2;
    
    struct GigaTree* gigatree = readCSV(csvFilename);

    if (gigatree == NULL) {
        printf("Invalid file name.\n");
    }
    else {
        printf("File loaded!\n");
        mainMenu(gigatree);
    }
}

void mainMenu(struct GigaTree* gigatree) {
    bool exit = false;
    char skip[200];     // Buffer to skip some characters

    // Ask for choice
    char c;
    while (!exit) {
        c = '1';
        do {
            if (c < '1' || c > '4') {
                printf("Invalid number. Please enter a valid number.\n");
            }
            printf("\n");
            printf("What do you want to do?\n");
            printf("__________________________\n");
            printf("1- Show global informations\n");
            printf("2- Export to local site\n");
            printf("3- Query informations\n");
            printf("4- Exit\n");
            printf("__________________________\n");
            printf("Your choice: ");
            c = getc(stdin);
            fgets(skip, sizeof(skip), stdin);   // Skip end-line
        } while (c < '1' || c > '4');

        switch (c) {
            case '1':
                printf("Global informations on the tree:\n");
                printf("__________________________\n");
                printf("%d people\n", numberPersons(gigatree));
                // TODO
                break;

            case '2':
                int error = exportLocalSite(gigatree);
                if (error == 0) {
                    printf("Local site exported successfully!\n");
                }
                else {
                    printf("Failed to export local site.\n");
                }
                break;
            
            case '3':
                queryMenu(gigatree);
                break;
            
            default:
                exit = true;
                break;
        }
    }
    deleteGigaTree(&gigatree);
}

void queryMenu(struct GigaTree* gigatree) {
    bool exit = false;
    char skip[200];     // Buffer to skip some characters

    // Ask for choice
    char c;
    while (!exit) {
        c = '1';
        do {
            if (c < '1' || c > '6') {
                printf("Invalid number. Please enter a valid number.\n");
            }
            printf("\n");
            printf("What do you want to do?\n");
            printf("__________________________\n");
            printf("1- Get first born in the tree\n");
            printf("2- Get last born in the tree\n");
            printf("3- Get number of people born in a region\n");
            printf("4- Get the region with highest number of births\n");
            printf("5- Get the number of people born on a given day\n");
            printf("6- Exit");
            printf("__________________________\n");
            printf("Your choice: ");
            c = getc(stdin);
            fgets(skip, sizeof(skip), stdin);   // Skip end-line
        } while (c < '1' || c > '6');

        struct Person* person;
        switch (c) {
            case '1':
                person = getOldest(gigatree);
                printf("The first person born in the tree is %s %s\n", getFirstName(person), getLastName(person));
                break;

            case '2':
                person = getYoungest(gigatree);
                printf("The last person born in the tree is %s %s\n", getFirstName(person), getLastName(person));
                break;
            
            case '3':
                printPeopleRegion(gigatree);
                break;

            case '4':
                printf("The region %s has the highest number of births.\n", mostBirthsRegion(gigatree));
                break;

            case '5':
                printPeopleOnBirthday(gigatree);
                break;
            
            default:
                exit = true;
                break;
        }
    }
}

// Print the number of people in a given region
void printPeopleRegion(struct GigaTree* gigatree) {
    printf("\nGet number of people born in a region\n");
    printf("__________________________\n");
    printf("Enter region name: ");
    // fgets(, MAX_FILENAME_LENGTH, stdin);
    // TODO
}

// Print the number of people born on the given day
void printPeopleOnBirthday(struct GigaTree* gigatree) {
    char buffer[200];
    printf("\nGet number of people born on a given day\n");
    printf("__________________________\n");
    printf("Enter birth day (Use format DD/MM): ");

    fgets(buffer, 200, stdin);
    
    unsigned int buffer_len = strlen(buffer);
    if (buffer_len >= 5 && buffer[2] == '/') {  // If correct format
        char str_day[3] = {buffer[0], buffer[1], '\0'};
        char str_month[3] = {buffer[3], buffer[4], '\0'};
        int day = atoi(str_day);
        int month = atoi(str_month);
        if (day >= 1 && day <= 31 && month >= 1 && month <= 12) {
            printf("%d people were born on %s/%s\n", births(gigatree, month, day), str_day, str_month);
        }
        else {
            printf("Invalid format.\n");
        }
    }
    else {
        printf("Invalid format.\n");
    }
}