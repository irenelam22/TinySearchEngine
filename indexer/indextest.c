/* 
 * indextest.c - second driver for `index` module
 *
 * The indextest reads an index file from indexer and saves the 
 * recreated index data structure into another file
 * 
 * see README for more details.
 *
 * Irene Lam, August 4, 2020
 * CS50, Summer 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"
#include "../common/pagedir.h"
#include "../common/index.h"

/************** main ***************
 * Builds an inverted index mapping from words to document ID and count
 * based on indexer output
 * 
 * Inputs:
 * where `oldIndexFilename` is the name of a file produced by the indexer, and
 * where `newIndexFilename` is the name of a file into which the index should be written.
 * Output: 0 if successful, 1 otherwise
 * 
 * Usage: ./indextest oldIndexFilename newIndexFilename
 */
int main(int argc, char* argv[]) {
    // Error-handling (basic, we trust that good inputs are provided)
    if (argc != 3) {
        fprintf(stderr, "Usage: ./indextest oldIndexFilename newIndexFilename\n");
        return 1;
    }

    // Load and save an index based on the output of indexer 
    index_t* index = index_load(argv[1]);
    index_save(index, argv[2]);
    index_delete(index);
    return 0;
}
