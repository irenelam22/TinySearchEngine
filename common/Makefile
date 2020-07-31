# Makefile for 'common' module
#
# Irene Lam, July 28, 2020
# CS50, Summer 2020

# PROG = pagedir
OBJS = pagedir.o
LIBS = ../libcs50/libcs50.a
LIB = common.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I ../libcs50 -DTEST
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# common.o: ../libcs50/libcs50.a pagedir.h
# readline.o: ../libcs50/readlinep.h
pagedir.o: pagedir.h

.PHONY: test valgrind clean

valgrind: $(PROG) 
	valgrind ./$(PROG) 

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)

