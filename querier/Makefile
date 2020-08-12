# Makefile for 'indexer' module
#
# Irene Lam, August 3, 2020
# CS50, Summer 2020

PROG = querier fuzzquery
OBJS = querier.o fuzzquery.o
LIBS = ../common/common.a ../libcs50/libcs50.a 

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../common -I../libcs50  
CC = gcc 
MAKE = make

.PHONY: test valgrind clean all

all: $(PROG)

fuzzquery: fuzzquery.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

querier: querier.o query.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

queriertest: queriertest.o querier.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

test:
	queriertest
	#bash testing.sh &> testing.out

# valgrind: $(PROG) 
# 	valgrind ./$(PROG) 

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
	#rm -rf ../data* ../garbage*
