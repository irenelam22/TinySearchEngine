# TSE Crawler Implementation Spec

### Overview

The specific data structures are defined in the sections below.

A `crawler` crawl a website and retrieve webpages beginning with a given URL. It parses through the initial webpage, extracts embedded URLs and retrieves those pages, and crawls these pages until there are no more pages to crawl or `maxDepth` is reached. Crawled URLs are further limited by those 'internal' to the designated CS50 server. When the crawler process is complete, the indexing of the documents begin, in which each document is saved to a unique id, with URL, current crawled depth, and HTML content.

### crawler

A `crawler` takes in command-line input and saves each explored webpage to a unique file, with a designated unique id number. The `crawler` uses a *bag* data structure to store webpages yet to be explored. In addition, a *hashtable* data structure records the webpages visited, to ensure that cycles within webpages are handled and that each webpage saved is unique.

The `crawler` module implements the following methods:
(Please see pseudocode for pseudocode implementation.)

```c
/**
 * Checks whether inputs received are valid. In particular, 
 * the method takes a (webpage_t*) seedURL, (char*) pageDir, (int) maxDepth 
 * and returns true if all inputs are valid, false otherwise
 */
bool inputCheck(webpage_t *seedURL, char* pageDir, int maxDepth);

/**
 * Crawls a website and retrieves webpages beginning with a given URL, as 
 * described in the README.md file. In particular, the crawler takes in 
 * (webpage_t*) seedURL, (char*) pageDir, (int) maxDepth and calls the 
 * pagefetcher, pagescanner, and pagesaver method to populate the bag
 * and hashtable accordingly.
 * 
 * See below pseudocode for more details on crawler implementation
 */
void crawler(webpage_t* seedURL, char* pageDir, int maxDepth);

/**
 * Extracts URLs from a page and returns the URLs one at a time
 * It takes in a webpage, hashtable, bag, and current depth, and 
 * populates the given bag with each extracted URL
 */	
void pagescanner(webpage_t *page, hashtable_t *visited, bag_t *bag, int depth);

/*
 * Itemprint function to print each URL within the bag,
 * Used for debugging purposes only, to be passed to a bag data structure.
 * Takes in a FILE and void* item, and prints each webpage URL. 
 */
void bag_web_print(FILE *fp, void *item);

/**
 * Main method for crawler, makes a copy of URL and directory onto the heap.
 * Also does minimal error-checking and string parsing.
 */
int main(int argc, char* argv[])
```

### common (pagedir)

`common` assembles code that will eventually be shared by crawler, indexer, and querier. Currently, this module only contains code for initializing the “pageDirectory” and saving & fetching webpages.

In particular, this functionality is implemented within the `pagedir` module, which supports the below methods:

```c
/**
 * Checks whether the input string is a valid, writable directory.
 * In particular, the method takes a char*, presumably to a directory,
 * and returns a boolean signifiying whether input is a valid, writable directory
 */
bool isDirectory(char* directory);

/**
 * Fetches the contents (HTML) for a given webpage from a URL and returns
 * the HTML contents for that given page. 
 */
char* pagefetcher(webpage_t *page);

/**
 * Outputs a page to the appropriate file. The method takes a webpage, 
 * (char*) page directory, and (int) unique ID. The method returns a boolean
 * contingent on whether the page is successfully  outputted into its 
 * respective file,
 */
bool pagesaver(webpage_t *page, char* pageDir, int id);
```

### Pseudo code for crawler

(Note: This was given by Professor Zhou within the Design spec.)

The crawler implementation runs as follows (using C):

1. execute from a command line as shown in the User Interface
2. parse the command line, validate parameters (e.g. noramlize URL, check if URL is internal)
3. create space on the heap (and free when done) and initialize other modules
4. make a *webpage* for the `seedURL`, marked with depth=0
5. add that page to the *bag* of webpages to crawl
6. add that URL to the *hashtable* of URLs seen
7. while there are more webpages to crawl,
	8. extract a webpage (URL,depth) item from the *bag* of webpages to be crawled
	9. use *pagefetcher* to retrieve the HTML for the page at that URL,
    10. pause for at least one second,
	11. use *pagesaver* to write the webpage to the `pageDirectory` with a unique document ID, as described in the Requirements.
	12. if the webpage depth is < `maxDepth`, explore the webpage to find links:
		13. use *pagescanner* to parse the webpage to extract all its embedded URLs;
		14. for each extracted URL,
			15. 'normalize' the URL (see below)
			16. if that URL is not 'internal' (see below), ignore it;
			17. try to insert that URL into the *hashtable* of URLs seen
				18. if it was already in the table, do nothing;
				19. if it was added to the table,
					20. make a new *webpage* for that URL, at depth+1
					21. add the new webpage to the *bag* of webpages to be crawled
        22. delete the webpage
23. delete the bag and hashtable