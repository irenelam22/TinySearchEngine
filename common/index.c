/* 
 * index.c - CS50 'index' module
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

/**************** global types ****************/
typedef hashtable_t index_t;

void updateIndex(char* fp, char* copy, char* num, int i);

index_t* index_new(const int num_slots)
{
    return (index_t*)hashtable_new(num_slots);
}

void* index_find(index_t* index, const char* key)
{
    return (index_t*)hashtable_find(index, key);
}

bool index_insert(index_t* index, const char *key, void *item)
{
    return (index_t*)hashtable_insert(index, key, item);
}

void* item_print(FILE *fp, const char *key, void *item)
{
    fprintf(fp, "%s ", key);
    counters_print((counters_t*)item, fp);
    fprintf(fp, "\n");
}

void index_print(index_t *ht, FILE *fp,
                     void (*itemprint)(FILE *fp, const char *key, void *item))
{
    hashtable_print((hashtable_t*)ht, fp, itemprint);
}

// Add index_print with itemprint

void index_delete(index_t* index, void (*itemdelete)(void *item) )
{
    hashtable_delete(index, itemdelete);
    // write itemdelete
}

/**
 * takes a page dir and populate an index data structure with all pages from directory
    builds index data structure index_t
 */
index_t* index_build(char* pagedir) 
{
    const int tableSize = 20;
    if (pagedir == NULL) {
        return NULL;
    }

    index_t* table = assertp(index_new(tableSize), "index_t failed");

    int i = 1;
    char* result = NULL;
    char* num = NULL;
    char* copy = NULL;
    updateIndex(pagedir, copy, num, i);
    
    FILE* fp;
    while ((fp = fopen(copy, "r")) != NULL) {
        if (lines_in_file(fp) < 3) {
            i++;
            updateIndex(pagedir, copy, num, i);
            continue;
        }
        char* url = freadlinep(fp);
        char* depth = freadlinep(fp);
        char* html = freadfilep(fp);
        webpage_t* page = assertp(webpage_new(url, atoi(depth), html), "webpage_new failed");
        if (page == NULL) {
            free(url);
            free(depth);
            free(html);
            break;
        }
        int pos = 0; 
        // mapping from *words* to (documentID, count) pairs
        while ((result = webpage_getNextWord(page, &pos)) != NULL) {
            if (strlen(result) < 3) {
                free(result);
                continue;
            }

            normalizeWord(result);
            counters_t* set = index_find(table, result);
            if (set == NULL) {
                set = assertp(counters_new(), "counters_new failed");
                
                counters_add(set, i);
                if(!index_insert(table, result, set)) {
                    fprintf(stderr, "Failed to insert %s", result);
                }
            }
            else {
                counters_add(set, i);
            }
            
            free(result);
        }
        free(url);
        free(depth);
        free(html);

        free(copy);
        free(num);
        i++;
        updateIndex(pagedir, copy, num, i);

    }
    
    return table;
}
void updateIndex(char* fp, char* copy, char* num, int i) 
{  
    copy = assertp(malloc(strlen(fp) +10), "file copy malloc failed");
    strcpy(copy, fp);
    num = assertp(malloc(sizeof(i)+1), "index_load ID");
    sprintf(num, "%d", i);
    strcat(copy, num);
}


void index_save(index_t* index, char* filename)
{
    if (index == NULL || filename == NULL) {
        fprintf(stderr, "invalid input to index_save");
        return;
    }

    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        fprintf(stderr, "index_save file would not open");
        return;
    }

    index_print(index, fp, item_print);
}

index_t* index_load(char* filename) 
{
    index_t* index = index_new(10);
    if (index == NULL) {
        fprintf(stderr, "index_load could not create new index");
        return;
    }
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "could not open index_load file");
        free(fp);
        return;
    }
    // word docID count [docID count]...
    char* line = NULL;
    char delim[] = " ";
    
    while ((line = freadlinep(fp)) != NULL) {
        char* word = strtok(line, delim);
        char* docID;
        char* count;
        counters_t* set = counters_new();
        if (set == NULL) {
            fprintf(stderr, "could not instantiate set");
            free(fp);
            index_delete(index, NULL);
            return;
        }
        while ((docID = strtok(line, delim) && (count = strtok(line,delim))) != NULL) {
            // *words* to *(documentID, count) pairs*
            counters_set(set, docID, count);  
        }
        index_insert(index, word, set);
    }
    free(fp);
    index_delete(index, NULL);
    return index;
}
