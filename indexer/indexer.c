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

// Function prototypes

// ./indexer pageDirectory indexFilename
int main(int argc, char* argv[])
{
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

    // Checking validity of page directory (existing & writable)
    if (!isDirectory(dir)) {
        return 1;
    }

    // Create memory in heap
    char* fileCopy = assertp(malloc(strlen(argv[2])+1), "file malloc failed");
    strcpy(fileCopy, argv[2]);
    FILE* fp = fopen(fileCopy, "w");
    free(fileCopy);
    
    if (fp == NULL) {
        fprintf(stderr, "Directory either does not exist or is not writable\n");
        return 1;
    }

    indexer(dircopy, fileCopy);

    fclose(fp);
    

    return 0;
}

bool inputCheck(char* dir, char* pathToFile) {
    if (dir == NULL || pathToFile == NULL) {
        return false;
    }

    // Checking validity of page directory (existing & writable)
    if (!isDirectory(dir)) {
        return false;
    }

    

    return true;
}

index_t* indexer(char* dir, char* filename) 
{
    index_t* index = index_build(dir);
}