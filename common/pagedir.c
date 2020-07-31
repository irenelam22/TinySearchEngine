/**
 * pagedir.c -- a module to help the crawler write pages to the pageDirectory
 * and help the indexer read them back in
 * Currently only includes methods relevant to crawler
 * 
 * Please see pagedir.h for more details
 * 
 * Irene Lam, July 27, 2020
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

#ifdef TEST
void debug(char* input)
{
    printf("%s\n", input); 
}
#else 
void debug(char* input) {}
#endif

/**************** isDirectory ****************/
/*
 * Checks whether the input string is a valid, writable directory
 * Input: char*, presumably to a directory
 * Ouput: boolean signifiying whether input is a valid, writable directory
 */
bool isDirectory(char* directory)
{
    // Reject if directory is NULL
    if (directory == NULL) {
        fprintf(stderr, "directory is NULL\n");
        return false;
    }

    // Create memory in heap
    char* copy = assertp(malloc(strlen(directory)+10), "directory malloc failed");
    strcpy(copy, directory);
    
    strcat(copy, ".crawler");
    FILE* fp = fopen(copy, "w");
    free(copy);

    // Directory must exist and be writable to
    if (fp == NULL) {
        fprintf(stderr, "Directory either does not exist or is not writable\n");
        return false;
    }
    fclose(fp);
    return true;
}

/**************** pagefetcher ****************/
/**
 * pagefetcher -- fetches the contents (HTML) for a page from a URL
 * and returns the HTML contents for that given page
 * Input: webpage
 * Output: HTML of the given webpage
 */
char* pagefetcher(webpage_t *page)
{
    // Trying to fetch HTML contents
    if (webpage_fetch(page)) { 
		return webpage_getHTML(page);
	}
	else { 
        // failed to fetch the page
		// fprintf(stderr, "failed to fetch %s\n", webpage_getHTML(page));
        debug("Failed to fetch page");
		webpage_delete(page);
		return NULL;
	}
}

/**************** pagesaver ****************/
/**
 * pagesaver -- outputs a page to the appropriate file
 * Input: webpage, (char*) page directory, (int) unique id
 * Output: true if pages successfully outputted into respective file,
 *         false otherwise
 */
bool pagesaver(webpage_t *page, char* pageDir, int id) 
{
    // Allocate memory and copy
    char* num = malloc(sizeof(id)+1);
    sprintf(num, "%d", id);
    debug(num);
    char* copy = malloc(strlen(pageDir)+1+sizeof(id));
    strcpy(copy, pageDir);
    strcat(copy, num);

    // Save the webpage URL, depth, and HTML contents into unique file
    FILE *fp = fopen(copy, "w"); 
	assertp(fp, "cannot open file for writing\n");
	fprintf(fp, "%s\n%d\n%s", webpage_getURL(page), webpage_getDepth(page), webpage_getHTML(page));
    debug(webpage_getURL(page));

    // Maintaining logistics -- free and close files
	fclose(fp);
    free(num);
    free(copy);
    return true;
}
