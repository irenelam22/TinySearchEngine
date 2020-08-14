/* 
 * index.c - CS50 'index' module
 *
 * 'index' module contains methods that need to be shared by crawler, 
 * indexer, and querier. More specifically, 'index' helps the indexer 
 * read the documents in a given `directory` outputted by the crawler, 
 * builds an inverted index mapping from words to document ID and count, 
 * and writes that index to a file
 * 
 * see index.h for more information.
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
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"
#include "../libcs50/file.h"
#include "index.h"
#include "word.h"
#include "pagedir.h"

#ifdef TEST
void debug(char* input)
{
    printf("%s", input); 
}
#else 
void debug(char* input) {}
#endif

/**************** global types ****************/
typedef hashtable_t index_t;

/**************** functions ****************/

/**************** index_new ****************/
/* Create a new (empty) index_t of the given integer size.
 * Input: integer representing the size of the index
 * Output: initialized and empty index_t
 */
index_t* index_new(const int num_slots)
{
    return (index_t*)hashtable_new(num_slots);
}

/**************** index_find ****************/
/* Returns the counters set associated with the given key
 * Input: index, key of set to find
 * Output: corresponding set if found, NULL if hashtable is NULL, 
 *         key is NULL, key is not found
 */
void* index_find(index_t* index, const char* key)
{
    return (index_t*)hashtable_find(index, key);
}

/**************** index_insert ****************/
/* Insert item, identified by key (string), into the given index.
 * Input: counter set item and corresponding key
 * Output: true if the new item was inserted properly, false otherwise
 */
bool index_insert(index_t* index, const char *key, void *item)
{
    return (index_t*)hashtable_insert(index, key, item);
}

/**************** printset ****************/
/* Helper method to print counters set as follows
    docID count [docID count]...
 * Input: filename, docID key, int count
 * Output: None (prints to file)
 */
void printset(void *fp, const int key, const int count)
{
    fprintf(fp, " %d %d", key, count);
}

/**************** item_print ****************/
/* itemprint method to be passed to index_print
 * Prints counters as follows:
        word docID count [docID count]...
 * Input: file, word key, counters set item
 * Output: None (prints to file)
 */
void item_print(void *fp, const char *key, void *item)
{
    fprintf(fp, "%s", key);
    counters_iterate((counters_t*)item, fp, printset);
    fprintf(fp, "\n");
}

/**************** index_print ****************/
/* Prints index where each line is as follows:
    word docID count [docID count]...
 * Input: index, file to print to, itemprint method
 * Output: None (prints to file)
 */
void index_print(index_t *ht, void *fp,
                     void (*itemprint)(void *fp, const char *key, void *item))
{
    hashtable_iterate((hashtable_t*)ht, fp, itemprint);
}

/**************** itemdelete ****************/
/* itemdelete method to be passed to index_delete
 * Deletes the counters set within the index
 * Input: counters set (from index)
 * Output: None
 */
void itemdelete(void* item)
{
    counters_delete((counters_t*)item);
}

/**************** index_delete ****************/
/* Deletes the index and its interior counter set
 * Input: index to be deleted
 * Output: None
 */
void index_delete(index_t* index)
{
    hashtable_delete(index, itemdelete);
}

/**************** iprint ****************/
/* For debugging purposes
 * Prints given index to stdout
 * Input: index
 * Output: None (prints to stdout)
 */
void iprint(index_t* index) 
{
    index_print(index, stdout, item_print);
}

/**************** updateIndex ****************/
/* Helper method for index_build
 * Updates the file within the directory for parsing
 * Input: filename, integer tagged to the file
 * Output: new file name (incremented by 1)
 */
char* updateIndex(char* fp, int i) 
{  
    // Allocate memory on the heap and increment document ID
    char* copy = assertp(malloc(strlen(fp) +10), "file copy malloc failed");
    strcpy(copy, fp);
    char* num = assertp(malloc(sizeof(i)+1), "index_load ID");
    sprintf(num, "%d", i);
    strcat(copy, num);
    free(num);
    return copy;
}

/**************** index_build ****************/
/* Builds an index data structure from a given pagedirectory by parsing through
 * the directory for respective word count and document ID
 * Input: directory output from crawler
 * Output: index representation of the file
 */
index_t* index_build(char* pagedir) 
{
    debug("Running index_build...\n");
    const int tableSize = 200;
    if (pagedir == NULL) {
        return NULL;
    }

    // Instantiate new index_t* and initialize variables
    index_t* table = assertp(index_new(tableSize), "index_t failed");

    int i = 1;
    char* result = NULL;
    char* copy = updateIndex(pagedir, i);
    
    // While the file is readable
    FILE* fp;
    while ((fp = fopen(copy, "r")) != NULL) {
        debug("Trying to read ");
        debug(copy);
        debug("\n");
        // Ignore words with less than three characters
        if (lines_in_file(fp) < 3) {
            i++;
            free(copy);
            copy = updateIndex(pagedir, i);
            fclose(fp);
            continue;
        }

        // Record url, depth, html, and new webpage
        char* url = freadlinep(fp);
        char* depth = freadlinep(fp);
        char* html = freadfilep(fp);
        webpage_t* page = assertp(webpage_new(url, atoi(depth), html), "webpage_new failed");
        if (page == NULL) {
            free(url);
            free(depth);
            free(html);
            fclose(fp);
            break;
        }

        // Keep track of current page number
        int pos = 0; 

        // Read through each word within the given webpage
        while ((result = webpage_getNextWord(page, &pos)) != NULL) {
            if (strlen(result) < 3) {
                free(result);
                continue;
            }
            normalizeWord(result);
            // debug("Parsing through word ");
            // debug(result);
            // debug("\n");

            // Map from words to (documentID, count)
            counters_t* set = index_find(table, result);
            
            // Create a new counters set if it doesn't already exist
            if (set == NULL) {
                set = assertp(counters_new(), "counters_new failed");
                
                counters_add(set, i);
                if(!index_insert(table, result, set)) {
                    fprintf(stderr, "Failed to insert %s\n", result);
                }
            }
            // Increment the word count if the set already exists
            else {
                counters_add(set, i);
            }
            
            free(result);
        }

        // Free and close file as unused
        fclose(fp);
        free(depth);
        webpage_delete(page);
        
        i++;
        free(copy);
        copy = updateIndex(pagedir, i);
    }
    free(copy);
    debug("Finished building index.\n");
    return table;
}

/**************** index_save ****************/
/* Saves the given data structure into a writable file
 * Input: Index data structure, file
 * Output: None
 */
void index_save(index_t* index, char* filename)
{
    debug("Trying to save file...\n");
    // Error-handling
    if (index == NULL || filename == NULL) {
        fprintf(stderr, "invalid input to index_save\n");
        return;
    }

    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "index_save file would not open\n");
        return;
    }

    // Print index to given file
    index_print(index, fp, item_print);
    fclose(fp);
    debug("Finished saving file.\n");
}

/**************** index_load ****************/
/* Creates an index data structure from a given index file and returns this
 * Input: filename of index file produced from index_save
 * Output: index representation of given file
 */
index_t* index_load(char* filename) 
{
    // Error-handling
    index_t* index = index_new(10);
    if (index == NULL) {
        fprintf(stderr, "index_load could not create new index\n");
        return NULL;
    }
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "could not open index_load file\n");
        return NULL;
    }

    debug("Trying to load file...\n");
    char* line = NULL;          // Current line for strtok
    char delim[] = " ";         // Delimiter for strtok

    /* As we are reading the output from index_save, lines are of the form
     word docID count [docID count]... */
    while ((line = freadlinep(fp)) != NULL) {
        // Update and initialize variables
        char* word = strtok(line, delim);
        char* docID = NULL;
        char* count = NULL;
        int intCount = 0;
        int intID = 0;
        counters_t* set = counters_new();
        if (set == NULL) {
            fprintf(stderr, "could not instantiate set\n");
            fclose(fp);
            free(line);
            index_delete(index);
            return NULL;
        }
        // debug("Reading word ");
        // debug(word);
        // debug("\n");
        // While another docID and corresponding count exists on the given line
        while (((docID = strtok(NULL, delim)) != NULL) && ((count = strtok(NULL,delim)) != NULL)) {
            intCount = atoi(count);
            intID = atoi(docID);

            // Set the (documentID, count) pairs of the given word
            counters_set(set, intID, intCount);  
        }
        index_insert(index, word, set);

        free(line);
    }
    fclose(fp);
    debug("Finished loading file.\n");
    return index;
}
