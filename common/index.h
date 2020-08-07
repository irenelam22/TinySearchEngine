
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

typedef hashtable_t index_t;

index_t* index_new(const int num_slots);
void* index_find(index_t* index, const char* key);
bool index_insert(index_t* index, const char *key, void *item);
void index_delete(index_t* index );
index_t* index_load(char* filename);
void index_save(index_t* index, char* filename);
index_t* index_build(char* pagedir);
char* updateIndex(char* fp, int i);