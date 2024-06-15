#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <sys/stat.h>
#include <unistd.h>

#include "exportLocal.h"
#include "gigaTree.h"
#include "person.h"

#define DEBUG

#define LASTNAME_TEMPLATE "lastname"
#define FORNAME_TEMPLATE "forname"
#define REGION_TEMPLATE "region"
#define BIRTH_DAY_TEMPLATE "birth_day"
#define BIRTH_MONTH_TEMPLATE "birth_month"
#define BIRTH_YEAR_TEMPLATE "birth_year"
#define FATHER_TEMPLATE "padre"
#define MOTHER_TEMPLATE "madre"

// There is a max of info length because info is a predifined string
#define MAX_INFO_LEN 64   // Info is the content of a template
#define NUMBER_FILES 9

int exportLocalSite(struct GigaTree* gigaTree) {
    // Arrays of sources/dest files (except template page of a person)
    // TODO : path is different on windows / on linux
    char* sources[NUMBER_FILES] = {
        "../ressource/img/dice.png",
        "../ressource/img/logo.png",
        "../ressource/img/moon.png",
        "../ressource/img/neuil.jpg",
        "../ressource/img/sun.png",
        "../ressource/img/tree.jpg",
        "../ressource/exports.js",
        "../ressource/index.js",
        "../ressource/pages.css"
    };
    char* dests[NUMBER_FILES] = {
        "../export/img/dice.png",
        "../export/img/logo.png",
        "../export/img/moon.png",
        "../export/img/neuil.jpg",
        "../export/img/sun.png",
        "../export/img/tree.jpg",
        "../export/exports.js",
        "../export/index.js",
        "../export/pages.css"
    };

    // Create directories
    if (createDir("../export")) {
        return 1;
    }
    if (createDir("../export/img")) {
        return 1;
    }

    char* sourcePerson = "../index.html";
    char* destPersonBase = "../export/persons/";
    unsigned int lenDestBast = strlen(destPersonBase);

    // TODO : check for errors returned by completeFile

    // Copy all files
    for (unsigned int i=0; i<NUMBER_FILES; i++) {
        completeFile(sources[i], dests[i], gigaTree, 0);
    }

    // TODO: uncomment and test when GigaTree is ready
//     // Copy person files
//     struct Person* person;
//     // Supposing id is at most 58 characters : (if id has more characters, there probably is a problem)
//     unsigned int destPersonMaxSize = strlen(destPersonBase) + 64;
//     char* destPerson = malloc(destPersonMaxSize*sizeof(char));
//     if (destPerson == NULL) {
// #ifdef DEBUG
//         printf("Allocation error.\n");
// #endif
//         return 1;
//     }

//     for (unsigned int i=0; i<numberPersons(gigaTree); i++) {
//         person = getPersonByIndex(gigaTree, i);
//         snprintf(destPerson, destPersonMaxSize, "%s%d.html", destPersonBase, getID(person));
//         // printf("%s", destPerson);    // TODO : delete
//         completeFile(sourcePerson, destPerson, gigaTree, i);
//     }
//     free(destPerson);

    return 0;
}

int createDir(const char* folder) {
    struct stat st;
    int error;
    error = mkdir(folder, 0777);
    if (error) {
        stat(folder, &st);
        if (!S_ISDIR(st.st_mode)) {      // If error is not caused by already existing folder
#ifdef DEBUG
            printf("Error: cannot create a folder.\n");
#endif
            return 1;   // Abort function with error
        }
    }
    return 0;
}

// Copy input file to output, and replaces each <template> with appropriate value
int completeFile(char* input_filename, char* output_filename, struct GigaTree* gigatree, struct Person* person) {
    // Open files
    FILE* input = fopen(input_filename, "r");
    if (input == NULL) {
#ifdef DEBUG
        printf("Error occured whilst trying to open file.\n");
#endif
        return 1;
    }
    FILE* output = fopen(output_filename, "w");
    if (output == NULL) {
#ifdef DEBUG
        printf("Error occured whilst trying to create file.\n");
#endif
        fclose(input);
        return 1;
    }
    
    char* info = NULL;             // Raw info string requested by input file
    char** parsedInfos = NULL;
    char* replacedInfo = NULL;      // Info after being replaced by the value
    bool mustDeleteReplacedInfo;
    unsigned int numberInfos;
    copyUntilTemplate(input, output);
    while (!feof(input)) {
        if (info != NULL) {
            free(info);
        }
        info = readUntilEndTemplate(input);
        if (info != NULL) {
            if (parsedInfos != NULL) {
                free(parsedInfos);
            }
            parsedInfos = parseInfo(info, "/", &numberInfos);
            if (parsedInfos != NULL) {      // If parse had no error
                //replacedInfo = getValueOf(parsedInfos, numberInfos, person, gigatree, &mustDeleteReplacedInfo);   // TODO: uncomment
                replacedInfo = "ERROR";
                addStringToFile(output, replacedInfo);

                if (mustDeleteReplacedInfo) {
                    //free(replacedInfo);   // TODO: uncomment
                    replacedInfo = NULL;
                }
            }
        }
        copyUntilTemplate(input, output);
    }
    if (info != NULL) {
        free(info);
        info = NULL;
    }
    if (parsedInfos != NULL) {
        free(parsedInfos);
        parsedInfos = NULL;
    }
    fclose(input);
    fclose(output);

    return 0;   // No error happened
}

// Copy input to output, until we meet "<template>"" in the file
// "<template>" is not copied to output
void copyUntilTemplate(FILE* input, FILE* output) {
    const unsigned int LEN_TEMPLATE = 10;
    const char TEMPLATE[11] = "<template>";

    unsigned int i_template = 0;
    char c = fgetc(input);

    while (!feof(input)) {
        if (c == TEMPLATE[i_template]) {
            if (i_template == LEN_TEMPLATE-1) {
                return;
            }
            i_template += 1;
        }
        else {
            if (i_template > 0) {
                for (unsigned int j=0; j<i_template; j++) {
                    fputc(TEMPLATE[j], output);
                }
                i_template = 0;
            }
            fputc(c, output);
        }
        c = fgetc(input);
    }
}

// Read f until we find end of template string
// All characters before end of template string are returned in a string
char* readUntilEndTemplate(FILE* f) {
    const unsigned int LEN_ENDTEMPLATE = 11;
    const char ENDTEMPLATE[12] = "</template>";

    char* info = malloc((MAX_INFO_LEN+1)*sizeof(char));     // Info is the content of a template
    if (info == NULL) {
#ifdef DEBUG
        printf("Allocation error\n");
#endif
        return NULL;
    }

    unsigned int i_template = 0;
    unsigned int i_info = 0;
    char c = fgetc(f);

    while (!feof(f) && i_info < MAX_INFO_LEN) {
        if (c == ENDTEMPLATE[i_template]) {
            if (i_template == LEN_ENDTEMPLATE -1) {
                info[i_info] = '\0';
                return info;
            }
            i_template += 1;
        }
        else {
            if (i_template > 0) {
                for (unsigned int j=0; j<i_template; j++) {
                    info[i_info] = ENDTEMPLATE[j];
                    i_info += 1;
                }
                i_template += 1;
            }
            info[i_info] = c;
            i_info += 1;
        }
        c = fgetc(f);
    }
#ifdef DEBUG
    printf("Error occured whilst searching for end of template.\n");
#endif
    return NULL;
}

// Add string s to file f
void addStringToFile(FILE* f, char* s) {
    unsigned int string_size = strlen(s);
    unsigned int i = 0;
    for (unsigned int i=0; i<string_size; i++) {
        fputc(s[i], f);
    }
}

// Parse info with given separator.
// Returns an array of strings of size parsedLength
char** parseInfo(char* rawInfo, char* separator, unsigned int* numberInfos) {
    unsigned int infoLen = strlen(rawInfo);

    // Count number of separated infos
    unsigned int separatorCount = 0;
    for (unsigned int i=0; i<infoLen; i++) {
        if (rawInfo[i] == ':') {
            separatorCount += 1;
        }
    }

    // Allocate memory
    char** parsedInfos = malloc((separatorCount+1) * sizeof(char*));
    if (parsedInfos == NULL) {
#ifdef DEBUG
        printf("Allocation error.\n");
#endif
        return NULL;
    }
    *numberInfos = separatorCount +1;

    // Parse info with separator ':'
    char* token = strtok(rawInfo, ":");
    unsigned int iParsed = 0;
    while (token != NULL) {
        // Allocate memory for current info part
        parsedInfos[iParsed] = malloc((strlen(token)+1)*sizeof(char));
        if (parsedInfos[iParsed] == NULL) {
#ifdef DEBUG
            printf("Allocation error.\n");
#endif
            // Avoid memory leaks in case of satured memory
            for (unsigned int j=0; j<iParsed; j++) {
                free(parsedInfos[j]);
            }
            free(parsedInfos);
            return NULL;
        }
        strcpy(parsedInfos[iParsed], token);
        token = strtok(NULL, ":");
        iParsed += 1;
    }
    return parsedInfos;
}

// String value corresponding to parsed info.
// Parsed info must correspond to predifined format
char* getValueOf(char** parsedInfo, unsigned int numberInfos, struct Person* person, struct GigaTree* gigatree, bool* mustDelete) {
    if (numberInfos == 0) {
        *mustDelete = false;
        return NULL;
    }
    // TODO: uncomment
//     if (strcmp(parsedInfo[0], FATHER_TEMPLATE) == 0) {
//         return getValueOf(parsedInfo+1, numberInfos-1, getPadre(person), gigatree);
//     }

//     if (strcmp(parsedInfo[0], MOTHER_TEMPLATE) == 0) {
//         return getValueOf(parsedInfo-1, numberInfos-1, getMadre(person), gigatree);
//     }

//     if (strcmp(parsedInfo[0], LASTNAME_TEMPLATE) == 0) {
//         *mustDelete = false;
//         return getLastName(person);
//     }

//     if (strcmp(parsedInfo[0], FORNAME_TEMPLATE) == 0) {
//         *mustDelete = false;
//         return getFirstName(person);
//     }

//     if (strcmp(parsedInfo[0], REGION_TEMPLATE) == 0) {
//         *mustDelete = false;
//         return getRegion(person);
//     }

//     if (strcmp(parsedInfo[0], BIRTH_DAY_TEMPLATE) == 0) {
//         char* birthDay = malloc(sizeof(char)*3);       // A birth day is composed of 1 or 2 numbers
//         if (birthDay == NULL) {
// #ifdef DEBUG
//             printf("Allocation error.\n");
// #endif
//             *mustDelete = false;
//             return "ERROR";
//         }
//         *mustDelete = true;
//         sprintf(birthDay, "%d", getBirthday(person)[0]);
//         return birthDay;
//     }

//     if (strcmp(parsedInfo[0], BIRTH_MONTH_TEMPLATE) == 0) {
//         char* birthMonth = malloc(sizeof(char)*3);     // A birth month is composed of 1 or 2 numbers
//         if (birthMonth == NULL) {
// #ifdef DEBUG
//             printf("Allocation error.\n");
// #endif
//             *mustDelete = false;
//             return "ERROR";
//         }
//         *mustDelete = true;
//         sprintf(birthMonth, "%d", getBirthday(person)[1]);
//         return birthMonth;
//     }

//     if (strcmp(parsedInfo[0], BIRTH_YEAR_TEMPLATE) == 0) {
//         char* birthYear = malloc(sizeof(char)*7);     // A birth month is composed of 1-6 numbers
//         if (birthYear == NULL) {
// #ifdef DEBUG
//             printf("Allocation error.\n");
// #endif
//             *mustDelete = false;
//             return "ERROR";
//         }
//         *mustDelete = true;
//         sprintf(birthYear, "%d", getBirthday(person)[2]);
//         return birthYear;
//     }

    return "ERREUR";
}