/**
 * pagedir.h -- header file for CS50 'index' module
 * 
 * 'index' module contains methods that need to be shared by crawler, 
 * indexer, and querier. More specifically, 'index' helps the indexer 
 * read the documents in a given `directory` outputted by the crawler, 
 * builds an inverted index mapping from words to document ID and count, 
 * and writes that index to a file
 * 
 * Irene Lam, August 7, 2020
 * Dartmouth CS50, Summer 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"


/**************** global types ****************/
typedef hashtable_t index_t;

/**************** functions ****************/

/**************** index_new ****************/
/* Create a new (empty) index_t of the given integer size.
 * Input: integer representing the size of the index
 * Output: initialized and empty index_t
 */
index_t* index_new(const int num_slots);

/**************** index_find ****************/
/* Returns the counters set associated with the given key
 * Input: index, key of set to find
 * Output: corresponding set if found, NULL if hashtable is NULL, 
 *         key is NULL, key is not found
 */
void* index_find(index_t* index, const char* key);

/**************** index_insert ****************/
/* Insert item, identified by key (string), into the given index.
 * Input: counter set item and corresponding key
 * Output: true if the new item was inserted properly, false otherwise
 */
bool index_insert(index_t* index, const char *key, void *item);

/**************** printset ****************/
/* Helper method to print counters set as follows
    docID count [docID count]...
 * Input: filename, docID key, int count
 * Output: None (prints to file)
 */
void printset(void *fp, const int key, const int count);

/**************** item_print ****************/
/* itemprint method to be passed to index_print
 * Prints counters as follows:
        word docID count [docID count]...
 * Input: file, word key, counters set item
 * Output: None (prints to file)
 */
void item_print(void *fp, const char *key, void *item);

/**************** index_print ****************/
/* Prints index where each line is as follows:
    word docID count [docID count]...
 * Input: index, file to print to, itemprint method
 * Output: None (prints to file)
 */
void index_print(index_t *ht, void *fp,
                     void (*itemprint)(void *fp, const char *key, void *item));

/**************** itemdelete ****************/
/* itemdelete method to be passed to index_delete
 * Deletes the counters set within the index
 * Input: counters set (from index)
 * Output: None
 */
void itemdelete(void* item);

/**************** index_delete ****************/
/* Deletes the index and its interior counter set
 * Input: index to be deleted
 * Output: None
 */
void index_delete(index_t* index );

/**************** iprint ****************/
/* For debugging purposes
 * Prints given index to stdout
 * Input: index
 * Output: None (prints to stdout)
 */
void iprint(index_t* index); 

/**************** index_load ****************/
/* Creates an index data structure from a given index file and returns this
 * Input: filename of index file produced from index_save
 * Output: index representation of given file
 */
index_t* index_load(char* filename);

/**************** index_save ****************/
/* Saves the given data structure into a writable file
 * Input: Index data structure, file
 * Output: None
 */
void index_save(index_t* index, char* filename);

/**************** index_build ****************/
/* Builds an index data structure from a given crawler pagedirectory by parsing through
 * the directory for respective word count and document ID
 * Input: directory output from crawler
 * Output: index representation of the file
 */
index_t* index_build(char* pagedir);

/**************** updateIndex ****************/
/* Helper method for index_build
 * Updates the file within the directory for parsing
 * Input: filename, integer tagged to the file
 * Output: new file name (incremented by 1)
 */
char* updateIndex(char* fp, int i);