/**
 * pagedir.h -- header file for CS50 'pagedir' module
 * 
 * 'Pagedir' module contains methods that need to be shared by crawler, 
 * indexer, and querier. Currently, it only contains methods relevant
 * to crawler.
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