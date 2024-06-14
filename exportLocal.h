#ifndef _EXPORTLOCAL_H
#define _EXPORTLOCAL_H

#include <stdio.h>

#include "gigaTree.h"
#include "person.h"

// int exportFamilies(struct GigaTree* gigaTree);
// int exportInfoFiles(struct GigaTree* gigaTree);

// Copy input file to output, and replaces each <template> with appropriate value
void completeFile(char* input_filename, char* output_filename, struct GigaTree* gigatree, unsigned int index_person);

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

// String value corresponding to parsed info.
// Parsed info must correspond to predifined format
char* getValueOf(char** parsedInfo, unsigned int numberInfos, struct GigaTree* gigatree, unsigned int index_person);

#endif // _EXPORTLOCAL_H