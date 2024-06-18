#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gigaTree.h"
#include "regions.h"
#include "person.h"
#include "csvReader.h"
#include "exportLocal.h"
#include <time.h>
#define MAX_FILENAME_LENGTH 255

char* path = "../db_temp/mini_db.csv";

void csvMenu();
void csvNameMenu();
void askCalculateFamilies(struct GigaTree* gigaTree);
void mainMenu(struct GigaTree* gigaTree);
void queryMenu(struct GigaTree* gigaTree);
void printPeopleRegion(struct GigaTree* gigaTree);
void printPeopleOnBirthday(struct GigaTree* gigaTree);


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
            printf("\n"
                   " ________  ___  ________  ________  _________  ________  _______   _______      \n"
                   "|\\   ____\\|\\  \\|\\   ____\\|\\   __  \\|\\___   ___\\\\   __  \\|\\  ___ \\ |\\  ___ \\     \n"
                   "\\ \\  \\___|\\ \\  \\ \\  \\___|\\ \\  \\|\\  \\|___ \\  \\_\\ \\  \\|\\  \\ \\   __/|\\ \\   __/|    \n"
                   " \\ \\  \\  __\\ \\  \\ \\  \\  __\\ \\   __  \\   \\ \\  \\ \\ \\   _  _\\ \\  \\_|/_\\ \\  \\_|/__  \n"
                   "  \\ \\  \\|\\  \\ \\  \\ \\  \\|\\  \\ \\  \\ \\  \\   \\ \\  \\ \\ \\  \\\\  \\\\ \\  \\_|\\ \\ \\  \\_|\\ \\ \n"
                   "   \\ \\_______\\ \\__\\ \\_______\\ \\__\\ \\__\\   \\ \\__\\ \\ \\__\\\\ _\\\\ \\_______\\ \\_______\\\n"
                   "    \\|_______|\\|__|\\|_______|\\|__|\\|__|    \\|__|  \\|__|\\|__|\\|_______|\\|_______|\n"
                   "                                                                                \n"
                   "                                                                                \n"
                   "                                                                                \n");
            printf("What do you want to do?\n");
            printf("__________________________\n");
            printf("1- Read CSV file\n");
            printf("2- Exit\n");
            printf("__________________________\n");
            printf("Your choice:");
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

    clock_t t;
    t = clock();
    struct GigaTree* gigaTree = readCSV(csvFilename);
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds


    if (gigaTree == NULL) {
        printf("Invalid file name.\n");
    }
    else {
        printf("File loaded in %.2f seconds!\n",time_taken);
        askCalculateFamilies(gigaTree);
    }
}

void askCalculateFamilies(struct GigaTree* gigaTree) {
    bool exit = false;
    char skip[200];     // Buffer to skip some characters
    printf("\n");

    // Ask for choice
    char c = 'y';
    do {
        if (c != 'y' && c != 'Y' && c != 'n' && c != 'N') {
            printf("Invalid number. Please enter a valid number.\n");
        }
        printf("\n");
        printf("Do you want to calculate number of families? y/n: \n");
        c = getc(stdin);
        fgets(skip, sizeof(skip), stdin);   // Skip end-line
    } while (c != 'y' && c != 'Y' && c != 'n' && c != 'N');

    if (c == 'y' || c == 'Y') {
        calculateNumberFamilies(gigaTree);
    }
    mainMenu(gigaTree);
}

void mainMenu(struct GigaTree* gigaTree) {
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
                printf("%d people (base people, unknown, count here)\n", numberPersons(gigaTree));
                printf("%d male, %d female & %d unknown\n", numberMale(gigaTree), numberFemale(gigaTree), numberPersons(gigaTree)-numberFemale(gigaTree)-numberMale(gigaTree));
                printf("%d dinstinct families\n", numberFamilies(gigaTree)-1);
                printf("%d inbreed people. Wonderful, isn't it ?", inbreeding(gigaTree));
                break;

            case '2':
                error = exportLocalSite(gigaTree);
                if (error == 0) {
                    printf("Local site exported successfully!\n");
                }
                else {
                    printf("Failed to export local site.\n");
                }
                break;

            case '3':
                queryMenu(gigaTree);
                break;

            default:
                exit = true;
                break;
        }
    }
    deleteGigaTree(&gigaTree);
}

void queryMenu(struct GigaTree* gigaTree) {
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
                person = getOldest(gigaTree);
                printf("The first person born in the tree is %s %s\n", getFirstName(person), getLastName(person));
                break;

            case '2':
                person = getYoungest(gigaTree);
                printf("The last person born in the tree is %s %s\n", getFirstName(person), getLastName(person));
                break;

            case '3':
                printPeopleRegion(gigaTree);
                break;

            case '4':
                char* mostBirthReg = mostBirthsRegion(gigaTree);
                if (mostBirthReg == NULL) {
                    printf("Error happened while fetching for region containing most births.\n");
                }
                else {
                    printf("The following region has the most births:\n");
                    printf("%s\n", mostBirthReg);
                }
                break;

            case '5':
                printPeopleOnBirthday(gigaTree);
                break;

            default:
                exit = true;
                break;
        }
    }
}

// Print the number of people born in a given region
void printPeopleRegion(struct GigaTree* gigaTree) {
    char regionName[MAX_REGION_LENGTH];
    printf("\nGet number of people born in a region\n");
    printf("__________________________\n");
    printf("Enter region name: ");
    fgets(regionName, MAX_REGION_LENGTH, stdin);
    regionName[strlen(regionName)-1] = '\0';    // Delete end-line character
    if (regionExists(getRegionTrie(gigaTree), regionName)) {
        printf("%d people were born in %s.\n", getBirths(getRegionTrie(gigaTree), regionName), regionName);
    }
    else {
        printf("Unknown region.\n");
    }
}

// Print the number of people born on the given day
void printPeopleOnBirthday(struct GigaTree* gigaTree) {
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
            printf("%d people were born on %s/%s\n", births(gigaTree, month, day), str_day, str_month);
        }
        else {
            printf("Invalid format.\n");
        }
    }
    else {
        printf("Invalid format.\n");
    }
}

