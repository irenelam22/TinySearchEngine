# CS50 Lab 4 Crawler 
## Irene Lam, Summer 2020

### crawler

A `crawler` crawl a website and retrieve webpages beginning with a given URL. It parses through the initial webpage, extracts embedded URLs and retrieves those pages, and crawls these pages until there are no more pages to crawl or `maxDepth` is reached. Crawled URLs are further limited by those 'internal' to the designated CS50 server. When the crawler process is complete, the indexing of the documents begin, in which each document is saved to a unique id, with URL, current crawled depth, and HTML content.

### Usage

The *crawler* module, defined in `crawler.h` and implemented in `crawler.c`, crawls through a given webpage up until a given depth and indexes these documents using the below functions:

```c
bool inputCheck(webpage_t *seedURL, char* pageDir, int maxDepth);
void crawler(webpage_t* seedURL, char* pageDir, int maxDepth);
char* pagefetcher(webpage_t *page);
void pagescanner(webpage_t *page, hashtable_t *visited, bag_t *bag, int depth);
void bag_web_print(FILE *fp, void *item);
```

Please run the following to execute the `crawler`:
`./crawler seedURL pageDirectory maxDepth`
* where `seedURL` is used as the initial URL,
* where `pageDirectory` is the directory in which to write downloaded webpages, and
* where `maxDepth` represents the maximum crawl depth.

### Implementation

Please refer to `IMPLEMENTATION.md` for details regarding implementation.

(Note: I attended TA Katherine Taylor's office hours. As per her advice, the DESIGN.md pseudocode
is sufficient for the Implementation spec. We also weren't sure how to include language dependent, operating system, and hardware dependent characteristics typical of the implementation spec, so these were mostly left out.)

### Assumptions

I made the same assumptions as those listed within the spec and within this file. Additionally, I chose to comment out log messages (e.g. failure to fetch a webpage) within crawler for brevity. I left these comments within the code such that they can be commented back in to see these log messages.

### Files

* `Makefile` - compilation procedure
* `crawler.h` - the interface
* `crawler.c` - the implementation
* `testing.sh` - integration testing driver
* `testing.out` - test data
* `IMPLEMENTATION.md` - implementation details
* `DESIGN.md` - design specs 

### Compilation

To compile, simply `make crawler.o`.

### Testing

The `testing.sh` script tests crawler on numerous files, and tests errors and edge cases.

To test, simply `make test`.