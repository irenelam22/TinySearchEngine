# CS50 Lab 4  
## Irene Lam, Summer 2020

### common

`common` assembles code that will eventually be shared by crawler, indexer, and querier. Currently, this module only contains code for initializing the “pageDirectory” and saving & fetching webpages.

### Usage

The *pagedir* module, defined in `pagedir.h` and implemented in `pagedir.c`, checks whether a passed directory is valid (exists & writable), and saves and fetches webpages. These are implemented with the below methods:

```c
bool isDirectory(char* directory);
char* pagefetcher(webpage_t *page);
bool pagesaver(webpage_t *page, char* pageDir, int id);
```

### Implementation

Please refer to `pagedir.h` for details regarding implementation.

### Assumptions

I made all assumptions listed within the spec. In addition, I wasn't sure what methods would be shared between crawler, indexer, and querier, so I only included `isDirectory`, `pagefetcher`, and `pagesaver` within `pagedir`.

### Files

* `Makefile` - compilation procedure
* `pagedir.h` - the interface for pagedir
* `pagedir.c` - the implementation for pagedir


### Compilation

To compile, simply run `make`.

