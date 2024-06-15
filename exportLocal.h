#ifndef _EXPORTLOCAL_H
#define _EXPORTLOCAL_H

#include <stdio.h>

#include "gigaTree.h"
#include "person.h"

/**
 * Create local site with given data
 * @param gigaTree Data to use to create local site
 * @return 1 if an error occured, 0 otherwise
 */
int exportLocalSite(struct GigaTree* gigaTree);

/**
 * Create a new directory. Return no error if directory already exists.
 * @param dirName name or path of directory
 * @return 1 if an error occured, 0 otherwise
 */
int createDir(const char* dirName);

/**
 * Copy input file to output. Each template is replaced by appropriate value.
 * @param inputFilename name of input file
 * @param outputFilename name of output file
 * @param gigatree GigaTree containing all families data
 * @param person The person the file is related to. For global files, any person in gigatree works.
 * @return 1 if an error occured, 0 otherwise
 */
int completeFile(char* inputFilename, char* outputFilename, struct GigaTree* gigatree, struct Person* person);

/**
 * Copy input to output, until the template string is found.
 * Template string is skipped.
 * @param input Input file
 * @param output Output file
 */
void copyUntilTemplate(FILE* input, FILE* output);

/**
 * Read file until the end of template string is found.
 * End of template string is skipped.
 * @param f The file to read
 * @return Return string read before end of template
 */
char* readUntilEndTemplate(FILE* f);

/**
 * Parse info string into multiple substrings, using the given separator
 * @param[in] info String to parse
 * @param[in] separator Separates substrings
 * @param[out] parsedLength Number of substrings returned
 * @return Return an array of strings, of size parsedLength.
 */
char** parseInfo(char* info, char separator, unsigned int* parsedLength);

/**
 * Get the value corresponding to a parsed info using given GigaTree and Person.
 * @param[in] parsedInfo Array of strings, parsed requested information. For example: {"padre", "forname"} means forname of father
 * @param[in] numberInfos size of array parsedInfo
 * @param[in] person Data to use to answer info request
 * @param[in] gigatree Data to use to answer info request
 * @param[out] mustDelete whether returned string requires a free() or not
 * @return Return the value requested in a string. Return NULL if an error occured, and "ERROR" if requested info is unknown
 */
char* getValueOf(char** parsedInfo, unsigned int numberInfos, struct Person* person, struct GigaTree* gigatree, bool* mustDelete);

/**
 * Delete the given array of strings
 * @param arrayStrings The array of strings to delete
 * @param lenArray Size of the array
 */
void deleteArrayStrings(char*** arrayStrings, unsigned int lenArray);

#endif // _EXPORTLOCAL_H