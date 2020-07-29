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
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"

#ifdef DEBUG
void debug(char* msg) { fprintf(stderr, "DEBUG: %s\n", msg); }
#else
void debug(char* msg) {}
#endif

bool inputCheck(webpage_t *seedURL, char* pageDir, int maxDepth);
void crawler(webpage_t* seedURL, char* pageDir, int maxDepth);
char* pagefetcher(webpage_t *page);
bool pagesaver(webpage_t *page, char* pageDir, int id);
void pagescanner(webpage_t *page, hashtable_t *visited, bag_t *bag, int depth);

void bag_web_print(FILE *fp, void *item) {
    webpage_t *web = (webpage_t*) item;
    fprintf(fp, "%s", webpage_getURL(web));
}

int main(int argc, char* argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Insufficient number of arguments\n");
    }
    if (argc > 4) {
        fprintf(stderr, "Too many arguments provided\n");
    }
    // Normalize seedURL
    char* seedURL = argv[1];
    if (!NormalizeURL(seedURL)) {
        fprintf(stderr, "Error normalizing seedURL %s\n", seedURL);
        return 1;
    }
    // Making a copy on the heap
    char* urlcopy = assertp(malloc(strlen(seedURL)+1), "url copy");
    strcpy(urlcopy, seedURL);

	webpage_t *web = webpage_new(urlcopy, 0, NULL); 
    if (!inputCheck(web, argv[2], atoi(argv[3]))) {
        return 2;
    }
    crawler(web, argv[2], atoi(argv[3]));
    webpage_delete(web);
    return 0;
} 

bool inputCheck(webpage_t *seedURL, char* pageDir, int maxDepth)
{
    if (seedURL == NULL) {
        fprintf(stderr, "seedURL is NULL\n");
        return false;
    }
    if (!IsInternalURL(webpage_getURL(seedURL))) {
        fprintf(stderr, "seedURL is not an internal URL\n");
        return false;
    }
    if (pageDir == NULL) {
        fprintf(stderr, "pageDir is NULL\n");
        return false;
    }
    if (opendir(pageDir) == NULL) {
        fprintf(stderr, "pageDir is not a valid directory\n");
        return false;
    }
    if (maxDepth < 0 || maxDepth > 10) {
        fprintf(stderr, "maxDepth must be within the range [0,10]\n");
        return false;
    }
    return true;
}

void crawler(webpage_t* seedURL, char* pageDir, int maxDepth)
{
    if (maxDepth < 0) {
        return;
    }
    // Initializing data structures
    bag_t *bag = bag_new();
    hashtable_t *visited = hashtable_new(10);
    int currDepth = 0;
    int id = 1;

    bag_insert(bag, seedURL);
    hashtable_insert(visited, webpage_getURL(seedURL), &currDepth);
	
    webpage_t* item;
    while ((item = bag_extract(bag)) != NULL) {
        if (pagefetcher(item) == NULL)
            continue;
        if (pagesaver(item, pageDir, id)) {
            id++;
        }
        else {
            break;
        }
        
        if (webpage_getDepth(item) < maxDepth) {
            pagescanner(item, visited, bag, webpage_getDepth(item));
            // currDepth = webpage_getDepth(item);
        }
    }
    bag_delete(bag, webpage_delete);
    hashtable_delete(visited, NULL);
}


char* pagefetcher(webpage_t *page)
{
    if (webpage_fetch(page)) { 
		return webpage_getHTML(page);
	}
	else { // failed to fetch the page
		fprintf(stderr, "failed to fetch %s\n", webpage_getHTML(page));
		webpage_delete(page);
		return NULL;
	}
}
		
void pagescanner(webpage_t *page, hashtable_t *visited, bag_t *bag, int depth)
{
    if (page == NULL) {
        fprintf(stderr, "pagescanner received invalid webpage\n");
    }
    char* next;
    int pos = 0;
    while ((next = webpage_getNextURL(page, &pos)) != NULL) {
        // Normalize next URL
        if (!NormalizeURL(next)) {
            fprintf(stderr, "Error normalizing next URL %s\n", next);
            continue;
        }
        if (!IsInternalURL(next)) {
            continue;
        }
        if (hashtable_insert(visited, next, &depth)) {
            
            // Making a copy on the heap
            char* urlcopy = assertp(malloc(strlen(next)+1), "url copy");
            strcpy(urlcopy, next);

            webpage_t *web = webpage_new(urlcopy, depth+1, NULL); 
            bag_insert(bag, web);
            debug("adding into queue");

            char* num = malloc(sizeof(webpage_getDepth(web)));
            sprintf(num, "%d", webpage_getDepth(web));
            debug("depth:");
            debug(num);
            free(num);
        }
        free(next);
    }
}

// outputs a page to the the appropriate file
bool pagesaver(webpage_t *page, char* pageDir, int id) 
{
    char* num = malloc(sizeof(id)+1);
    sprintf(num, "%d", id);
    char* copy = malloc(strlen(pageDir)+1+sizeof(id));
    strcpy(copy, pageDir);
    strcat(copy, num);
    debug(num);

    FILE *fp = fopen(copy, "w"); 
	assertp(fp, "cannot open file for writing\n");
	fprintf(fp, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));

	fclose(fp);
    free(num);
    free(copy);
    return true;
}
