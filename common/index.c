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

/**************** global types ****************/
typedef hashtable_t index_t;

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

void index_delete(index_t* index, void (*itemdelete)(void *item) )
{
    hashtable_delete(index, itemdelete);
}

index_t* index_load(char* filename) 
{
    const int tableSize = 20;
    if (filename == NULL) {
        return NULL;
    }

    index_t* table = index_new(tableSize);

    int i = 1;
    char* result;
    char* num = assertp(malloc(sizeof(i)+1), "index_load ID");
    sprintf(num, "%d", i);
    char* copy = assertp(malloc(strlen(filename) +10), "file copy malloc failed");
    strcpy(copy, filename);
    strcat(copy, num);
    
    FILE* fp;
    while ((fp = fopen(copy, "r")) != NULL) {
        if (lines_in_file(fp) < 3) {
            continue;
        }
        char* url = freadlinep(fp);
        char* depth = freadlinep(fp);
        char* html = freadfilep(fp);
        webpage_t* page = webpage_new(url, atoi(depth), html);
        if (page == NULL) {
            break;
        }
        int pos = 0;
        // mapping from *words* to (documentID, count) pairs
        while ((result = webpage_getNextWord(page, &pos)) != NULL) {
            counters_t* set = hashtable_find(table, result);
            if (set == NULL) {
                set = counters_new();
                counters_add(set, i);
                hashtable_insert(table, result, set);
            }
            else {
                counters_add(set, i);
            }
            
            free(result);
        }
        free(url);
        free(depth);
        free(html);

        i++;
        free(copy);
        free(num);
        copy = assertp(malloc(strlen(filename) +10), "file copy malloc failed");
        strcpy(copy, filename);
        num = assertp(malloc(sizeof(i)+1), "index_load ID");
        sprintf(num, "%d", i);
        strcat(copy, num);

    }
    free(copy);
    free(num);
    
    return table;
}

void index_save(index_t* index, char* filename)
{

}