/* 
 * hashtable.c - CS50 'counters' module
 *
 * see hashtable.h for more information.
 *
 * Irene Lam, July 19, 2020
 * CS50, Summer 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "set.h"
#include "jhash.h" 

/**************** global types ****************/
typedef struct hashtable {
    set_t **table;
    int length;
} hashtable_t;

/**************** functions ****************/

/**************** hashtable_new ****************/
/* Create a new (empty) hashtable.
 *
 * Caller provides:
 *   number of slots to be used for the hashtable (must be > 0).
 * We return:
 *   pointer to the new hashtable; return NULL if error.
 * We guarantee:
 *   hashtable is initialized empty.
 * Caller is responsible for:
 *   later calling hashtable_delete.
 */
hashtable_t *hashtable_new(const int num_slots) 
{   
    // Allocate memory and check whether successful
    set_t** sets = calloc(num_slots, sizeof(set_t*));
    if (sets == NULL) {
        return NULL;
    }
    hashtable_t* h_table = calloc(1, sizeof(hashtable_t));
    if (h_table == NULL) {
        free(sets);
    }

    // Initializing new set for each slot in hashtable
    for (int i = 0; i < num_slots; i++) {
        *(sets+i) = set_new();
    }
    
    h_table -> length = num_slots;
    h_table -> table = sets;
    return h_table;
}

/**************** hashtable_insert ****************/
/* Insert item, identified by key (string), into the given hashtable.
 *
 * Caller provides:
 *   valid pointer to hashtable, valid string for key, valid pointer for item.
 * We return:
 *   false if key exists in ht, any parameter is NULL, or error;
 *   true iff new item was inserted.
 * Notes:
 *   The key string is copied for use by the hashtable; that is, the module
 *   is responsible for allocating memory for a copy of the key string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its key string after this call.  
 */
bool hashtable_insert(hashtable_t *ht, const char *key, void *item) 
{
    // Error-handling
    if (ht == NULL || key == NULL || item == NULL) {
        return false;
    }
    // Insert after hashing
    set_t **ptr = ht -> table;
    unsigned long hash = JenkinsHash(key, ht -> length);
    return set_insert(*(ptr + (int)hash), key, item);
}

/**************** hashtable_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid pointer to hashtable, valid string for key.
 * We return:
 *   pointer to the item corresponding to the given key, if found;
 *   NULL if hashtable is NULL, key is NULL, or key is not found.
 * Notes:
 *   the hashtable is unchanged by this operation.
 */
void *hashtable_find(hashtable_t *ht, const char *key){
    // Error-handling
    if (ht == NULL || key == NULL) {
        return NULL;
    }
    // Find key using hash
    set_t **ptr = ht -> table;
    unsigned long hash = JenkinsHash(key, ht -> length);
    return set_find(*(ptr + hash), key);
    
}

/**************** hashtable_print ****************/
/* Print the whole table; provide the output file and func to print each item.
 * 
 * Caller provides:
 *   valid pointer to hashtable, 
 *   FILE open for writing,
 *   itemprint that can print a single (key, item) pair.
 * We print:
 *   nothing, if NULL fp.
 *   "(null)" if NULL ht.
 *   one line per hash slot, with no items, if NULL itemprint.
 *   otherwise, one line per hash slot, listing (key,item) pairs in that slot.
 * Note:
 *   the hashtable and its contents are not changed by this function,
 */
void hashtable_print(hashtable_t *ht, FILE *fp, 
                     void (*itemprint)(FILE *fp, const char *key, void *item))
{
    // Error-handling
    if (fp == NULL) {
        return;
    }
    if (ht == NULL) {
        fprintf(fp, "(null)\n");
        return;
    }
    // Print using set_print method
    set_t **ptr = ht -> table;
    for (int i = 0; i < ht -> length; i++) {
        set_print(*(ptr + i), fp, itemprint);
    }
}

/**************** hashtable_iterate ****************/
/* Iterate over all items in the table; in undefined order.
 *
 * Caller provides:
 *   valid pointer to hashtable, 
 *   arbitrary void*arg pointer,
 *   itemfunc that can handle a single (key, item) pair.
 * We do:
 *   nothing, if ht==NULL or itemfunc==NULL.
 *   otherwise, call the itemfunc once for each item, with (arg, key, item).
 * Notes:
 *   the order in which hashtable items are handled is undefined.
 *   the hashtable and its contents are not changed by this function,
 *   but the itemfunc may change the contents of the item.
 */
void hashtable_iterate(hashtable_t *ht, void *arg,
                       void (*itemfunc)(void *arg, const char *key, void *item) ) 
{
    // Error-handling
    if (ht == NULL || itemfunc == NULL) {
        return;
    }
    // Iterate using set_iterate function
    set_t **ptr = ht -> table;
    for (int i = 0; i < ht -> length; i++) {
        set_iterate(*(ptr + i), arg, (*itemfunc));
    }

}

/**************** hashtable_delete ****************/
/* Delete hashtable, calling a delete function on each item.
 *
 * Caller provides:
 *   valid hashtable pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if hashtable==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, call the itemfunc on each item.
 *   free all the key strings, and the hashtable itself.
 * Notes:
 *   We free the strings that represent key for each item, because 
 *   this module allocated that memory in hashtable_insert.
 */
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) ) 
{
    // Error-handling
    if (ht == NULL) {
        return;
    }
    // Free each set
    set_t **ptr = ht -> table;
    for (int i = 0; i < ht -> length; i++) {
        set_delete(*(ptr + i), itemdelete);
        *(ptr + i) = NULL;
    }
    
    // Free the table and pointer
    free(ptr);
    ptr = NULL;
    free(ht);
    ht = NULL;
}
