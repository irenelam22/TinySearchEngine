# Makefile for 'indexer' module
#
# Irene Lam, August 3, 2020
# CS50, Summer 2020

PROG = indexer
OBJS = indexer.o 
# LIBS = ../libcs50/memory.o ../libcs50/bag.o ../libcs50/hashtable.o ../libcs50/webpage.o ../libcs50/set.o ../libcs50/jhash.o ../libcs50/file.o
LIBS = ../libcs50/libcs50.a ../common/common.a ../libcs50/webpage.o

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../common -I../libcs50
CC = gcc 
MAKE = make

$(PROG): $(OBJS) ../libcs50/libcs50.a ../common/common.a ../libcs50/webpage.o ../libcs50/file.o
	$(CC) $(CFLAGS) $^  -o $@

# indecer.o: ../libcs50/bag.h ../libcs50/hashtable.h ../libcs50/webpage.h ../libcs50/memory.h \
 ../common/pagedir.h

all: indextest

readline.o: ../libcs50/readlinep.h
indextest.o: ../common/index.h
# memory.o: ../libcs50/memory.h
# indexer.o: indexer 

.PHONY: test valgrind clean

test:
#	bash -v testing.sh
	bash testing.sh 

# valgrind: $(PROG) 
# 	valgrind ./$(PROG) 

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f $(PROG)
	#rm -rf ../data* ../garbage*
