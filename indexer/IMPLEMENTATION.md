# TSE Indexer Implementation Spec
### Irene Lam, Dartmouth CS50, Summer 2020

### Overview

The specific data structures are defined in the sections below.

An `indexer` reads the documents in a given `directory` outputted by the crawler (see `Crawler README` for details), builds an inverted index mapping from words to document ID and count, and writes that index to a file.

### indexer

A `indexer` reads files from the given directory, builds an index that maps from words to pages (URLs), and writes that index to a given file.. The `indexer` uses a *hashtable* data structure to store the words, and uses a *counterset* data structure to keep track of the documentID and respective count of the word.

The `indexer` module implements the following method:
(Please see pseudocode for pseudocode implementation.)

```c
/**
 * Main method for indexer, builds an index from the output directory of crawler and saves this index data structure to a given file
 */
int main(int argc, char* argv[])
```

### common (index)

`common` assembles code that will eventually be shared by crawler, indexer, and querier. For the purposes of this lab, the only additions to common is in creating the `index_t` data structure (with all its corresponding insert, print, delete, load, save, etc. functions), and normalizing a given word.

In particular, this functionality is implemented within the `index.c` module, which supports the below methods:

```c

/**************** global types ****************/
typedef hashtable_t index_t;

/**************** functions ****************/

/**************** index_new ****************/
/* Create a new (empty) index_t of the given integer size.
 * Input: integer representing the size of the index
 * Output: initialized and empty index_t
 */
index_t* index_new(const int num_slots);

/**************** index_find ****************/
/* Returns the counters set associated with the given key
 * Input: index, key of set to find
 * Output: corresponding set if found, NULL if hashtable is NULL, 
 *         key is NULL, key is not found
 */
void* index_find(index_t* index, const char* key);

/**************** index_insert ****************/
/* Insert item, identified by key (string), into the given index.
 * Input: counter set item and corresponding key
 * Output: true if the new item was inserted properly, false otherwise
 */
bool index_insert(index_t* index, const char *key, void *item);

/**************** printset ****************/
/* Helper method to print counters set as follows
    docID count [docID count]...
 * Input: filename, docID key, int count
 * Output: None (prints to file)
 */
void printset(void *fp, const int key, const int count);

/**************** item_print ****************/
/* itemprint method to be passed to index_print
 * Prints counters as follows:
        word docID count [docID count]...
 * Input: file, word key, counters set item
 * Output: None (prints to file)
 */
void item_print(void *fp, const char *key, void *item);

/**************** index_print ****************/
/* Prints index where each line is as follows:
    word docID count [docID count]...
 * Input: index, file to print to, itemprint method
 * Output: None (prints to file)
 */
void index_print(index_t *ht, void *fp,
                     void (*itemprint)(void *fp, const char *key, void *item));

/**************** itemdelete ****************/
/* itemdelete method to be passed to index_delete
 * Deletes the counters set within the index
 * Input: counters set (from index)
 * Output: None
 */
void itemdelete(void* item);

/**************** index_delete ****************/
/* Deletes the index and its interior counter set
 * Input: index to be deleted
 * Output: None
 */
void index_delete(index_t* index );

/**************** iprint ****************/
/* For debugging purposes
 * Prints given index to stdout
 * Input: index
 * Output: None (prints to stdout)
 */
void iprint(index_t* index); 

/**************** index_load ****************/
/* Creates an index data structure from a given index file and returns this
 * Input: filename of index file produced from index_save
 * Output: index representation of given file
 */
index_t* index_load(char* filename);

/**************** index_save ****************/
/* Saves the given data structure into a writable file
 * Input: Index data structure, file
 * Output: None
 */
void index_save(index_t* index, char* filename);

/**************** index_build ****************/
/* Builds an index data structure from a given pagedirectory by parsing through
 * the directory for respective word count and document ID
 * Input: directory output from crawler
 * Output: index representation of the file
 */
index_t* index_build(char* pagedir);

/**************** updateIndex ****************/
/* Helper method for index_build
 * Updates the file within the directory for parsing
 * Input: filename, integer tagged to the file
 * Output: new file name (incremented by 1)
 */
char* updateIndex(char* fp, int i);
```

Note: Only `index_build` and `index_save` were used for `indexer.c`. `index_load` was used for `indextest`, as explained in testing plan section below.

### Pseudo code for indexer

(Note: This was given by Professor Zhou within the assignment.)

The `indexer.c` implementation runs as follows (using C):
```c
1. Process and validate command-line parameters
2. Initialize data structure index
3. Build the index data structure using `index_build(directory)`
    1. Check validity of parameters (must be a crawler generated directory)
    2. Instantiate an index structure
    3. While the passed directory contains unread files
        1. Check that the file contains a URL, depth, and HTML section
        2. Create a webpage from the contents of the file
        3. For each word in the webpage
            1. Ignore the word if the number of characters is less than 3
            2. Normalize the word (as defined below)
            3. Increment the count of the respective word in the index structure if the word was previously found in the document. Otherwise, create a new counters set of the documentID and count (in this case, 1), and insert it as the item to the word in index
    4. Clean up necessary files and variables
4. Save the index into a file using `index_save(file, index)`
    1. Check validity of parameters 
    2. Call `item_print` to print the index data structure into the given file
5. Clean up data structures
```

### Testing plan for indexer

To test the output of `indexer.c`, we compare them to the [files](http://old-www.cs.dartmouth.edu/~cs50/data/tse-output/) given by Professor Zhou. We can similarly do the same to test the validity of `indextest.c`.

In particular, to test the accuracy of `indexer`, we use the `testing.sh` script. In addition to checking for edge cases, the script will also sort the output of the `indexer` and Professor Zhou's respective file and compare the two. If the outputs are the same, we know that the index structures produced from `indexer.c` are indeed the same, meaning that `indexer` was accurate in parsing through the crawler directory and tallying the frequency of the words per given document file.

Please see `DESIGN.md` for more testing specific examples.