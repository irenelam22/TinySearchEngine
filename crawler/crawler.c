/**
 * crawler.c -- driver file for the crawler portion of the Tiny Search Engine
 * Please see README.md for more details
 * 
 * Irene Lam, July 26, 2020
 * Dartmouth CS50, Summer 2020
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"

bool inputCheck(webpage_t *seedURL, char* pageDir, int maxDepth);
void crawl(char* seedURL, char* pageDir, int maxDepth);

int main(int argc, char* argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Insufficient number of arguments\n");
    }
    if (argc > 4) {
        fprintf(stderr, "Too many arguments provided\n");
    }
    // Normalize seedURL
    webpage_t *seedURL = webpage_new(argv[1], 0, NULL);
    if (!NormalizeURL) {
        fprintf(stderr, "Error normalizing seedURL\n");
        return 1;
    }
    if (!inputCheck(seedURL, argv[2], argv[3])) {
        return 1;
    }
    crawler(argv[1], argv[2], argv[3]);
    
    return 0;
}

bool inputCheck(webpage_t *seedURL, char* pageDir, int maxDepth)
{
    if (seedURL == NULL) {
        fprintf(stderr, "seedURL is NULL\n");
        return false;
    }
    if (!IsInternalURL) {
        fprintf(stderr, "seedURL is not an internal URL\n");
        return false;
    }
    if (pageDir == NULL) {
        fprintf(stderr, "pageDir is NULL\n");
        return false;
    }
    // TODO: check whether directory exists
    if (maxDepth < 0 || maxDepth > 10) {
        fprintf(stderr, "maxDepth must be within the range [0,10]\n");
        return false;
    }
}

void crawler(char* seedURL, char* pageDir, int maxDepth)
{
    if (maxDepth < 0) {
        return;
    }
    // Initializing data structures
    bag_t *bag = bag_new();
    hashtable_t *visited = hashtable_new(10);
    int currDepth = 0;

    bag_insert(bag, seedURL);
    hashtable_insert(visited, seedURL, currDepth);

    while ((webpage_t *item = bag_extract(bag)) != NULL) {
        
    }

    
}

// outputs a page to the the appropriate file
void pagesaver(webpage_t *seedURL, char* pageDir) 
{

}