# Makefile for 'common' module
#
# Irene Lam, July 28, 2020
# CS50, Summer 2020

PROG = common
OBJS = common.o
LIBS = ../libcs50/libcs50.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../lib
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

common.o: ../libcs50/libcs50.a
readline.o: ../libcs50/readlinep.h

.PHONY: test valgrind clean

test:
	bash -v testing.sh

valgrind: $(PROG) 
	valgrind ./$(PROG) 

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)

