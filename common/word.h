/**
 * word.h -- header file for CS50 'word' module
 * 
 * the 'word' module contains methods that need to be shared by crawler, 
 * indexer, and querier. More specifically, 'word' currently supports one 
 * method to normalize a given word (changing the word to lower case)
 * 
 * Irene Lam, August 7, 2020
 * Dartmouth CS50, Summer 2020
 */
#include <ctype.h>
#include <string.h>

/**************** normalizeWord ****************/
/* Changes the given words to all-lower case and returns this word
 * Input: char* representing a word
 * Output: lower-case version of the wrod
 */
char* normalizeWord(char* original); 