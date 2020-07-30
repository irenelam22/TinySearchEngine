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

### Implementation

Please refer to `IMPLEMENTATION.md` for details regarding implementation.

### Assumptions

No assumptions beyond those that made within the spec.

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

To test with valgrind, `make valgrind`.
