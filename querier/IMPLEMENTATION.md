# TSE Querier Implementation Spec
### Irene Lam, Dartmouth CS50, Summer 2020

### Overview

The specific data structures are defined in the sections below.

The `querier` reads the documents in a given `directory` outputted by the crawler and the index file produced by indexer, to interactively answer written queries entered by the user. 

### querier

A `querier` reads an index file produced by indexer to determine matches the queries from stdin. It further parses through the crawler-generated directory for URLs that correspond to the matched results. The `querier` uses a *counterset* data structure to keep track of the documentID and respective count of each word, minimizing/summing over these counts depending on the conjunctions passed (AND, OR).

The `querier` module supports the following local data types:
```c
/*************** twocts ***************
 * stores two counter sets, used during min_iterate to minimize
 * across the counter sets
 */
struct twocts;

/*************** row ***************
 * used during selection sort to rank the outputs by count
 * stores the key corresponding to the running maximum count, the running
 * maximum count, an array of visited docIDs, and the size of the 
 * list being traversed
 */
struct row;
```

The `querier` module implements the following methods:
(Please see pseudocode for pseudocode implementation.)

```c
/************** main ***************
 * Main method runs the program
 * Inputs:
 * pageDirectory: the pathname of a directory produced by the Crawler, and
 * indexFilename: the pathname of a file produced by the Indexer.
 * Output: 0 if successful, 1 otherwise
 * 
 * Usage: ./querier pageDirectory indexFilename
 */ 
int main(int argc, char* argv[]);

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

/************** selection_sort ***************
 * Prints the counters set in descending order based on counts
 * Inputs:
 * set: counters set to be printed in descending order
 * pagedir: crawler directory
 * Output: none (directly prints the input counters set) 
 */
void selection_sort_helper(void *arg, const int key, const int count);
```

### Pseudo code for querier

(Note: This was based on the pseudocode by Professor Zhou given within the assignment.)

The `querier.c` implementation runs as follows (using C):
1. Execute from a command line with usage syntax `./querier pageDirectory indexFilename`
2. Validate its command-line arguments:
    1. Ensure `pageDirectory` is the pathname for an existing Crawler-generated directory by checking for the existence of .crawler
    2. Ensure `indexFilename` is the pathname of a readable, existing file
2. Load the index from `indexFilename` into an internal data structure.
3. Read search queries from stdin, one per line, until EOF.
	1. clean each query by changing the query to its lower case variation
    2. if the query syntax is somehow invalid, print an error message, do not perform the query, and prompt for the next query.
	3. print the 'clean' query for user to see.
	4. use the index to identify the set of documents that satisfy the query
        1. Check the next word in the query (if any)
            1. If the word is an OR, combine the current query into the final counters set
            2. Otherwise, add the current query to the temporary counters set of AND sequences
        2. If there are no more words and the temporary counters set is not empty, merge the temporary counters set into the final set by summing over the values
	5. if the query is empty (no words), print nothing.
	6. if no documents satisfy the query, print `No documents match.`
4. Rank the resulting set of documents according to its score, and print the set of documents in decreasing rank order; for each, list the score, document ID and URL.
    1. Obtain the URL by reading the first line of the relevant document file from the `pageDirectory`.
5. Exit with zero status when EOF is reached on stdin.


### Testing plan for querier

To test the output of `querier.c`, we run several hard-coded examples as welll as `fuzzquery` inputs and manually check the outputs.

In particular, we check several cases that test conjunction precedence (that AND is considered before OR), that each respective conjunction properly sums (in the case of OR) or minimizes (in the case of AND) over the counts, as well as other edge cases. We should note that because the pathname to the index file and crawler director are passed and not generated, it is less important to test on different index/crawler directories, and more input to test different permutations of words present or absent in these files. Of course, the larger the file, the more intersections/unions the words will presumably have, which allow tests to be more accurate.

Please see `DESIGN.md` for more testing specific examples.