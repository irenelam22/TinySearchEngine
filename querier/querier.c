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

/******** local data types *******/
struct twocts {
	counters_t *temp;
	counters_t *curr;
};

struct row {
    int maxkey;
    int maxcount;
    int* visited;
    int size;
};

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
    // if (!isDirectory(dircopy)) {
    //     free(dircopy);
    //     return false;
    // }

    FILE* fp = fopen(index, "r");
    
    if (fp == NULL) {
        fprintf(stderr, "%s either does not exist or is not readable\n", index);
        return false;
    }

    // Build and save index
    fclose(fp);
    return true;
}

void process_query(index_t* index, char* pagedir)
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
    
    int length = 0;
    printf("Please input your query:\n");
    while ((line = freadlinep(stdin)) != NULL) {
        printf("Query: %s\n", line);
        // char** words = assertp(malloc(8*(strlen(line)+1)), "words did not malloc");
        char** words = calloc(sizeof(char*), strlen(line) + 1);
        length = extract_words(line, words);
        
        if (!valid_query(words, length)) {
            free(words);
            free(line);
            break;
        }

        run_query(words, index, pagedir);
        printf("Please input your query:\n");

        free(words);
        free(line);
    }
    
}
int extract_words(char* line, char** words)
{
    char delim[] = " ";                         // Delimiter for strtok
    char* word = strtok(line, delim);           // Current line for strtok
    if (word == NULL) {
        return 0;
    }
    
    int i = 1;
    normalizeWord(word);
    words[0] = word;
    while ((word = strtok(NULL, delim)) != NULL) {
        normalizeWord(word);
        *(words + i) = word;
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

void sum_iterate(void *arg, const int key, const int count)
{
    counters_t* final = arg;
  
    // find the same key in setA
    int finalcount = counters_get(final, key);
    if (finalcount == 0) {
        counters_set(final, key, count);
    } 
    else {  
        counters_set(final, key, finalcount+count);
    }
}

int min(const int a, const int b) 
{
    return (a < b ? a : b);
}

void min_iterate(void *arg, const int key, const int count)
{
	struct twocts *two = arg; 

	counters_set(two->temp, key, min(count, counters_get(two->curr, key)));
}

void run_query(char** words, index_t* index, char* pagedir) 
{
    counters_t* temp = NULL;
    counters_t* final = assertp(counters_new(), "run_query counters failed");
    int i = 0;
    char* word = NULL;
    while ((word = words[i]) != NULL) {
        if (words[i+1] != NULL && (strcmp(words[i+1], "or") == 0)) { // OR
            if (temp == NULL) {
                temp = index_find(index, word);
            }
            else {
                counters_iterate(temp, index_find(index, word), sum_iterate);
            }
            counters_iterate(temp, final, sum_iterate);
            i+=2;
            temp = NULL;
        }
        else {
            counters_t* curr = index_find(index, word);
            if (strcmp(word, "and") == 0) {
                i++;
            }
            else if (temp == NULL) {
                temp = index_find(index, word);
                i++;
            }
            else { 
                struct twocts args = {temp, curr}; 
                counters_iterate(temp, &args, min_iterate);
                i++;
            }
        }
        // printf("Printing FINAL and TEMP\n");
        // counters_print(final, stderr);
        // counters_print(temp, stderr);
    }
    if (temp != NULL ) {
        counters_iterate(temp, final, sum_iterate);
    }
    selection_sort(final, pagedir);
    counters_delete(final);
    // counters_iterate(final, pageDirectory, query_print);
} 

void query_print(void *arg, const int key, const int count)
{
    // webpage_t* page = arg;
    // mUST PRINT WEBPAGE

    if (count > 0) {
        printf("score %d doc %d:\n", count, key);
    }
}

// arg = countersnode_t
void selection_sort_helper(void *arg, const int key, const int count)
{
    struct row* node = arg;
    for (int i = 0; i < node->size; i++) {
        if (key == node->visited[i]) {
            return;
        }
    }
    if (node->maxcount < count) {
        node->maxcount = count;
        node->maxkey = key;
    }
}

void findLength(void *arg, const int key, const int count)
{
    int* key_count = arg;
    (void)key_count;
    *key_count += 1;
}

void selection_sort(counters_t* set, char* pagedir)
{
    int length = 0;
    counters_iterate(set, &length, findLength);
    int* visited = assertp(calloc(sizeof(int), length), "selection sort calloc failed");
    // maxkey, maxcount, visited, size;
    struct row node = {0, 0, visited, length}; 
    for (int pos = 0; pos < length; pos++) {
        counters_iterate(set, &node, selection_sort_helper);
        visited[pos] = node.maxkey;
        if (node.maxcount > 0) {
            char* num = assertp(malloc(sizeof(node.maxkey)+1), "sort maxkey");
            sprintf(num, "%d", node.maxkey);
            char* urlcopy = assertp(malloc(strlen(pagedir)+1+sizeof(node.maxkey)), "sort urlcopy failed");
            strcpy(urlcopy, pagedir);
            strcat(urlcopy, num);
            FILE* fp = fopen(urlcopy, "r");
            if (fp == NULL) {
                fprintf(stderr, "sort file could not be opened");
            }
            char* url = freadlinep(fp);
            printf("score %d doc %d: %s\n", node.maxcount, node.maxkey, url);
            fclose(fp);
            free(num);
            free(urlcopy);
            free(url);
        }
        else {
            if (pos == 0) {
                printf("No documents match.\n");
            }
            free(visited);
            return;
        }
        node.maxkey = 0;
        node.maxcount = 0;
    }
    free(visited);
}
