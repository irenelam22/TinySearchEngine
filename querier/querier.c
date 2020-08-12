/* 
 * querier.c - driver for `querier` module
 * 
 * see IMPLEMENTATION and DESIGN spec for more information.
 *
 * Irene Lam, August 11, 2020
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
#include "../common/word.h"
#include "querier.h"

// Function prototype
// bool inputCheck(char* dircopy, char* index);
// int extract_words(char* line, char** words );
// bool valid_query(char** query, int length);

bool inputCheck(char* dircopy, char* index) 
{
    // Check if the given directory is a valid, crawler-generated directory
    char* pagedir = assertp(malloc(strlen(dircopy)+10), "indexer malloc failed");
    strcpy(pagedir, dircopy);
    strcat(pagedir, ".crawler");
    FILE *dirfile = fopen(pagedir, "r");
    if (dirfile == NULL) {
        fprintf(stderr, "Please provide a valid crawler-generated directory\n");
        free(pagedir);
        free(dircopy);
        return false;
    }
    free(pagedir);
    fclose(dirfile);

    // Checking validity of page directory (existing & writable)
    if (!isDirectory(dircopy)) {
        free(dircopy);
        return false;
    }
    free(dircopy);
    // Create memory in heap

    FILE* fp = fopen(index, "r");
    
    if (fp == NULL) {
        fprintf(stderr, "%s either does not exist or is not readable\n", index);
        return false;
    }

    // Build and save index
    fclose(fp);
    return true;
}

void process_query(index_t* index)
{
    /*
    3. read search queries from stdin, one per line, until EOF.
	4. clean and parse each query according to the *syntax* described below.
	5. if the query syntax is somehow invalid, print an error message, do not perform the query, and prompt for the next query.
	5. print the 'clean' query for user to see.
	5. use the index to identify the set of documents that *satisfy* the query, as described below.
	6. if the query is empty (no words), print nothing.
	6. if no documents satisfy the query, print `No documents match.`
	7. otherwise, rank the resulting set of documents according to its *score*, as described below, and print the set of documents in decreasing rank order; for each, list the score, document ID and URL.
    (Obtain the URL by reading the first line of the relevant document file from the `pageDirectory`.)
	8. Exit with zero status when EOF is reached on stdin.
    */
   char* line = NULL;
   char** words = assertp(malloc(8*(strlen(line)+1)), "words did not malloc");
   int length = 0;
   while ((line = freadlinep(stdin)) != NULL) {
       length = extract_words(line, words);
       if (!valid_query(words, length)) {
           break;
       }
   }
    
}
int extract_words(char* line, char** words )
{
    char* word = NULL;          // Current line for strtok
    char delim[] = " ";         // Delimiter for strtok
    int i = 0;
    while ((word = strtok(NULL, delim)) != NULL) {
        words[i] = normalizeWord(word);
        i++;
    }
    return i;
}

bool valid_query(char** query, int length)
{
    char* adjacent = NULL;
    char* currWord = NULL;
    if (length > 0) {
        if ((strcmp(query[0], "and") == 0) || (strcmp(query[0], "or") == 0)) {
            fprintf(stderr, "Error: %s cannot be first\n", query[0]);
            return false;
        }
    }
    for (int i = 0; i < length; i++) {
        currWord = query[i];
        if (adjacent != NULL) {
            if ((strcmp(currWord, "and") == 0) || (strcmp(currWord, "or") == 0)) {
                fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent\n", adjacent, currWord);
                return false;
            }
            else {
                adjacent = NULL;
            }
        } 
        else {
            if ((strcmp(currWord, "and") == 0) || (strcmp(currWord, "or") == 0)) {
                adjacent = currWord;
            }
        }
        for (int j = 0; j < strlen(currWord); j++) {
            if (isalpha(currWord[j]) == 0) {
                fprintf(stderr, "Error: bad character '%c' in query\n", currWord[j]);
                return false;
            }
        }
    }
    return true;
}