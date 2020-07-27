/* 
 * bag.c - CS50 'counters' module, implements a counter set distinguished by
 * an integer key.
 *
 * see counters.h and README.md for more information.
 *
 * Irene Lam, July 18, 2020
 * CS50, Summer 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"


/**************** file-local global variables ****************/
/* none */

/**************** struct types ****************/
typedef struct counterNode {
    int item;                   // integer item
    int counter;                // counter of item
    struct counterNode *next;   // next element of the counter set
} counternode_t;

typedef struct counters {
    counternode_t *head;       // head of the counter set
} counters_t;

/**************** counters_new ****************/
/* Create a new (empty) counter structure.
 *
 * We return:
 *   pointer to a new counterset; NULL if error. 
 * We guarantee:
 *   counterset is intialized empty.
 * Caller is responsible for:
 *   later calling counters_delete();
 */
counters_t *counters_new(void)
{
    counters_t* counterSet = calloc(1, sizeof(counters_t));
    return counterSet;
}

/**************** counters_add ****************/
/* Increment the counter indicated by key.
 * 
 * Caller provides:
 *   valid pointer to counterset, and key(must be >= 0)
 * We return:
 *   the new value of the counter related to the indicated key.
 *   0 on error (if ctrs is NULL or key is negative)
 * We guarantee:
 *   counter values will always be >= 1.
 * We do:
 *  ignore if ctrs is NULL or key is negative.
 *  if the key does not yet exist, create a counter for it and initialize to 1.
 *  if the key does exist, increment its counter by 1.
 */
int counters_add(counters_t *ctrs, const int key)
{
    // Error-checking if invalid key or NULL ctrs
    if (key < 0) { 
        return 0;
    }
    if (ctrs == NULL) {
        return 0;
    }

    // Iterate through the list starting from the head, and increment counter
    counternode_t* ptr = ctrs -> head;
    while (ptr != NULL) {
        // If key is found
        if (ptr -> item == key) {
            ptr -> counter++;
            return ptr -> counter;
        }
        // Otherwise move the pointer along
        ptr = ptr -> next;
    }

    // Adding new (key, counter) to the set if key not found
    counternode_t *new = calloc(1, sizeof(counternode_t));
    if (new == NULL) {
        return 0;
    }
    new -> counter = 1;
    new -> item = key;
    new -> next = ctrs -> head;
    ctrs -> head = new;
    return new -> counter;
}

/**************** counters_get ****************/
/* Return current value of counter associated with the given key.
 *
 * Caller provides:
 *   valid pointer to counterset, and key(must be >= 0)
 * We return:
 *   current value of counter associte with the given key, if present,
 *   0 if ctrs is NULL or if key is not found.   
 * Note:
 *   counterset is unchanged as a result of this call.
 */
int counters_get(counters_t *ctrs, const int key)
{
    // Error-handling if ctrs is NULL
    if (ctrs == NULL) {
        return 0;
    }
    // Iterate through the list to retrieve the counter value of the given key
    counternode_t* ptr = ctrs -> head;
    while (ptr != NULL) {
        if (ptr -> item == key) {
            return ptr -> counter;
        }
        ptr = ptr -> next;
    }
    // Return 0 if key not found
    return 0;
}

/**************** counters_set ****************/
/* Set the current value of counter associated with the given key.
 *
 * Caller provides:
 *   valid pointer to counterset, 
 *   key(must be >= 0), 
 *   counter value(must be >= 0).
 * We return:
 *   false if ctrs is NULL, if key < 0 or count < 0, or if out of memory.
 *   otherwise returns true.
 * We do:
 *   If the key does not yet exist, create a counter for it and initialize to
 *   the given value. 
 *   If the key does exist, update its counter value to the given value.
 */
bool counters_set(counters_t *ctrs, const int key, const int count)
{
    // Error-handling 
    if (ctrs == NULL || key < 0 || count < 0) {
        return false;
    }
    // Iterate down the list to search for the key and set its counter
    counternode_t* ptr = ctrs -> head;
    while (ptr != NULL) {
        if (ptr -> item == key) {
            ptr -> counter = count;
            return true;
        }
        ptr = ptr -> next;
    }

    // If key not found, create a new key within the set with the given counter
    counternode_t *new = calloc(1, sizeof(counternode_t));
    if (new == NULL) {
        return false;
    }
    new -> counter = count;
    new -> item = key;
    new -> next = ctrs -> head;
    ctrs -> head = new;
    return true;
}

/**************** counters_print ****************/
/* Print all counters; provide the output file.
 *
 * Caller provides:
 *   valid pointer to counterset, 
 *   FILE open for writing.
 * We print:
 *   Nothing if NULL fp. 
 *   "(null)" if NULL ctrs.
 *   otherwise, comma=separated list of key=counter pairs, all in {brackets}.
 */
void counters_print(counters_t *ctrs, FILE *fp)
{
    // Error-handling 
    if (fp == NULL) {
        return;
    }
    if (ctrs == NULL) {
        fprintf(fp, "(null)\n");
        return;
    }

    // Print each key,counter pair in a comma separated list, wrapped in brackets
    fprintf(fp, "{");
    counternode_t* ptr = ctrs -> head;
    while (ptr != NULL) {
        if (ptr -> next == NULL) {
            fprintf(fp, "%d=%d}\n", ptr -> item, ptr -> counter);
        } 
        else {
            fprintf(fp, "%d=%d,", ptr -> item, ptr -> counter);
        }
        ptr = ptr -> next;
    }
    
    
}

/**************** counters_iterate ****************/
/* Iterate over all counters in the set.
 *
 * Caller provides:
 *   valid pointer to counterset, 
 *   arbitrary void*arg,
 *   valid pointer to itemfunc that can handle one item.
 * We do:
 *   nothing, if ctrs==NULL or itemfunc==NULL.
 *   otherwise, call itemfunc once for each item, with (arg, key, count).
 * Note:
 *   the order in which items are handled is undefined.
 *   the counterset is unchanged by this operation.
 */
void counters_iterate(counters_t *ctrs, void *arg, 
                      void (*itemfunc)(void *arg, 
                                       const int key, const int count))
{
    // Error-handling
    if (ctrs == NULL || itemfunc == NULL) {
        return;
    }
    // Iterate through the set and call itemfunc for each item
    counternode_t* ptr = ctrs -> head;
    while (ptr != NULL) {
        (*itemfunc)(arg, ptr -> item, ptr -> counter);
        ptr = ptr -> next;
    }
}

/**************** counters_delete ****************/
/* Delete the whole counterset.
 *
 * Caller provides:
 *   a valid pointer to counterset.
 * We do:
 *   we ignore NULL ctrs.
 *   we free all memory we allocate for this counterset.
 */
void counters_delete(counters_t *ctrs)
{
    // Error-handling
    if (ctrs == NULL) {
        return;
    }

    for (counternode_t *node = ctrs->head; node != NULL; ) {
      counternode_t *next = node -> next;	    // remember what comes next
      free(node);			                    // free the node
      node = next;			                    // and move on to next
    }

    // Free the set and set to NULL
    free(ctrs);
    ctrs=NULL;
}
