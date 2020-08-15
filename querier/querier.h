/* 
 * file utilities - reading a word, line, or entire file
 * 
 * David Kotz, 2016, 2017, 2019
 */

#ifndef __QUERIER_H
#define __QUERIER_H

#include <stdio.h>
#include <stdbool.h>
#include "../common/index.h"

// Function prototype
void query_print(void *arg, const int key, const int count);
bool inputCheck(char* dircopy, char* index);
int extract_words(char* line, char** words );
bool valid_query(char** query, int length);
void process_query(index_t* index, char* pagedir);
void sum_iterate(void *arg, const int key, const int count);
void min_iterate(void *arg, const int key, const int count);
void run_query(char** words, index_t* index, char* url) ;
void query_print(void *arg, const int key, const int count);

void selection_sort(counters_t* set, char* url);
void findLength(void *arg, const int key, const int count);
void selection_sort_helper(void *arg, const int key, const int count);

#endif // __QUERIER_H
