#include "querier.h"

/************** main ***************
 * 
 * Inputs:
 * pageDirectory: the pathname of a directory produced by the Crawler, and
 * indexFilename: the pathname of a file produced by the Indexer.
 * Output: 0 if successful, 1 otherwise
 * 
 * Usage: ./querier pageDirectory indexFilename
 */
int main(int argc, char* argv[])
{
    // Error-handling
    if (argc < 3) {
        fprintf(stderr, "Insufficient number of arguments\n");
        return 1;
    }
    if (argc > 3) {
        fprintf(stderr, "Too many arguments provided\n");
        return 1;
    }
    char* dir = argv[1];
    char* dircopy = assertp(malloc(strlen(argv[1])+10), "dir copy");
    strcpy(dircopy, dir);

    // Add a backslash to the end of the directory if needed
    if (dircopy[strlen(dircopy)-1] != '/') {
        strcat(dircopy, "/");
    }

    if (!inputCheck(dircopy, argv[2])) {
        return 1;
    }
    // index_t* index = index_load(argv[1]);
    return 0;
}
