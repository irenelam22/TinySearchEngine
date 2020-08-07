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

char* updateIndex(char* fp, int i);

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

void printset(void *fp, const int key, const int count)
{
    fprintf(fp, " %d %d", key, count);
}

void item_print(void *fp, const char *key, void *item)
{
    fprintf(fp, "%s", key);
    counters_iterate((counters_t*)item, fp, printset);
    fprintf(fp, "\n");
}

void index_print(index_t *ht, void *fp,
                     void (*itemprint)(void *fp, const char *key, void *item))
{
    hashtable_iterate((hashtable_t*)ht, fp, itemprint);
}

void itemdelete(void* item)
{
    counters_delete((counters_t*)item);
}

void index_delete(index_t* index)
{
    hashtable_delete(index, itemdelete);
}

void iprint(index_t* index) 
{
    index_print(index, stdout, item_print);
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
    char* copy = updateIndex(pagedir, i);
    
    FILE* fp;
    while ((fp = fopen(copy, "r")) != NULL) {
        if (lines_in_file(fp) < 3) {
            i++;
            free(copy);
            copy = updateIndex(pagedir, i);
            fclose(fp);
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
            fclose(fp);
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

        fclose(fp);
        free(depth);
        webpage_delete(page);
        
        i++;
        free(copy);
        copy = updateIndex(pagedir, i);
    }
    free(copy);
    return table;
}

char* updateIndex(char* fp, int i) 
{  
    char* copy = assertp(malloc(strlen(fp) +10), "file copy malloc failed");
    strcpy(copy, fp);
    char* num = assertp(malloc(sizeof(i)+1), "index_load ID");
    sprintf(num, "%d", i);
    strcat(copy, num);
    free(num);
    return copy;
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
    fclose(fp);
}

index_t* index_load(char* filename) 
{
    index_t* index = index_new(10);
    if (index == NULL) {
        fprintf(stderr, "index_load could not create new index");
        return NULL;
    }
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "could not open index_load file");
        free(fp);
        return NULL;
    }
    // word docID count [docID count]...
    char* line = NULL;
    char delim[] = " ";
    
    while ((line = freadlinep(fp)) != NULL) {
        char* word = strtok(line, delim);
        char* docID = NULL;
        char* count = NULL;
        int intCount = 0;
        int intID = 0;
        counters_t* set = counters_new();
        if (set == NULL) {
            fprintf(stderr, "could not instantiate set");
            free(fp);
            index_delete(index);
            return NULL;
        }
        while (((docID = strtok(NULL, delim)) != NULL) && ((count = strtok(NULL,delim)) != NULL)) {
            intCount = atoi(count);
            intID = atoi(docID);
            // *words* to *(documentID, count) pairs*
            counters_set(set, intID, intCount);  
        }
        index_insert(index, word, set);
    }
    free(fp);
    return index;
}
