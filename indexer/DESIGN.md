# TSE Indexer Design Spec
### Irene Lam, Dartmouth CS50, Summer 2020

The **indexer** reads the document files produced by the TSE crawler, builds an index, and writes that index to a file.

When the indexing process is complete, the quering of the index files can begin.


### User interface

The indexer's only interface with the user is on the command-line; it must always have two arguments.

```
./indexer pageDirectory indexFilename
``` 

For example:

``` bash
$ ./indexer ../data/ ../data/indexnew
```

### Inputs and outputs

Input: the only inputs are command-line parameters; see the User Interface above.

Output: We read each file from the given `pageDirectory`, each with a unique document ID, and build an inverted-index data structure (called `index_t`) that maps from *words* to *(documentID, count) pairs*, wherein each *count* represents the number of occurrences of the given word in the given document.

We then create a file `indexFilename`and write the index to that file, in the format described below:

 * one word per line
 * each line provides the word and one or more (docID, count) pairs, in the format
	 * *word docID count [docID count]...*
 * where *word* is a string of lower-case letters (ignoring words with less than three characters),
 * where *docID* is a positive non-zero integer,
 * where *count* is a positive non-zero integer,
 * where the word and integers are separated by spaces.

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules
 2. *indexer*, which builds and saves an inverted-index data structure given a crawler directory
 3. *index_build*, which builds an index data structure from a given crawler-created pagedirectory 
 4. *index_save*, which outputs a page to the the appropriate file
 4. *index_load*, (for `indextest`) which creates an index data structure from a given index file

And some helper modules that provide data structures:

 1. *hashtable* to help instantiate the `index_t` data structure
 2. *counterset* to keep track of the documentID and respective ID for each word

Additionally, we introduce the below basic methods specific to `index_t`:
 1. *index_new*, which instantiates a new index data structure
 2. *index_find*, which finds the counter set of a given key
 3. *index_insert*, which inserts a new item at the given key
 4. *index_print*, which prints the index structure in the format described above
 5. *index_delete*, which deletes the index structure and its counter sets (if possible)

### Pseudo code for logic/algorithmic flow

The indexer will run as follows:
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

**normalize the word** means to change all characters to lower-case.
As one simple example, `Home` becomes `home`.

Please note that the order in which words are displayed when calling `index_save` cannot be guaranteed. This is because `index_t` uses many methods from *hashtable*, which explicitly denies any promise about the order of items traversed.

The indexer completes and exits when there are no more files within the given crawler-generated directory to traverse.


### Dataflow through modules

 1. *main* parses parameters and passes them to `index_build` and `index_save`.
 2. *index* uses an index data structure (upgraded hashtable with counters set item) to track the frequency of words as they appear across different files 
 3. *index_build* builds an index data structure from a given crawler-created pagedirectory 
 4. *index_save* outputs a page to the the appropriate file
 4. *index_load* (for `indextest`) creates an index data structure from a given index file


### Major data structures

Two helper modules provide data structures:

 1. *countes set* of documentID (key) and frequency (count)
 2. *hashtable* of words (key) and aforementioend counters set

### Testing plan

*Integration testing*.  Assemble the indexer and test it as a whole.
In each case, examine the output files carefully to be sure they count the number of words correctly, contain all words (of 3 or more characters) listed within the files, prints properly, ensure that no words are missing or duplicated, etc.

Prints "progress" indicators from the indexer as it proceeds (e.g. current file being read).

0. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

1. Test the indexer with a nonexistant `pageDirectory`.

2. Test the indexer with a non-crawler generated `pageDirectory`.

3. Test the indexer with a non-writable `pageDirectory`.

4. Create an index file from a simple, closed set of cross-linked web pages using `indexer`, in which some page(s) are mentioned multiple times within a page, and multiple times across the set of pages.

Compare the result with the respective index file from Professor Zhou. Sort the two and compare the files to ensure that the `indexer` created a proper `index` file.

5. Point the indexer at a page in that site, and explore at depths 0, 1, 2, 3, 4, 5.
Verify that the files created match expectations using Professor Zhou's files.

6. Point the indexer at the Wikipedia playground. Explore at depths 0, 1, 2.
Verify that the files created match expectations using Professor Zhou's files.

7. More tests as needed! (In this case, on the toscrape playground.)

(Note: Only tests with verifiable results (e.g. available on Professor Zhou's answer key) were tested to ensure the accuracy of results.)
