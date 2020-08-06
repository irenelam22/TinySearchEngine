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


//./indextest oldIndexFilename newIndexFilename
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./indextest oldIndexFilename newIndexFilename\n");
        return 1;
    }
    
    index_t* index = index_new(10);
    index = index_load("../data");
    index_save(index, argv[2]);
    return 0;
}