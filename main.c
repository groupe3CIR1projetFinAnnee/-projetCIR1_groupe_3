#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gigaTree.h"
#include "regions.h"
#include "person.h"
#include "csvReader.h"
#include "exportLocal.h"
#define MAX_FILENAME_LENGTH 255

char* path = "../db_temp/mini_db.csv";

void csvMenu();
void csvNameMenu();
void mainMenu(struct GigaTree* gigatree);
void queryMenu(struct GigaTree* gigatree);
void printPeopleRegion(struct GigaTree* gigatree);
void printPeopleOnBirthday(struct GigaTree* gigatree);


int main(){
    csvMenu();
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
        int error;
        switch (c) {
            case '1':
                printf("Global informations on the tree:\n");
                printf("__________________________\n");
                printf("%d people (base people, unknown, count here)\n", numberPersons(gigatree));
                printf("%d male, %d female & %d unknown\n", numberMale(gigatree), numberFemale(gigatree), numberPersons(gigatree)-numberFemale(gigatree)-numberMale(gigatree));
                printf("%d inbreed people. Wonderful, isn't it ?", inbreeding(gigatree));
                // TODO
                break;

            case '2':
                error = exportLocalSite(gigatree);
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
            printf("6- Exit\n");
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
                if (gigatree->mostBirthsRegion == NULL) {
                    printf("Error happened while fetching for region containing most births.\n");
                }
                else {
                    printf("The following region has the most births:\n");
                    printf("%s\n", mostBirthsRegion(gigatree));
                }
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

// Print the number of people born in a given region
void printPeopleRegion(struct GigaTree* gigatree) {
    char regionName[MAX_REGION_LENGTH];
    printf("\nGet number of people born in a region\n");
    printf("__________________________\n");
    printf("Enter region name: ");
    fgets(regionName, MAX_REGION_LENGTH, stdin);
    regionName[strlen(regionName)-1] = '\0';    // Delete end-line character
    if (regionExists(getRegionTrie(gigatree), regionName)) {
        printf("%d people were born in %s.\n", getBirths(getRegionTrie(gigatree), regionName), regionName);
    }
    else {
        printf("Unknown region.\n");
    }
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

