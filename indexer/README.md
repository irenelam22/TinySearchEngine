Assume filename doesn't end in a backslash (since it's a filename)

/*
indexer - first!
read in page dir with pages from crawler
call index_build -> takes a page dir and populate an index data structure with all pages from directory
    builds index data structure index_t*


index.c
index_save -> saves the index data structure into a readable file (takes the new file name)

index_load -> takes an index file (from index_save) and creates a index data structure
called in indextest

indextest read index file from indexer (call indexer first), use inxex_load to recreate index data structure
save index data sturcture to another file

testing.sh - random tests
*/

reminders: change hashtable size at end