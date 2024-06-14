#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// int exportFamilies(struct GigaTree* gigaTree) {
//     return 0;
// }

// int exportInfoFiles(struct GigaTree* gigaTree) {
//     return 0;
// }

// Copy input file to output, and replaces each <template> with appropriate value
void completeFile(char* input_filename, char* output_filename, struct GigaTree* gigatree, unsigned int index_person) {
    // Open files
    FILE* input = fopen(input_filename, "r");
    if (input == NULL) {
#ifdef DEBUG
        printf("Error occured whilst trying to open file.\n");
#endif
        return;
    }
    FILE* output = fopen(output_filename, "w");
    if (output == NULL) {
#ifdef DEBUG
        printf("Error occured whilst trying to create file.\n");
#endif
        fclose(input);
        return;
    }
    
    char* info;             // Raw info string requested by input file
    char** parsedInfos;
    char* replacedInfo;   // Info after being replaced by the value
    unsigned int numberInfos;
    copyUntilTemplate(input, output);
    while (!feof(input)) {
        info = readUntilEndTemplate(input);
        if (info != NULL) {
            parsedInfos = parseInfo(info, "/", &numberInfos);
            replacedInfo = getValueOf(parsedInfos, numberInfos, gigatree, index_person);
            addStringToFile(output, replacedInfo);
        }
        copyUntilTemplate(input, output);
    }
    fclose(input);
    fclose(output);
}

// Copy input to output, until we meet "<template>"" in the file
// "<template>" is not copied to output
void copyUntilTemplate(FILE* input, FILE* output) {
    const unsigned int LEN_TEMPLATE = 10;
    const char TEMPLATE[11] = "<template>";

    unsigned int i_template = 0;
    char c;

    while (!feof(input)) {
        c = fgetc(input);
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
    char c;

    while (!feof(f) && i_info < MAX_INFO_LEN) {
        c = fgetc(f);
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
char* getValueOf(char** parsedInfo, unsigned int numberInfos, struct GigaTree* gigatree, unsigned int index_person) {
    if (numberInfos == 0) {
        return NULL;
    }
    if (strcmp(parsedInfo[0], FATHER_TEMPLATE) == 0) {
        return "Father";
    }
    if (strcmp(parsedInfo[0], MOTHER_TEMPLATE) == 0) {
        return "Mother";
    }
    if (strcmp(parsedInfo[0], LASTNAME_TEMPLATE) == 0) {
        return "Smith";
    }
    if (strcmp(parsedInfo[0], FORNAME_TEMPLATE) == 0) {
        return "John";
    }
    if (strcmp(parsedInfo[0], REGION_TEMPLATE) == 0) {
        return "Alabama";
    }
    if (strcmp(parsedInfo[0], BIRTH_DAY_TEMPLATE) == 0) {
        return "1";
    }
    if (strcmp(parsedInfo[0], BIRTH_MONTH_TEMPLATE) == 0) {
        return "Janvier";
    }
    if (strcmp(parsedInfo[0], BIRTH_YEAR_TEMPLATE) == 0) {
        return "2024";
    }
    return "ERREUR";
}