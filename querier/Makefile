# Makefile for 'indexer' module
#
# Irene Lam, August 15, 2020
# CS50, Summer 2020

PROG = querier fuzzquery
OBJS = querier.o fuzzquery.o
LIBS = ../common/common.a ../libcs50/libcs50.a 

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../common -I../libcs50  
CC = gcc 
MAKE = make

.PHONY: test clean all

all: $(PROG)

fuzzquery: fuzzquery.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

querier: querier.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

test:
	bash testing.sh &> testing.out

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
	rm -r ../querygarbage ../queryWithoutRead
