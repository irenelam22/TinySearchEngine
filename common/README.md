# CS50 Lab 4  
## Irene Lam, Summer 2020

### common

`common` assembles code that will eventually be shared by crawler, indexer, and querier. Currently, this module only contains code for initializing the “pageDirectory” and saving & fetching webpages; creating the `index_t` data structure, and normalizing a given word.

### Usage

The *pagedir* module, defined in `pagedir.h` and implemented in `pagedir.c`, checks whether a passed directory is valid (exists & writable), and saves and fetches webpages. These are implemented with the below methods:

```c
bool isDirectory(char* directory);
char* pagefetcher(webpage_t *page);
bool pagesaver(webpage_t *page, char* pageDir, int id);
```

The *index* module, defined in `index.h` and implemented in `index.c`, implements a `index_t` struct, based on the hashtable implementation, that keeps an inverted mapping of word to document ID & count. These are implemented with the below methods:

```c
typedef hashtable_t index_t;
index_t* index_new(const int num_slots);
void* index_find(index_t* index, const char* key);
bool index_insert(index_t* index, const char *key, void *item);
void printset(void *fp, const int key, const int count);
void item_print(void *fp, const char *key, void *item);
void index_print(index_t *ht, void *fp,
                     void (*itemprint)(void *fp, const char *key, void *item));
void iprint(index_t* index); 
void itemdelete(void* item);
void index_delete(index_t* index );
void index_delete(index_t* index );
index_t* index_load(char* filename);
void index_save(index_t* index, char* filename);
index_t* index_build(char* pagedir);
char* updateIndex(char* fp, int i);
```

The *word* module, defined in `word.h` and implemented in `word.c`, simply normalizes a given word (by returning the lower-case counterpart of the word). This is implemented in the below method:

```c
char* normalizeWord(char* original); 
```

### Implementation

Please refer to `pagedir.h`, `index.h`, and `word.h` for more details regarding their respective implementation.

### Assumptions

I made all assumptions listed within the spec. In addition, I wasn't sure what methods would be shared between crawler, indexer, and querier, so I placed all methods relevant to `index_t` in `index.c`.

As per AnonymityBot on Slack, I assumed that the index could be instantiated within `index_build`. As such, certain parameters and function declarations may be different from what is listed in the assignment.

### Files

* `Makefile` - compilation procedure
* `pagedir.h` - the interface for pagedir
* `pagedir.c` - the implementation for pagedir
* `index.h` - the interface for index
* `index.c` - the implementation for index
* `word.h` - the interface for word
* `word.c` - the implementation for word

### Compilation

To compile, simply run `make`.

