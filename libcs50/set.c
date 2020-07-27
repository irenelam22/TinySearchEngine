/* 
 * set.c - maintains an unordered collection of (key,item) pairs;
 * any given key can only occur in the set once. It starts out empty 
 * and grows as the caller inserts new (key,item) pairs.  The caller 
 * can retrieve items by asking for their key, but cannot remove or 
 * update pairs.  Items are distinguished by their key.
 * 
 * see set.h and README.md for more information.
 *
 * Irene Lam, July 18, 2020
 * CS50, Summer 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "set.h"

/**************** global types ****************/
struct set_node {
    void* set_item;
    char* set_key;
    struct set_node* next;
};  

typedef struct set {
    struct set_node* head;
} set_t;  

/**************** functions ****************/

/**************** set_new ****************/
/* Create a new (empty) set.
 * 
 * We return:
 *   pointer to a new set, or NULL if error.
 * We guarantee:
 *   The set is initialized empty.
 * Caller is responsible for:
 *   later calling set_delete.
 */
set_t *set_new(void) 
{
    set_t* new_set = calloc(1, sizeof(set_t));
    return new_set;
}

/**************** set_insert ****************/
/* Insert item, identified by a key (string), into the given set.
 *
 * Caller provides:
 *   valid set pointer, valid string pointer, and pointer to item.
 * We return:
 *  false if key exists, any parameter is NULL, or error;
 *  true iff new item was inserted.
 * Caller is responsible for:
 *   later calling set_delete to free the memory used by key strings.
 * Notes:
 *   The key string is copied for use by the set; that is, the module
 *   is responsible for allocating memory for a copy of the key string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its key string after this call.  
 */
bool set_insert(set_t *set, const char *key, void *item)
{
    // Error-handling
    if ( set == NULL || key == NULL || item == NULL ) {
        return false;
    }
    // Check whether a node of the same key already exists
    struct set_node* ptr = set -> head;
    while (ptr != NULL) {
        if (strcmp(ptr -> set_key, key) == 0) {
            return false;
        }
        ptr = ptr -> next;
    }
    // Check whether there is sufficient memory allocated
    struct set_node* new_node = calloc(1, sizeof(struct set_node));
    if (new_node == NULL) {
        return false;
    }
    // Insert the new node containing the (key, item) pair at the head
    new_node -> set_key = calloc(1, strlen(key) + 1);
    if (new_node -> set_key == NULL) {
        free(new_node);
        return false;
    }

    strcpy(new_node -> set_key, key);
    new_node -> set_item = item;
    new_node -> next = set -> head;
    set -> head = new_node;
    return true;
}

/**************** set_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid set pointer, valid string pointer.
 * We return:
 *   a pointer to the desired item, if found; 
 *   NULL if set is NULL, key is NULL, or key is not found.
 * Notes:
 *   The item is *not* removed from the set.
 *   Thus, the caller should *not* free the pointer that is returned.
 */
void *set_find(set_t *set, const char *key)
{
    // Error-handling
    if (set == NULL || key == NULL) {
        return NULL;
    }

    // Find the node by iterating from the head
    struct set_node* ptr = set -> head;
    while (ptr != NULL) {
        if (strcmp(ptr -> set_key, key) == 0) {
            return ptr -> set_item;
        }
        ptr = ptr -> next;
    }

    // Return NULL if a node with the given key could not be found
    return NULL;
}

/**************** set_print ****************/
/* Print the whole set; provide the output file and func to print each item.
 *
 * Caller provides:
 *   valid set pointer,
 *   FILE open for writing,
 *   valid pointer to function that prints one item.
 * We print:
 *   nothing if NULL fp. Print (null) if NULL set.
 *   print a set with no items if NULL itemprint. 
 *  otherwise, 
 *   print a comma-separated list of items surrounded by {brackets}.
 * Notes:
 *   The set and its contents are not changed.
 *   The 'itemprint' function is responsible for printing (key,item).
 */
void set_print(set_t *set, FILE *fp, 
               void (*itemprint)(FILE *fp, const char *key, void *item) )
{
    // Error-handling when file or set is NULL
    if (fp == NULL) {
        return;
    }
    if (set == NULL) {
        fprintf(fp, "(null)\n");
        return;
    }

    // Iterate through the set 
    struct set_node* ptr = set -> head;
    fprintf(fp, "{");
    while (ptr != NULL) {
        // Print using itemprint if available
        if (itemprint != NULL) {
            (*itemprint)(fp, ptr -> set_key, ptr -> set_item);
        }
        else {
            fprintf(fp, "%s", ptr -> set_key);
        }
        if (ptr -> next != NULL) {
            fprintf(fp, ",");
        }
        ptr = ptr -> next;
    }
    fprintf(fp, "}\n");
    
}

/**************** set_iterate ****************/
/* Iterate over the set, calling a function on each item.
 * 
 * Caller provides:
 *   valid set pointer,
 *   arbitrary argument (pointer) that is passed-through to itemfunc,
 *   valid pointer to function that handles one item.
 * We do:
 *   nothing, if set==NULL or itemfunc==NULL.
 *   otherwise, call the itemfunc on each item, with (arg, key, item).
 * Notes:
 *   the order in which set items are handled is undefined.
 *   the set and its contents are not changed by this function,
 *   but the itemfunc may change the contents of the item.
 */
void set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item) )
{
    // Error-handling
    if (set == NULL || itemfunc == NULL) {
        return;
    }
    // Call itemfunc on each node of the set
    struct set_node* ptr = set -> head;
    while (ptr != NULL) {
        (*itemfunc)(arg, ptr -> set_key, ptr -> set_item);
        ptr = ptr -> next;
    }
}

/**************** set_delete ****************/
/* Delete set, calling a delete function on each item.
 *
 * Caller provides:
 *   valid set pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if set==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, call the itemfunc on each item.
 *   free all the key strings, and the set itself.
 * Notes:
 *   We free the strings that represent key for each item, because 
 *   this module allocated that memory in set_insert.
 */
void set_delete(set_t *set, void (*itemdelete)(void *item) )
{
    // Error-handling
    if (set == NULL) {
        return;
    }
    // Iterate through each node within the set, freeing memory
    for (struct set_node* node = set->head; node != NULL; ) {
        if (itemdelete != NULL && node->set_item != NULL) {
            (*itemdelete)(node->set_item);
        }
        struct set_node* next = node -> next;	    // remember what comes next
        free(node -> set_key);                      // free the key
        free(node);			                        // free the node
        node = next;			                    // and move on to next
    }

    // Free the set
    free(set);
    set = NULL;
}
 
