# CS50 Lab 5 Querier 
## Irene Lam, Summer 2020

### querier

A `querier` reads the index produced by the Indexer (see `Indexer README` for details) and the page files produced by the Crawler (see `Crawler README` for details), to interactively answer written queries entered by the user.

### Usage

The *querier* module, implemented in `querier.c`, answers search queries from stdin using the following methods:

```c
void query_print(void *arg, const int key, const int count);
bool inputCheck(char* dircopy, char* index);
int extract_words(char* line, char** words );
bool valid_query(char** query, int length);
void process_query(index_t* index, char* pagedir);
void sum_iterate(void *arg, const int key, const int count);
void min_iterate(void *arg, const int key, const int count);
void run_query(char** words, index_t* index, char* url) ;
void query_print(void *arg, const int key, const int count);
void selection_sort(counters_t* set, char* url);
void findLength(void *arg, const int key, const int count);
void selection_sort_helper(void *arg, const int key, const int count);
```

Please run the following to execute `querier`:
`./querier pageDirectory indexFilename`
* where `pageDirectory` is the pathname of a directory produced by the Crawler, and
* where `indexFilename` is the pathname of a file produced by the Indexer.

### Implementation

Please refer to `IMPLEMENTATION.md` for details regarding implementation.

### Functionality

The `querier` supports the following functionalities:
* prints the set of documents that contain all the words in the query
* supports ‘and’ and ‘or’ operators
* supports ‘and’ and ‘or’ operators, with 'and' precedence over 'or'
* prints the document set in decreasing order by score

### Assumptions

I made the same assumptions as those listed within the specs of this assignment, as well as those listed within this file. 

Additionally, I assumed that the any parameter filenames will not end in a backslash (given that it is supposed to be a file and not a directory). If the filename ends in a backslash, `indexer` and `indextest` will return some statement regarding invalid input.

As per my previous conversation with TA Katherine Taylor, I assumed that the `IMPLEMENTATION` spec does not need pseudocode, or sufficies with the pseudocode given by the assignment, and that sections for language dependent, operating system, and hardware dependent characteristics typical of the implementation spec could be left out for this assignment.

I assumed unit testing is not required for this lab.

I assumed the order of docIDs after sorting does not matter (according to slack). The docIDs therefore display based on the order of the words in stdin, and next on their initial order in the index file.

### Files

* `Makefile` - compilation procedure
* `querier.c` - the implementation for querier
* `testing.sh` - integration testing driver
* `testing.out` - test data
* `IMPLEMENTATION.md` - implementation details 
* `DESIGN.md` - design specs 

### Testing Files
* `BASIC_CONJUNCTION` -  Testing accuracy of 'and' and 'or' conjunctions (letters at depth 6)
* `BASIC_TEST` - One word test, used for error-handling
* `MISSING_WORDS` - Testing with words not in letters at depth 6
* `ONE_WORD_TEST` - Testing with one words for letters at depth 6
* `PRECEDENCE_TEST` - Testing with multiple conjunctions to test precedence for letters at depth 6
* `NON_LETTERS` - Testing with non-letter characters (e.g. -,+,!)
* `TOSCRAPE_TEST` - Testing multiple examples for toscrape, applicable for depths 0, 1, 2
* `WIKIPEDIA_TEST` - Testing multiple examples for wikipedia, applicable for depths 0, 1, 2


### Compilation

To compile, simply `make querier.o`.

### Testing

The `testing.sh` script tests `querier` on numerous files for errors and edge cases. 

To test, simply `make` then `make test`.

Please find results in `testing.out`.
