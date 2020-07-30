/**
 * crawler.c -- driver file for the crawler portion of the Tiny Search Engine
 * The main objective of the 'crawler' is to crawl a website and retrieve webpages 
 * beginning with a given URL. It parses through the initial webpage, 
 * extracts embedded URLs and retrieves those pages, and crawls these pages 
 * until there are no more pages to crawl or `maxDepth` is reached. Crawled URLs
 * are further limited by those 'internal' to the designated CS50 server.
 * When the crawler process is complete, the indexing of the documents begin.
 * Please see README.md for more details
 * 
 * Usage: ./crawler seedURL pageDirectory maxDepth
 * 
 * Irene Lam, July 26, 2020
 * Dartmouth CS50, Summer 2020
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

// Function prototypes
bool inputCheck(webpage_t *seedURL, char* pageDir, int maxDepth);
void crawler(webpage_t* seedURL, char* pageDir, int maxDepth);
char* pagefetcher(webpage_t *page);
bool pagesaver(webpage_t *page, char* pageDir, int id);
void pagescanner(webpage_t *page, hashtable_t *visited, bag_t *bag, int depth);
void bag_web_print(FILE *fp, void *item);

/**
 * Main method for crawler
 * 
 * Input: seedURL, pageDirectory, maxDepth
 * Output: integer (0 if successful, 1 if error)
 */
int main(int argc, char* argv[])
{
    // Error-handling: Ensuring number of arguments passed suffices
    if (argc < 4) {
        fprintf(stderr, "Insufficient number of arguments\n");
        return 1;
    }
    if (argc > 4) {
        fprintf(stderr, "Too many arguments provided\n");
        return 1;
    }
    // Normalize seedURL and throws an error if URL cannot be normalized
    char* seedURL = argv[1];
    if (!NormalizeURL(seedURL)) {
        fprintf(stderr, "Error normalizing seedURL %s\n", seedURL);
        return 1;
    }
    // Making a copy of the seedURL on the heap
    char* urlcopy = assertp(malloc(strlen(seedURL)+1), "url copy");
    strcpy(urlcopy, seedURL);

    // char* dircopy = argv[2];
    // Making a copy of the directory on the heap
    char* dir = argv[2];
    char* dircopy = assertp(malloc(strlen(argv[2])+10), "dir copy");
    strcpy(dircopy, dir);

    // Add a backslash to the end of the directory if needed
    if (dircopy[strlen(dircopy)-1] != '/') {
        strcat(dircopy, "/");
    }

    // Check inputs (see inputCheck below for more details)
	webpage_t *web = webpage_new(urlcopy, 0, NULL); 
    if (!inputCheck(web, dircopy, atoi(argv[3]))) {
        // free(urlcopy);
        webpage_delete(web);
        free(dircopy);
        return 2;
    }

    crawler(web, dircopy, atoi(argv[3]));
    free(dircopy);
    return 0;
} 

/**
 * inputCheck -- checks whether inputs received are valid
 * Input: (webpage_t*) seedURL, (char*) pageDir, (int) maxDepth
 * Output: true if all inputs are valid, false otherwise
 */
bool inputCheck(webpage_t *seedURL, char* pageDir, int maxDepth)
{
    // Checking validity of seedURL
    if (seedURL == NULL) {
        fprintf(stderr, "seedURL is NULL\n");
        return false;
    }
    if (!IsInternalURL(webpage_getURL(seedURL))) {
        fprintf(stderr, "seedURL is not an internal URL\n");
        return false;
    }

    // Checking validity of page directory (existing & writable)
    if (!isDirectory(pageDir)) {
        return false;
    }
    
    // Checking validity of maxDepth
    if (maxDepth < 0 || maxDepth > 10) {
        fprintf(stderr, "maxDepth must be within the range [0,10]\n");
        return false;
    }
    return true;
}

/**
 * crawler -- crawls a website and retrieves webpages 
 * beginning with a given URL. It parses through the initial webpage, 
 * extracts embedded URLs and retrieves those pages, and crawls these pages 
 * until there are no more pages to crawl or `maxDepth` is reached. Crawled URLs
 * are further limited by those 'internal' to the designated CS50 server.
 * When the crawler process is complete, the indexing of the documents begin.
 * 
 * See IMPLEMENTATION.md for more details 
 * 
 * Input: (webpage_t*) seedURL, (char*) pageDir, (int) maxDepth
 * Output: none
 */
void crawler(webpage_t* seedURL, char* pageDir, int maxDepth)
{
    // Initializing data structures
    bag_t *bag = bag_new();                     // Bag of pages to be explored
    hashtable_t *visited = hashtable_new(10);   // Hashtable of visited pages
    int currDepth = 0;                          // Current depth counter
    int id = 1;                                 // Unique page ID counter
    webpage_t* item;                            // Current webpage

    // Populate 
    bag_insert(bag, seedURL);
    hashtable_insert(visited, webpage_getURL(seedURL), &currDepth);
	
    /*
    Fetch, save, scan, and delete the webpage while there are more 
    pages to explore, contingent on maxDepth not being exceeded and 
    no errors while exploring pages
    */
    while ((item = bag_extract(bag)) != NULL) {
        if (pagefetcher(item) == NULL)
            continue;
        if (pagesaver(item, pageDir, id)) {
            id++;
        }
        
        if (webpage_getDepth(item) < maxDepth) {
            pagescanner(item, visited, bag, webpage_getDepth(item));
        }
        webpage_delete(item);
    }

    // Free memory
    bag_delete(bag, NULL);
    hashtable_delete(visited, NULL);
}


/**
 * pagescanner -- extracts URLs from a page and returns each one at a time
 * Inputs: webpage, hashtable, bag, current depth
 * Output: none (directly populates the bag with each extracted URL)
 */		
void pagescanner(webpage_t *page, hashtable_t *visited, bag_t *bag, int depth)
{
    // Error-handling
    if (page == NULL) {
        fprintf(stderr, "pagescanner received invalid webpage\n");
    }
    char* next;
    int pos = 0;
    // While there are more webpages to be extracted
    while ((next = webpage_getNextURL(page, &pos)) != NULL) {
        
        // Normalize next URL
        if (!NormalizeURL(next)) {
            fprintf(stderr, "Error normalizing next URL %s\n", next);
            free(next);
            continue;
        }

        // Internal URL check
        if (!IsInternalURL(next)) {
            free(next);
            continue;
        }

        // Insert the webpage into the hashtable after visiting
        if (hashtable_insert(visited, next, &depth)) {
            char* urlcopy = assertp(malloc(strlen(next)+1), "url copy");
            strcpy(urlcopy, next);

            // Extract any additional pages and insert into bag
            webpage_t *web = webpage_new(urlcopy, depth+1, NULL); 
            bag_insert(bag, web);
        }
        free(next);
    }
}

/*
 * bag_web_print: itemprint function to print each URL within the bag
 * Used for debugging purposes
 * Input: FILE*, void*
 * Output: None (prints each webpage URL within the bag)
 */
void bag_web_print(FILE *fp, void *item) 
{
    webpage_t *web = (webpage_t*) item;
    fprintf(fp, "%s", webpage_getURL(web));
}
