/**
 * pagedir.c -- initializes pageDirectory directory and stores webpages
 * Please see pagedir.h for more details
 * 
 * Irene Lam, July 27, 2020
 * Dartmouth CS50, Summer 2020
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

bool isDirectory(char* directory)
{
    if (directory == NULL) {
        fprintf(stderr, "directory is NULL\n");
        return false;
    }
    char* copy = assertp(malloc(strlen(directory)+10), "directory malloc failed");
    strcpy(copy, directory);
    if (directory[strlen(directory)-1] != '/') {
        fprintf(stderr, "Please pass directory with a forward slash\n");
        return false;
    }
    
    strcat(copy, ".crawler");

    FILE* fp = fopen(copy, "w");
    free(copy);
    if (fp == NULL) {
        fprintf(stderr, "Directory either does not exist or is not writable");
        return false;
    }
    fclose(fp);
    return true;
}
