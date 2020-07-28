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
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../libcs50/memory.h"

bool inputCheck(webpage_t *seedURL, char* pageDir, int maxDepth);
void crawler(webpage_t* seedURL, char* pageDir, int maxDepth);
char* pagefetcher(webpage_t *page);
bool pagesaver(webpage_t *page, char* pageDir, int id);
void pagescanner(webpage_t *page, hashtable_t *visited, int depth);

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
    // TODO: check whether directory exists
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
    int id = 0;

    bag_insert(bag, seedURL);
    hashtable_insert(visited, webpage_getURL(seedURL), &currDepth);
	
    webpage_t* item;
    while ((item = bag_extract(bag)) != NULL) {
        sleep(60);
        pagefetcher(item);
        if (pagesaver(item, pageDir, id)) {
            id++;
        }
        else {
            break;
        }
        
        if (currDepth < maxDepth) {
            pagescanner(item, visited, currDepth);
            currDepth++;
        }
    }
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
		
void pagescanner(webpage_t *page, hashtable_t *visited, int depth)
{
    if (page == NULL) {
        fprintf(stderr, "pagescanner received invalid webpage\n");
    }
    char* next;
    while ((next = webpage_getNextURL(page, 0)) != NULL) {
        // Normalize next URL
        if (!NormalizeURL(next)) {
            fprintf(stderr, "Error normalizing next URL %s\n", next);
            return;
        }
        if (!IsInternalURL(next)) {
            continue;
        }
        if (hashtable_insert(visited, next, &depth)) {
            printf("TODO: e12");
            // 12. make a new *webpage* for that URL, at depth+1
	        // 13. add the new webpage to the *bag* of webpages to be crawled
        }
    }


}

// outputs a page to the the appropriate file
bool pagesaver(webpage_t *page, char* pageDir, int id) 
{
    // TODO: pageDir/id
    char* num = malloc(40);
    sprintf(num, "%d", id);
    FILE *fp = fopen(strcat(pageDir, num), "w"); 
	assertp(fp, "cannot open file for writing\n");

	fprintf(fp, "%s\n%d\n%s", webpage_getURL(page), id, webpage_getHTML(page));

	fclose(fp);
    free(num);
    return true;
}
