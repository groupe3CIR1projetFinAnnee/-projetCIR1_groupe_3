#ifndef _EXPORTLOCAL_H
#define _EXPORTLOCAL_H

#include <stdio.h>

#include "gigaTree.h"
#include "person.h"

int exportLocalSite(struct GigaTree* gigaTree);

int createDir(const char* folder);

// Copy input file to output, and replaces each <template> with appropriate value
int completeFile(char* input_filename, char* output_filename, struct GigaTree* gigatree, struct Person* person);

// Copy input to output until we find the template string
// Template string is not copied to output
void copyUntilTemplate(FILE* input, FILE* output);

// Read f until we find end of template string
// All characters before end of template string are returned in a string
char* readUntilEndTemplate(FILE* f);

// Add string s to file f
void addStringToFile(FILE* f, char* s);

// Parse info with given separator.
// Returns an array of strings of size parsedLength
char** parseInfo(char* info, char* separator, unsigned int* parsedLength);

// Return string value corresponding to parsed info.
// Parsed info must correspond to predifined format
// If must delete is true, returned string must be freed
char* getValueOf(char** parsedInfo, unsigned int numberInfos, struct Person* person, struct GigaTree* gigatree, bool* mustDelete);

#endif // _EXPORTLOCAL_H