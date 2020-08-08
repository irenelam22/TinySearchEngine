/* 
 * indexer.c - first driver for `index` module
 *
 * The indexer reads the documents in a given `directory` outputted by the 
 * crawler, builds an inverted index mapping from words to document ID and count, 
 * and writes that index to a file
 * 
 * see IMPLEMENTATION and DESIGN spec for more information.
 *
 * Irene Lam, August 4, 2020
 * CS50, Summer 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"
#include "../common/pagedir.h"
#include "../common/index.h"

/************** main ***************
 * Builds an inverted index mapping from words to document ID and count
 * based on crawler output
 * 
 * Inputs:
 * pageDirectory: the pathname of a directory produced by the Crawler, and
 * indexFilename: the pathname of a file into which the index should be written
 * Output: 0 if successful, 1 otherwise
 * 
 * Usage: ./indexer pageDirectory indexFilename
 */
int main(int argc, char* argv[])
{
    // Error-handling
    if (argc < 3) {
        fprintf(stderr, "Insufficient number of arguments\n");
        return 1;
    }
    if (argc > 3) {
        fprintf(stderr, "Too many arguments provided\n");
        return 1;
    }
    char* dir = argv[1];
    char* dircopy = assertp(malloc(strlen(argv[1])+10), "dir copy");
    strcpy(dircopy, dir);

    // Add a backslash to the end of the directory if needed
    if (dircopy[strlen(dircopy)-1] != '/') {
        strcat(dircopy, "/");
    }

    // Check if the given directory is a valid, crawler-generated directory
    char* pagedir = assertp(malloc(strlen(dircopy)+10), "indexer malloc failed");
    strcpy(pagedir, dircopy);
    strcat(pagedir, ".crawler");
    FILE *dirfile = fopen(pagedir, "r");
    if (dirfile == NULL) {
        fprintf(stderr, "Please provide a valid crawler-generated directory\n");
        free(pagedir);
        return 1;
    }
    free(pagedir);
    fclose(dirfile);

    // Checking validity of page directory (existing & writable)
    if (!isDirectory(dircopy)) {
        free(dircopy);
        return 1;
    }

    // Create memory in heap
    char* fileCopy = assertp(malloc(strlen(argv[2])+1), "file malloc failed");
    strcpy(fileCopy, argv[2]);
    FILE* fp = fopen(fileCopy, "w");
    
    if (fp == NULL) {
        fprintf(stderr, "Directory either does not exist or is not writable\n");
        free(fileCopy);
        free(dircopy);
        return 1;
    }

    // Build and save index
    index_t* index = index_build(dircopy);
    index_save(index, fileCopy);
    index_delete(index);
    free(dircopy);
    free(fileCopy);
    fclose(fp);
    return 0;
}
