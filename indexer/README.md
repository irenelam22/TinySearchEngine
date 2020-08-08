# CS50 Lab 5 Indexer 
## Irene Lam, Summer 2020

### indexer

An `indexer` reads the documents in a given `directory` outputted by the crawler (see `Crawler README` for details), builds an inverted index mapping from words to document ID and count, and writes that index to a file.

### indextest

`indextest` reads an index file from indexer and saves the recreated index data structure into another file.

### Usage

The *indexer* and *indextest* modules, implemented in `indexer.c` and `indextest.c` respectively, builds and saves and loads an index mapping using the below functions from `/commmon/index.c`. Please see the respective file for more details on these functions:

```c
index_t* index_load(char* filename);
void index_save(index_t* index, char* filename);
index_t* index_build(char* pagedir);
```

Please run the following to execute `indexer`:
`./indexer pageDirectory indexFilename`
* where `pageDirectory` is the pathname of a directory produced by the Crawler, and
* where `indexFilename` is the pathname of a file into which the index should be written; the indexer creates the file (if needed) and overwrites the file (if it already exists).

Please run the following to execute `indextest`:
`./indextest oldIndexFilename newIndexFilename`
* where `oldIndexFilename` is the name of a file produced by the indexer, and
* where `newIndexFilename` is the name of a file into which the index should be written.

### Implementation

Please refer to `IMPLEMENTATION.md` for details regarding implementation.


### Assumptions

I made the same assumptions as those listed within the specs of this assignment, as well as those listed within this file. Additionally, I assumed that the given filename will not end in a backslash (given that it is supposed to be a file and not a directory).

As per the assignment, I assumed that the `DESIGN` and `IMPLEMENTATION` spec need only be written for `indexer.c` and not `indextest.c`. As per my previous conversation with TA Katherine Taylor, I assumed that the `IMPLEMENTATION` spec does not need pseudocode, or sufficies with the pseudocode given by the assignment, and that sections for language dependent, operating system, and hardware dependent characteristics typical of the implementation spec could be left out for this assignment.

I assumed unit testing is not required for this lab (according to slack).

As per TA Arjun Srinivasa's advice, I assume that testing the accuracy of `indexer` means comparing the results of `indexer` to those provided by Professor Zhou [here](http://old-www.cs.dartmouth.edu/~cs50/data/tse-output/).

### Files

* `Makefile` - compilation procedure
* `indexer.c` - the implementation for indexer
* `indextest.c` - the implementation for indextest
* `testing.sh` - integration testing driver
* `testing.out` - test data
* `IMPLEMENTATION.md` - implementation details for indexer
* `DESIGN.md` - design specs for indexer 

### Compilation

To compile, simply `make indexer.o`.

### Testing

The `testing.sh` script tests `indexer` and `indextest` on numerous files for errors and edge cases. It also ensures that the output of the two files is the same once sorted.

To test, simply `make test`.
