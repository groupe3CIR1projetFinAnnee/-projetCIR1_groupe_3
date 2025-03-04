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

// Templates definition
#define ID_TEMPLATE "id"
#define LASTNAME_TEMPLATE "lastname"
#define FORNAME_TEMPLATE "forname"
#define REGION_TEMPLATE "region"
#define BIRTH_DAY_TEMPLATE "birth_day"
#define BIRTH_MONTH_TEMPLATE "birth_month"
#define BIRTH_YEAR_TEMPLATE "birth_year"
#define GENDER_TEMPLATE "gender"
#define FATHER_TEMPLATE "padre"
#define MOTHER_TEMPLATE "madre"
#define NUM_PEOPLE_TEMPLATE "number_people"
#define NUM_MEN_TEMPLATE "rate_men"
#define NUM_WOMEN_TEMPLATE "rate_women"
#define NUM_FAMILIES_TEMPLATE "number_families"
#define INBREEDING_TEMPLATE "rate_inbreeding"
#define ID_ARRAY_TEMPLATE "array_ids"

#define MAN_STR "man"
#define WOMAN_STR "woman"
#define UNKNOWN_STR "unknown"

// There is a max of info length because info is a predifined string
#define MAX_INFO_LEN 64   // Info is the content of a template
#define NUMBER_FILES 14

/**
 * Create local site with given data
 * @param gigaTree Data to use to create local site
 * @return 1 if an error occured, 0 otherwise
 */
int exportLocalSite(struct GigaTree* gigaTree) {
    // Arrays of sources/dest files (except template page of a person)
    char* sources[NUMBER_FILES] = {
        "../resource/img/dice.png",
        "../resource/img/logo.png",
        "../resource/img/moon.png",
        "../resource/img/unknown.png",
        "../resource/img/woman.png",
        "../resource/img/man.png",
        "../resource/img/sun.png",
        "../resource/img/tree.jpg",
        "../resource/img/logo_gigatree.png",
        "../resource/pages.js",
        "../resource/infos.js",
        "../resource/pages.css",
        "../resource/infos.css",
        "../resource/infos.html"
    };
    char* dests[NUMBER_FILES] = {
        "../export/img/dice.png",
        "../export/img/logo.png",
        "../export/img/moon.png",
        "../export/img/unknown.png",
        "../export/img/woman.png",
        "../export/img/man.png",
        "../export/img/sun.png",
        "../export/img/tree.jpg",
        "../export/img/logo_gigatree.png",
        "../export/pages.js",
        "../export/infos.js",
        "../export/pages.css",
        "../export/infos.css",
        "../export/infos.html"
    };
    char* sourcePerson = "../resource/template.html";           // Source template file of a person
    char* destPersonBase = "../export/persons/";    // Dest directory of persons

    // Create directories
    if (createDir("../export")) {
        return 1;
    }
    if (createDir("../export/img")) {
        return 1;
    }
    if (createDir("../export/persons")) {
        return 1;
    }

    // Copy all global files
    int error;
    for (unsigned int i=0; i<NUMBER_FILES; i++) {
        error = completeFile(sources[i], dests[i], gigaTree, getPersonByIndex(gigaTree, 0));
        if (error) {
            return 1;
        }
    }

    // Prepare variables
    struct Person* person;
    // Supposing id is at most 200 characters : (if id has more characters, there probably is a problem)
    unsigned int destPersonMaxSize = strlen(destPersonBase) + 206;
    char* destPerson = malloc(destPersonMaxSize*sizeof(char));
    if (destPerson == NULL) {
#ifdef DEBUG
        printf("Allocation error.\n");
#endif
        return 1;
    }
    unsigned int id;
    char* id_str;
    bool mustDelete;

    // Copy person files
    for (unsigned int i=0; i<numberPersons(gigaTree); i++) {
        person = getPersonByIndex(gigaTree, i);
        unsigned int id = getID(person);
        mustDelete = false;
        id_str = uintToString(id, 200, &mustDelete);

        // Write dest filename to destPerson
        snprintf(destPerson, destPersonMaxSize, "%s%s.html", destPersonBase, id_str);
        if (mustDelete) {
            free(id_str);
        }
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
#ifdef __unix__
    error = mkdir(folder, 0777);
#elif defined(_WIN32) || defined(WIN32)
    error = mkdir(folder);
#endif

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
        printf("Error occured whilst trying to open file %s.\n", inputFilename);
#endif
        return 1;
    }
    
    FILE* output = fopen(outputFilename, "w");     // Open output
    if (output == NULL) {
#ifdef DEBUG
        printf("Error occured whilst trying to create file %s.\n", outputFilename);
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
            fclose(input);
            fclose(output);
            return 1;
        }
        // Parse info into multiple strings (e.g. "padre:lastname" becomes {"padre", "lastname"})
        parsedInfos = parseInfo(info, ':', &numberInfos);
        if (parsedInfos == NULL) {      // Parse error
            free(info);
            fclose(input);
            fclose(output);
            return 1;
        }
        // Get final value asked by the template
        replacedInfo = getValueOf(parsedInfos, numberInfos, person, gigatree, &mustDeleteReplacedInfo);
        if (replacedInfo == NULL) {     // Error in given parameters
            free(info);
            deleteArrayStrings(&parsedInfos, numberInfos);
            fclose(input);
            fclose(output);
            return 1;
        }

        // Add value to output file
        fprintf(output, "%s", replacedInfo);

        // Free allocated variables
        if (mustDeleteReplacedInfo) {
            free(replacedInfo);
            replacedInfo = NULL;
        }
        deleteArrayStrings(&parsedInfos, numberInfos);
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
 * @return Return the value requested in a string. Return NULL if an error occured, and "ERREUR" if requested info is unknown
 */
char* getValueOf(char** parsedInfo, unsigned int numberInfos, struct Person* person, struct GigaTree* gigatree, bool* mustDelete) {
    *mustDelete = false;
    if (numberInfos == 0) {
#ifdef DEBUG
        printf("An error occured because template information is invalid.\n");
#endif
        return NULL;
    }
    
    // Compare parsedInfo[0] to each predifined template and return corresponding information
    if (strcmp(parsedInfo[0], FATHER_TEMPLATE) == 0) {
        if (person == NULL) {
            return "ERREUR";
        }
        return getValueOf(parsedInfo+1, numberInfos-1, getPadre(person), gigatree, mustDelete);
    }

    if (strcmp(parsedInfo[0], MOTHER_TEMPLATE) == 0) {
        if (person == NULL) {
            return "ERREUR";
        }
        return getValueOf(parsedInfo+1, numberInfos-1, getMadre(person), gigatree, mustDelete);
    }

    if (strcmp(parsedInfo[0], LASTNAME_TEMPLATE) == 0) {
        if (person == NULL) {
            return "ERREUR";
        }
        if (getID(person) == 0) {       // Default person
            return "inconnu";
        }
        return getLastName(person);
    }

    if (strcmp(parsedInfo[0], FORNAME_TEMPLATE) == 0) {
        if (person == NULL) {
            return "ERREUR";
        }
        if (getID(person) == 0) {       // Default person
            return "inconnu";
        }
        return getFirstName(person);
    }

    if (strcmp(parsedInfo[0], REGION_TEMPLATE) == 0) {
        if (person == NULL) {
            return "ERREUR";
        }
        if (getID(person) == 0) {       // Default person
            return "inconnue";
        }
        return getRegion(person);
    }

    if (strcmp(parsedInfo[0], BIRTH_DAY_TEMPLATE) == 0) {
        if (person == NULL) {
            return "ERREUR";
        }
        if (getID(person) == 0) {       // Default person
            return "00";
        }
        unsigned int* birth = getBirthday(person);
        unsigned int birthDay = birth[0];
        free(birth);
        return uintToString(birthDay, 16, mustDelete);      // A birth day is composed of 1 or 2 numbers
    }

    if (strcmp(parsedInfo[0], BIRTH_MONTH_TEMPLATE) == 0) {
        if (person == NULL) {
            return "ERREUR";
        }
        if (getID(person) == 0) {       // Default person
            return "00";
        }
        unsigned int* birth = getBirthday(person);
        unsigned int birthMonth = birth[1];
        free(birth);
        return uintToString(birthMonth, 16, mustDelete);    // A birth month is composed of 1 or 2 numbers
    }

    if (strcmp(parsedInfo[0], BIRTH_YEAR_TEMPLATE) == 0) {
        if (person == NULL) {
            return "ERREUR";
        }
        if (getID(person) == 0) {       // Default person
            return "0000";
        }
        unsigned int* birth = getBirthday(person);
        unsigned int birthYear = birth[2];
        free(birth);
        return uintToString(birthYear, 16, mustDelete);     // A birth year is composed of 1-6 numbers
    }

    if (strcmp(parsedInfo[0], GENDER_TEMPLATE) == 0) {
        if (person == NULL) {
            return "ERREUR";
        }
        int gender = getSex(person);
        switch (gender) {
            case MALE:
                return MAN_STR;
            case FEMALE:
                return WOMAN_STR;
            default:
                return UNKNOWN_STR;
        }
    }

    if (strcmp(parsedInfo[0], ID_TEMPLATE) == 0) {
        if (person == NULL) {
            return "ERREUR";
        }
        unsigned int id = getID(person);
        return uintToString(id, 255, mustDelete);   // Id has probably less than 255 characters
    }

    if (strcmp(parsedInfo[0], NUM_PEOPLE_TEMPLATE) == 0) {
        if (gigatree == NULL) {
            return "ERREUR";
        }
        unsigned int numPeople = numberPersons(gigatree);
        return uintToString(numPeople, 200, mustDelete);        // Number of people is probably lower than 200 characters
    }

    if (strcmp(parsedInfo[0], NUM_MEN_TEMPLATE) == 0) {
        if (gigatree == NULL) {
            return "ERREUR";
        }

        unsigned int numMales = numberMale(gigatree);
        unsigned numFemales = numberFemale(gigatree);
        unsigned int maleRate = (unsigned int) (100* ((double)numMales) / (numMales+numFemales));
        return uintToString(maleRate, 200, mustDelete); // Rate of men is probably lower than 200 characters
    }

    if (strcmp(parsedInfo[0], NUM_WOMEN_TEMPLATE) == 0) {
        if (gigatree == NULL) {
            return "ERREUR";
        }

        unsigned int numMales = numberMale(gigatree);
        unsigned numFemales = numberFemale(gigatree);
        unsigned int femaleRate = (unsigned int) (100* ((double)numFemales) / (numMales+numFemales));
        return uintToString(femaleRate, 200, mustDelete);     // Rate of women is probably lower than 200 characters
    }

    if (strcmp(parsedInfo[0], NUM_FAMILIES_TEMPLATE) == 0) {
        if (gigatree == NULL) {
            return "ERREUR";
        }

        unsigned int numFamilies = numberFamilies(gigatree)-1;
        return uintToString(numFamilies, 200, mustDelete);  // Number of families is probably lower than 200 characters
    }

    if (strcmp(parsedInfo[0], INBREEDING_TEMPLATE) == 0) {
        if (gigatree == NULL) {
            return "ERREUR";
        }
        unsigned int inbreedingRate = (unsigned int) ((double)inbreeding(gigatree) / numberPersons(gigatree));
        return uintToString(inbreedingRate, 200, mustDelete);   // Inbreeding rate is probably lower than 200 characters
    }

    if (strcmp(parsedInfo[0], ID_ARRAY_TEMPLATE) == 0) {
        if (gigatree == NULL) {
            return "ERREUR";
        }
        // return JS formated array of every person id

        // Find how much memory is needed
        unsigned int numPeople = numberPersons(gigatree);
        if (numPeople == 0) {
            return "[]";
        }
        unsigned int jsArray_len = 1;
        unsigned int id;
        bool delID;
        char* id_str;
        for (unsigned int i=0; i<numPeople; i++) {
            id = getID(getPersonByIndex(gigatree, i));
            delID = false;
            id_str = uintToString(id, 200, &delID);
            jsArray_len += strlen(id_str) +1;
            if (delID) {
                free(id_str);
            }
        }

        // Allocate memory
        char* jsArray = malloc((jsArray_len+1)*sizeof(char));
        if (jsArray == NULL) {
#ifdef DEBUG
            printf("Allocation error.\n");
#endif
            return "ERREUR";
        }

        // Write JS array
        unsigned int jsArray_index = 1;
        jsArray[0] = '[';
        for (unsigned int i=0; i<numPeople; i++) {
            id = getID(getPersonByIndex(gigatree, i));
            delID = false;
            id_str = uintToString(id, 200, &delID);
            strcpy(jsArray +jsArray_index, id_str);
            jsArray_index += strlen(id_str) +1;
            jsArray[jsArray_index-1] = ',';
            if (delID) {
                free(id_str);
            }
        }
        jsArray[jsArray_index-1] = ']';
        jsArray[jsArray_index] = '\0';
        *mustDelete = true;
        return jsArray;
    }

    return "ERREUR";
}


/**
 * Convert an unsigned int to a string
 * @param uint The unsigned int to convert
 * @param max_char Max length of output string
 * @param[out] mustDelete Set to true if return variable if allocated in stack, false otherwise
 * @return String containing the uint
 */
char* uintToString(unsigned int uint, unsigned int max_char, bool* mustDelete) {
    *mustDelete = false;
    char* buffer = malloc(max_char *sizeof(char));
    if (buffer == NULL) {
#ifdef DEBUG
        printf("Allocation error.\n");
#endif
        return "ERREUR";
    }
    snprintf(buffer, 200, "%d", uint);

    unsigned int buffer_len = strlen(buffer);
    char* outputString = malloc(sizeof(char) * (buffer_len+1));
    if (outputString == NULL) {
    #ifdef DEBUG
        printf("Allocation error.\n");
    #endif
        free(buffer);
        return "ERREUR";
    }
    strcpy(outputString, buffer);
    free(buffer);
    *mustDelete = true;
    return outputString;
}

/**
 * Delete the given array of strings
 * @param arrayStrings The array of strings to delete
 * @param lenArray Size of the array
 */
void deleteArrayStrings(char*** arrayStrings, unsigned int lenArray) {
    if (*arrayStrings != NULL) {
        for (unsigned int i=0; i<lenArray; i++) {
            if ((*arrayStrings)[i] != NULL) {
                free((*arrayStrings)[i]);
                (*arrayStrings)[i] = NULL;
            }
        }

        free(*arrayStrings);
        *arrayStrings = NULL;
    }
}