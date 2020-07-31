/**
 * pagedir.h -- header file for CS50 'pagedir' module
 * 
 * 'Pagedir' module contains methods that need to be shared by crawler, 
 * indexer, and querier. More specifically, 'pagedir' helps the crawler 
 * write pages to the pageDirectory and help the indexer read them back in
 * 
 * Irene Lam, July 28, 2020
 * Dartmouth CS50, Summer 2020
 */

#include <stdio.h>

/**************** isDirectory ****************/
/**
 * Checks whether the input string is a valid, writable directory
 * Input: char*, presumably to a directory
 * Ouput: boolean signifiying whether input is a valid, writable directory
 */
bool isDirectory(char* directory);

/**************** pagefetcher ****************/
/**
 * pagefetcher -- fetches the contents (HTML) for a page from a URL and returns
 * the HTML contents for that given page
 * Input: webpage
 * Output: HTML of the given webpage
 */
char* pagefetcher(webpage_t *page);

/**************** pagesaver ****************/
/**
 * pagesaver -- outputs a page to the appropriate file
 * Input: webpage, (char*) page directory, (int) unique id
 * Output: true if pages successfully outputted into respective file,
 *         false otherwise
 */
bool pagesaver(webpage_t *page, char* pageDir, int id);