/* 
 * querier.h - header file for querier
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

#ifndef __QUERIER_H
#define __QUERIER_H

#include <stdio.h>
#include <stdbool.h> 
#include "../common/index.h"

// Function prototypes
/************** inputCheck ***************
 * Checks and validates the parameters
 * Inputs:
 * dircopy: a directory produced by the Crawler, and
 * index: the pathname of a file produced by the Indexer.
 * Output: true if successful, false otherwise 
 */ 
bool inputCheck(char* dircopy, char* index);

/************** extract_words ***************
 * Extracts words from a line and normalizes it
 * Determines words based on space delimiter
 * Returns the length of the line (number of words extracted)
 * Inputs: line, array of words
 * Output: integer representing the number of words in the line 
 */
int extract_words(char* line, char** words );

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
bool valid_query(char** query, int length);

/************** process_query ***************
 * Processes the query by reading each line from stdin
 * Inputs:
 * index: loaded index data structure
 * pagedir: crawler-generated directory
 * Output: none 
 */
void process_query(index_t* index, char* pagedir);

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
void sum_iterate(void *arg, const int key, const int count);

/************** min_iterate ***************
 * Used for AND conjunctions
 * Finds the minimum of two integers
 * Note: this code was provided by Professor Zhou, I claim no ownership
 * for the following method
 * Inputs: two integers (a and b)
 * Output: the smaller of the two input integers
 */
int min(const int a, const int b);

/************** min_iterate ***************
 * Used for AND conjunctions (andsequences)
 * Minimizes the count based on a given key (docID)
 * Note: this code was provided by Professor Zhou, I claim no ownership
 * for the following method
 * Inputs:
 * arg: counters set to be changed
 * key: docID
 * count: count of word for the given docID
 * Output: none (directly modifies the input counters set) 
 */
void min_iterate(void *arg, const int key, const int count);

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
void run_query(char** words, index_t* index, char* url) ;

/************** selection_sort ***************
 * Prints the counters set in descending order based on counts
 * Inputs:
 * set: counters set to be printed in descending order
 * pagedir: crawler directory
 * Output: none (directly prints the input counters set) 
 */
void selection_sort(counters_t* set, char* url);

/************** findLength ***************
 * Finds the number of non-zero counts in a given counters set
 * Inputs:
 * arg: integer reference
 * key: docID
 * count: count for the given docID
 * Output: none (directly modifies the reference) 
 */
void findLength(void *arg, const int key, const int count);

/************** selection_sort_helper ***************
 * Helper method for selection sort, used for printing the counts
 * in sorted order (descending)
 * Finds the running maximum count (not part of the visited set of docIDs)
 * and updates its respective key
 * Inputs:
 * arg: row of the counters set we are iterating through
 * key: docID
 * count: count of word for the given docID
 * Output: none (directly modifies the input counters set) 
 */
void selection_sort_helper(void *arg, const int key, const int count);

#endif // __QUERIER_H
