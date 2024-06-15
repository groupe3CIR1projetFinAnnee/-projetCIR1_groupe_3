#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Directory creation/verification packages
#include <sys/stat.h>
#include <unistd.h>

#include "exportLocal.h"
#include "gigaTree.h"
#include "person.h"

#define DEBUG

// Templates definition
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
#define NUMBER_FILES 10

/**
 * Create local site with given data
 * @param gigaTree Data to use to create local site
 * @return 1 if an error occured, 0 otherwise
 */
int exportLocalSite(struct GigaTree* gigaTree) {
    // Arrays of sources/dest files (except template page of a person)
    char* sources[NUMBER_FILES] = {
        "../ressource/img/dice.png",
        "../ressource/img/logo.png",
        "../ressource/img/moon.png",
        "../ressource/img/neuil.jpg",
        "../ressource/img/sun.png",
        "../ressource/img/tree.jpg",
        "../ressource/exports.js",
        "../ressource/index.js",
        "../ressource/pages.css",
        "test.txt"
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
        "../export/pages.css",
        "output_test.txt"
    };
    char* sourcePerson = "../index.html";           // Source template file of a person
    char* destPersonBase = "../export/persons/";    // Dest directory of persons

    // Create directories
    if (createDir("../export")) {
        return 1;
    }
    if (createDir("../export/img")) {
        return 1;
    }

    // Copy all global files
    int error;
    for (unsigned int i=0; i<NUMBER_FILES; i++) {
        error = completeFile(sources[i], dests[i], gigaTree, 0);
        if (error) {
            return 1;
        }
    }

    // TODO: uncomment and test when GigaTree is ready
    // Prepare variables
    struct Person* person;
    // Supposing id is at most 58 characters : (if id has more characters, there probably is a problem)
    unsigned int destPersonMaxSize = strlen(destPersonBase) + 64;
    char* destPerson = malloc(destPersonMaxSize*sizeof(char));
    if (destPerson == NULL) {
#ifdef DEBUG
        printf("Allocation error.\n");
#endif
        return 1;
    }

    // Copy person files
    for (unsigned int i=0; i<numberPersons(gigaTree); i++) {
        person = getPersonByIndex(gigaTree, i);

        // Write dest filename to destPerson
        snprintf(destPerson, destPersonMaxSize, "%s%d.html", destPersonBase, getID(person));
        // printf("%s", destPerson);    // TODO : delete

        error = completeFile(sourcePerson, destPerson, gigaTree, person);
        if (error) {
            free(destPerson);
            return 1;
        }
    }
    free(destPerson);

    return 0;
}


/**
 * Create a new directory. Return no error if directory already exists.
 * @param dirName name or path of directory
 * @return 1 if an error occured, 0 otherwise
 */
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


/**
 * Copy input file to output. Each template is replaced by appropriate value.
 * @param inputFilename name of input file
 * @param outputFilename name of output file
 * @param gigatree GigaTree containing all families data
 * @param person The person the file is related to. For global files, any person in gigatree works.
 * @return 1 if an error occured, 0 otherwise
 */
int completeFile(char* inputFilename, char* outputFilename, struct GigaTree* gigatree, struct Person* person) {
    // Open files
    FILE* input = fopen(inputFilename, "r");       // Open input
    if (input == NULL) {
#ifdef DEBUG
        printf("Error occured whilst trying to open file.\n");
#endif
        return 1;
    }

    FILE* output = fopen(outputFilename, "w");     // Open output
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
    bool mustDeleteReplacedInfo = false;
    unsigned int numberInfos;

    // Copy file. Stop if a template is found
    copyUntilTemplate(input, output);

    while (!feof(input)) {
        // Skip input file until end of template. Template content is stored into info
        info = readUntilEndTemplate(input);
        if (info == NULL) {     // Error: end of template unreachable
            return 1;
        }
        // Parse info into multiple strings (e.g. "padre:lastname" becomes {"padre", "lastname"})
        parsedInfos = parseInfo(info, ':', &numberInfos);
        if (parsedInfos == NULL) {      // Parse error
            free(info);
            return 1;
        }
        // Get final value asked by the template
        //replacedInfo = getValueOf(parsedInfos, numberInfos, person, gigatree, &mustDeleteReplacedInfo);   // TODO: uncomment
        if (replacedInfo == NULL) {     // Error in given parameters
            free(info);
            free(parsedInfos);
            return 1;
        }
        replacedInfo = "ERROR";     // TODO: delete

        // Add value to output file
        fprintf(output, "%s", replacedInfo);

        // Free allocated variables
        if (mustDeleteReplacedInfo) {
            //free(replacedInfo);   // TODO: uncomment
            replacedInfo = NULL;
        }
        free(parsedInfos);
        parsedInfos = NULL;
        free(info);
        info = NULL;

        // Continue copying until we find another template
        copyUntilTemplate(input, output);
    }
    fclose(input);
    fclose(output);

    return 0;   // No error happened
}


/**
 * Copy input to output, until the template string is found.
 * Template string is skipped.
 * @param input Input file
 * @param output Output file
 */
void copyUntilTemplate(FILE* input, FILE* output) {
    // Define template string. TEMPLATE must be of size LEN_TEMPLATE+1
    const unsigned int LEN_TEMPLATE = 10;
    const char TEMPLATE[11] = "<template>";

    unsigned int i_template = 0;
    char c = fgetc(input);
    // For each character
    while (!feof(input)) {
        // If the character continues the template string
        if (c == TEMPLATE[i_template]) {
            // If template string entirely found
            if (i_template == LEN_TEMPLATE-1) {
                return;
            }
            i_template += 1;
        }
        else {
            // If there is an uncomplete template string, paste it to output
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


/**
 * Read file until the end of template string is found.
 * End of template string is skipped.
 * @param f The file to read
 * @return Return string read before end of template
 */
char* readUntilEndTemplate(FILE* f) {
    // Define end template string. ENDTEMPLATE must be of size LEN_ENDTEMPLATE+1
    const unsigned int LEN_ENDTEMPLATE = 11;
    const char ENDTEMPLATE[12] = "</template>";

    // Info is the content of a template
    char* info = malloc((MAX_INFO_LEN+1)*sizeof(char));
    if (info == NULL) {
#ifdef DEBUG
        printf("Allocation error\n");
#endif
        return NULL;
    }

    unsigned int i_template = 0;
    unsigned int i_info = 0;
    char c = fgetc(f);
    // For each character
    while (!feof(f) && i_info < MAX_INFO_LEN) {
        // If the character continues the end template string
        if (c == ENDTEMPLATE[i_template]) {
            // If end template string entirely found
            if (i_template == LEN_ENDTEMPLATE -1) {
                info[i_info] = '\0';
                return info;
            }
            i_template += 1;
        }
        else {
            // If there is an uncomplete end template string, paste it to info
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

    // If we reach end of file, or if template content is too long, there probably is an error is the file
#ifdef DEBUG
    printf("Error occured whilst searching for end of template.\n");
#endif
    free(info);
    return NULL;
}


/**
 * Parse info string into multiple substrings, using the given separator
 * @param[in] info String to parse
 * @param[in] separator Separates substrings
 * @param[out] parsedLength Number of substrings returned
 * @return Return an array of strings, of size parsedLength.
 */
char** parseInfo(char* rawInfo, char separator, unsigned int* numberInfos) {
    // Count number of separated infos
    unsigned int infoLen = strlen(rawInfo);
    unsigned int separatorCount = 0;
    for (unsigned int i=0; i<infoLen; i++) {
        if (rawInfo[i] == separator) {
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

    // Convert char separator into string
    char str_sep[2] = {separator, '\0'};

    // Parse info using separator
    char* token = strtok(rawInfo, str_sep);
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
        // Copy substring to current info part
        strcpy(parsedInfos[iParsed], token);

        token = strtok(NULL, str_sep);      // Get string until next separator
        iParsed += 1;
    }
    return parsedInfos;
}


/**
 * Get the value corresponding to a parsed info using given GigaTree and Person.
 * @param[in] parsedInfo Array of strings, parsed requested information. For example: {"padre", "forname"} means forname of father
 * @param[in] numberInfos size of array parsedInfo
 * @param[in] person Data to use to answer info request
 * @param[in] gigatree Data to use to answer info request
 * @param[out] mustDelete whether returned string requires a free() or not
 * @return Return the value requested in a string. Return NULL if an error occured, and "ERROR" if requested info is unknown
 */
char* getValueOf(char** parsedInfo, unsigned int numberInfos, struct Person* person, struct GigaTree* gigatree, bool* mustDelete) {
    if (numberInfos == 0) {
#ifdef DEBUG
        printf("An error occured because template information is invalid.\n");
#endif
        *mustDelete = false;
        return NULL;
    }
    // Compare parsedInfo[0] to each predifined template and return corresponding information
    // TODO: uncomment
//     if (strcmp(parsedInfo[0], FATHER_TEMPLATE) == 0) {
//         return getValueOf(parsedInfo+1, numberInfos-1, getPadre(person), gigatree);
//     }

//     if (strcmp(parsedInfo[0], MOTHER_TEMPLATE) == 0) {
//         return getValueOf(parsedInfo+1, numberInfos-1, getMadre(person), gigatree);
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