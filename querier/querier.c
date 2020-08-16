/* 
 * querier.c - driver for 'querier' module
 *
 * The querier reads the documents in a given `directory` outputted by the 
 * crawler and the index file produced by indexer, to interactively answer 
 * written queries entered by the user
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

/*************** twocts ***************
 * stores two counter sets, used during min_iterate to minimize
 * across the counter sets
 */
struct twocts {
	counters_t *temp;       // temporary counters set being written to
	counters_t *curr;       // current counters set being traversed
};

/*************** row ***************
 * used during selection sort to rank the outputs by count
 * stores the key corresponding to the running maximum count, the running
 * maximum count, an array of visited docIDs, and the size of the 
 * list being traversed
 */
struct row {
    int maxkey;         // key corresponding to the running max count
    int maxcount;       // running max count
    int* visited;       // array of visited docIDs
    int size;           // size
};

/************** main ***************
 * Main method runs the program
 * Inputs:
 * pageDirectory: the pathname of a directory produced by the Crawler, and
 * indexFilename: the pathname of a file produced by the Indexer.
 * Output: 0 if successful, 1 otherwise
 * 
 * Usage: ./querier pageDirectory indexFilename
 */ 
int main(int argc, char* argv[])
{
    // Error-handling
    if (argc < 3) {
        fprintf(stderr, "Insufficient number of arguments\n");
        return 1;
    }
    if (argc > 3) {
        fprintf(stderr, "Too many arguments provided\n");
        return 1;
    }
    char* dircopy = assertp(malloc(strlen(argv[1])+10), "dir copy");
    strcpy(dircopy, argv[1]);

    // Add a backslash to the end of the directory if needed
    if (dircopy[strlen(dircopy)-1] != '/') {
        strcat(dircopy, "/");
    }

    // Validate inputs
    if (!inputCheck(dircopy, argv[2])) {
        return 1;
    }

    // Load index and process query
    index_t* index = index_load(argv[2]);
    process_query(index, dircopy);

    // Clean up
    free(dircopy);
    index_delete(index);
    return 0;
}

/************** inputCheck ***************
 * Checks and validates the parameters
 * Inputs:
 * dircopy: a directory produced by the Crawler, and
 * index: the pathname of a file produced by the Indexer.
 * Output: true if successful, false otherwise 
 */ 
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

    // Check that the index file is readable and existing
    FILE* fp = fopen(index, "r");
    
    if (fp == NULL) {
        fprintf(stderr, "%s either does not exist or is not readable\n", index);
        return false;
    }

    fclose(fp);
    return true;
}

/************** process_query ***************
 * Processes the query by reading each line from stdin
 * Inputs:
 * index: loaded index data structure
 * pagedir: crawler-generated directory
 * Output: none 
 */
void process_query(index_t* index, char* pagedir)
{
    // Instantiate variables
    char* line = NULL;
    int length = 0;
    
    printf("Please input your query:\n");
    // Read lines from stdin 
    while ((line = freadlinep(stdin)) != NULL) {
        // Extract words from the line and normalize content
        char** words = calloc(sizeof(char*), strlen(line) + 1);
        length = extract_words(line, words);
        printf("Query:");
        for (int i = 0; i < length; i++) {
            printf(" %s", words[i]);
        }
        printf("\n");

        // Error-handling: ensure all words are valid
        if (!valid_query(words, length)) {
            free(words);
            free(line);
            continue;
        }

        run_query(words, index, pagedir);
        printf("Please input your query:\n");

        // Clean up
        free(words);
        free(line);
    }
    
}

/************** extract_words ***************
 * Extracts words from a line and normalizes it
 * Determines words based on space delimiter
 * Returns the length of the line (number of words extracted)
 * Inputs: line, array of words
 * Output: integer representing the number of words in the line 
 */
int extract_words(char* line, char** words)
{
    char delim[] = " ";                         // Delimiter for strtok
    char* word = strtok(line, delim);           // Current line for strtok
    if (word == NULL) {
        return 0;
    }
    
    int i = 1;                                  // Number of words

    // Normalize each word and insert the word into the words array
    normalizeWord(word);
    words[0] = word;
    while ((word = strtok(NULL, delim)) != NULL) {
        normalizeWord(word);
        *(words + i) = word;
        i++;
    }
    return i;
}

/************** valid_query ***************
 * Validate words in query. Must satisfy the below conditions
 *     - No conjunctions in the beginning or end
 *     - No consecutive conjunctions
 *     - One or more words
 *     - No non-letter characters (e.g. -,!,.,=)
 * Inputs:
 * query: array of words
 * length: number of words in query
 * Output: true if words satisfy the above conditions, false otherwise 
 */
bool valid_query(char** query, int length)
{
    char* adjacent = NULL;          // Previous conjunction (if any)
    char* currWord = NULL;          // Current conjunction (if any)

    // Do nothing if the query is empty
    if (length == 0) {
        return false;  
    }
    
    // Ensure conjunctions are not first in the query
    if ((strcmp(query[0], "and") == 0) || (strcmp(query[0], "or") == 0)) {
        fprintf(stderr, "Error: '%s' cannot be first\n", query[0]);
        return false;
    }

    // Ensure conjunctions are not last in the query
    if ((strcmp(query[length-1], "and") == 0) || (strcmp(query[length-1], "or") == 0)) {
        fprintf(stderr, "Error: '%s' cannot be last\n", query[length-1]);
        return false;
    }
    
    // Ensure no two conjunctions are consecutive
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
        // Ensure there are no non-letter characters within each word in the query
        for (int j = 0; j < strlen(currWord); j++) {
            if (isalpha(currWord[j]) == 0) {
                fprintf(stderr, "Error: bad character '%c' in query\n", currWord[j]);
                return false;
            }
        }
    }
    return true;
}

/************** sum_iterate ***************
 * Used for OR conjunctions
 * Adds the count based on a given key (docID)
 * Note: this code was provided by Professor Zhou, I claim no ownership
 * for the following method
 * Inputs:
 * arg: counters set to be changed
 * key: docID
 * count: count of word for the given docID
 * Output: none (directly modifies the input counters set) 
 */
void sum_iterate(void *arg, const int key, const int count)
{
    counters_t* final = arg;
  
    // find the same key and update the count accordingly
    int finalcount = counters_get(final, key);
    if (finalcount == 0) {
        counters_set(final, key, count);
    } 
    else {  
        counters_set(final, key, finalcount+count);
    }
}

/************** min ***************
 * Used for AND conjunctions
 * Finds the minimum of two integers
 * Note: this code was provided by Professor Zhou, I claim no ownership
 * for the following method
 * Inputs: two integers (a and b)
 * Output: the smaller of the two input integers
 */
int min(const int a, const int b) 
{
    return (a < b ? a : b);
}

/************** min_iterate ***************
 * Used for AND conjunctions
 * Minimizes the count based on a given key (docID)
 * Note: this code was provided by Professor Zhou, I claim no ownership
 * for the following method
 * Inputs:
 * arg: counters set to be changed
 * key: docID
 * count: count of word for the given docID
 * Output: none (directly modifies the input counters set) 
 */
void min_iterate(void *arg, const int key, const int count)
{
	struct twocts *two = arg;
	counters_set(two->temp, key, min(count, counters_get(two->curr, key)));
}

/************** run_query ***************
 * Runs the query and considers AND and OR precedence
 * Note: I understand this method is a bit long, but everything in the method
 * is highly connected, hence why I chose to keep AND and OR in one method
 * Inputs:
 * words: array of words from stdin
 * index: index file
 * pagedir: crawler directory
 * Output: none (directly prints changes to stdout/stderr) 
 */
void run_query(char** words, index_t* index, char* pagedir) 
{
    // Instantiate temporary and final counters set 
    counters_t* temp = NULL;        
    counters_t* final = assertp(counters_new(), "run_query counters failed");
    int i = 0;
    char* word = NULL;
    bool empty_create = false;

    // For each word parsed from stdin
    while ((word = words[i]) != NULL) {
        // If the next word is OR...
        if (words[i+1] != NULL && (strcmp(words[i+1], "or") == 0)) { 
            // Merge in the current word to the existing AND query if possible
            if (temp == NULL) {
                temp = index_find(index, word);
            }
            else {
                counters_t* curr = index_find(index, word);
                struct twocts args = {curr, temp}; 
                counters_iterate(curr, &args, min_iterate);
            }
            // Combine the current AND clause into the final counters set
            counters_iterate(temp, final, sum_iterate);
            i+=2;

            if (empty_create) {
                counters_delete(temp);
            }
            temp = NULL;
        }
        // Otherwise merge into temporary AND counters set
        else {
            counters_t* curr = index_find(index, word);
            if (strcmp(word, "and") == 0) {
                i++;
            }
            else if (temp == NULL) {
                // If the word does not exist in the index, reinitialize temp
                temp = index_find(index, word);
                if (temp == NULL) {
                    empty_create = true;
                    temp = counters_new();
                }
                i++;
            }
            else { 
                // Minimize counts across AND sequence
                struct twocts args = {curr, temp}; 
                counters_iterate(curr, &args, min_iterate);
                i++;
            }
        }
    // Merge temp into final if possible
    }
    if (temp != NULL ) {
        counters_iterate(temp, final, sum_iterate);
        if (empty_create) {
            counters_delete(temp);
        }
    }
    // Print the sorted final set and clean up
    selection_sort(final, pagedir);
    counters_delete(final);
} 

/************** selection_sort_helper ***************
 * Helper method for selection sort, used for printing the counts
 * in sorted order (descending)
 * Finds the running maximum count (not part of visited)
 * and updates its respective key
 * Inputs:
 * arg: row of the counters set we are iterating through
 * key: docID
 * count: count of word for the given docID
 * Output: none (directly modifies the input counters set) 
 */
void selection_sort_helper(void *arg, const int key, const int count)
{
    struct row* node = arg;
    // Ensure that the key has not yet been visited
    for (int i = 0; i < node->size; i++) {
        if (key == node->visited[i]) {
            return;
        }
    }
    // Update the running max key and count if the current count is higher
    if (node->maxcount < count) {
        node->maxcount = count;
        node->maxkey = key;
    }
}

/************** findLength ***************
 * Finds the number of non-zero counts in a given counters set
 * Inputs:
 * arg: integer reference
 * key: docID
 * count: count for the given docID
 * Output: none (directly modifies the reference) 
 */
void findLength(void *arg, const int key, const int count)
{
    int* key_count = arg;
    if (count > 0) {
        *key_count += 1;
    }
}

/************** selection_sort ***************
 * Prints the counters set in descending order based on counts
 * Inputs:
 * set: counters set to be printed in descending order
 * pagedir: crawler directory
 * Output: none (directly prints the input counters set) 
 */
void selection_sort(counters_t* set, char* pagedir)
{
    // Find the number of docIDs that match stdin
    int length = 0;
    counters_iterate(set, &length, findLength);
    if (length == 0) {
        printf("No documents match.\n");
        return;
    }

    int* visited = assertp(calloc(sizeof(int), length), "selection sort calloc failed");
    printf("Matches %d documents (ranked):\n", length);
    struct row node = {0, 0, visited, length}; 

    // For each word in the counters set
    for (int pos = 0; pos < length; pos++) {
        counters_iterate(set, &node, selection_sort_helper);
        visited[pos] = node.maxkey;
        if (node.maxcount > 0) {
            // Find the corresponding URL within the crawler directory
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
            // Print the score, docID, and URL for the matched result
            printf("score   %d doc   %d: %s\n", node.maxcount, node.maxkey, url);

            // Clean up
            fclose(fp);
            free(num);
            free(urlcopy);
            free(url);
        }
        node.maxkey = 0;
        node.maxcount = 0;
    }
    free(visited);
}
