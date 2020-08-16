# TSE Querier Design Spec
### Irene Lam, Dartmouth CS50, Summer 2020

The **querier** reads the document files produced by the TSE crawler and index file produced by TSE indexer, and interactively answer written queries entered by the user. In particular, based on the user's input, the querier parses through the index for words that match the input, and prints the respective document ID and count.


### User interface

The querier's only interface with the user is on the command-line; it must always have two arguments.

```
./querier pageDirectory indexFilename
``` 

For example:

``` bash
$ ./querier  ../tse-output/wikipedia-depth-2 ../tse-output/wikipedia-index-2 
```

### Inputs and outputs

Input: the only inputs are command-line parameters; see the User Interface above.

Output: We load the `indexFilename` into an index data structure, each containing the words to (documentID, count) for a given crawler directory. The querier then determines the number of matches among these words and prints these matches with the following information:

* the score -- the number of times a word satisfying the user's query displayed in a single document 
* the document ID corresponding to the score
* the URL for each match 

These results are diplayed in descending order by count.


### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules
 2. *extract_words*, which extracts words from a line and normalizes it into its lower-case counterpart
 3. *valid_query*, which validates words in a query (e.g. no consecutive conjunctions)
 4. *process_query*, which processes the user's query from stdin
 4. *sum_iterate*, which adds the count across sets, used for the OR sequence
 2. *min*, which returns the minimum of two numbers
 3. *min_iterate*, which minimizes the count across sets, used for `andsequences`
 4. *run_query*, which handles the AND and OR sequences from the query
 4. *selection_sort*, which prints the final counters set in descending order by count
 4. *findLength*, which finds the number of non-zero words in a set
 4. *selection_sort_helper*, which finds the running maximum count and respective key for all documents not in the visited list of docIDs

And some helper modules that provide data structures:

 1. *counterset* to keep track of the documentID and respective ID for each word

Additionally, we introduce the below local data types:
 1. *twocts*, which stores two counter sets
 2. *row*, which refers to a row in the counter set, storing the maximum running count and corresponding key, a list of visited document IDs, and the total number of words

### Pseudo code for logic/algorithmic flow

The querier will run as follows:
```c
1. Execute from a command line with usage syntax `./querier pageDirectory indexFilename`
2. Validate its command-line arguments:
    1. Ensure `pageDirectory` is the pathname for an existing Crawler-generated directory by checking for the existence of .crawler
    2. Ensure `indexFilename` is the pathname of a readable, existing file
3. Load the index from `indexFilename` into an internal data structure.
4. Read search queries from stdin, one per line, until EOF.
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
5. Rank the resulting set of documents according to its score, and print the set of documents in decreasing rank order; for each, list the score, document ID and URL.
    1. Obtain the URL by reading the first line of the relevant document file from the `pageDirectory`.
6. Exit with zero status when EOF is reached on stdin.
```

**normalize the word** means to change all characters to lower-case.
As one simple example, `Home` becomes `home`.

**AND sequence** refers to a series of words connected by ANDs (either implicitly or explicitly)

**OR sequence** refers to a series of sequences connected by ORs (explicitly)

Please note that the order in which docIDs are displayed after sorting by count cannot be guaranteed. This is because `index_t` uses many methods from *hashtable*, which explicitly denies any promise about the order of items traversed.

The querier completes and exits when there are no more words from stdin to traverse through, or when the user has stopped the program (e.g. using ctrl-D).


### Dataflow through modules

 1. *main* parses parameters, loads the index, and passes them to *process_query*
 2. *process_query* reads through each line, extracts and normalizes the word (using *extract_words* and *normalizeWord* respectively), before running the query
 3. *run_query* determines the number of matches based on AND and OR sequences (using *min_iterate* and *sum_iterate* respectively)
 4. *min_iterate* minimizes across the count of the words, used for AND-sequences
 5. *sum_iterate* adds the count of the words, used for OR-sequences
 6. *selection_sort* prints the matches in descending order based on the count of each document ID

### Major data structures

Three helper modules provide data structures:

 1. *counter set* of documentID (key) and frequency (count)
 2. *hashtable* of words (key) and aforementioend counters set
 3. *index* using the above structure to map word to (documentID, count) pairs

### Testing plan

*Integration testing*.  Assemble the querier and test it as a whole.
In each case, examine the outputted statements carefully to ensure they count the number of matches correctly, print the correct counts and corresponding document IDs and URLs, print the correct number of matches, ensures that no matches are missing or duplicated, etc.

0. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

1. Test the querier with a nonexistant `pageDirectory`.

2. Test the querier with a non-crawler generated `pageDirectory`.

3. Test the querier with a non-readable `indexFilename`.

4. Test the querier on an empty query.

5. Test the querier on a query with only whitespaces.

6. Test the querier with a starting conjunction.

7. Test the querier with an ending conjunction.

8. Test the querier with consecutive conjunctions.

9. Test the querier with words not present in the index file.

10. Test the querier with non-letter characters.

11. Point the querier to letters. Test the querier on one word, basic phrases combined by conjunctions, conjunction precedence (AND over OR), and using fuzzquery. Ensure that all results meet expectations, print the correct docIDs, counts, and URLs. 

12. Point the querier to toscrape and wikipedia at different depths and test with different combinations of phrases in addition to fuzzquery.

13. More tests as needed! (In this case, many fuzztests.)

(Note: Tests were manually checked.)
