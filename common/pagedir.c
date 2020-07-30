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

/**************** isDirectory ****************/
/*
 * Checks whether the input string is a valid, writable directory
 * Input: char*, presumably to a directory
 * Ouput: boolean signifiying whether input is a valid, writable directory
 */
bool isDirectory(char* directory)
{
    // Reject if directory is NULL
    if (directory == NULL) {
        fprintf(stderr, "directory is NULL\n");
        return false;
    }

    // Create memory in heap
    char* copy = assertp(malloc(strlen(directory)+10), "directory malloc failed");
    strcpy(copy, directory);

    // Directory must end in a forward slash
    if (directory[strlen(directory)-1] != '/') {
        fprintf(stderr, "Please pass directory with a forward slash\n");
        return false;
    }
    
    strcat(copy, ".crawler");
    FILE* fp = fopen(copy, "w");
    free(copy);

    // Directory must exist and be writable to
    if (fp == NULL) {
        fprintf(stderr, "Directory either does not exist or is not writable");
        return false;
    }
    fclose(fp);
    return true;
}
