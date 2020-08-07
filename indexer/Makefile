# Makefile for 'indexer' module
#
# Irene Lam, August 3, 2020
# CS50, Summer 2020

PROG = indexer indextest
OBJS = indexer.o indextest.o
# LIBS = ../libcs50/memory.o ../libcs50/bag.o ../libcs50/hashtable.o ../libcs50/webpage.o ../libcs50/set.o ../libcs50/jhash.o ../libcs50/file.o
LIBS = ../common/common.a ../libcs50/libcs50.a 

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../common -I../libcs50  
CC = gcc 
MAKE = make

# $(PROG): $(OBJS) $(LIBS)
# 	$(CC) $(CFLAGS) $^ -o $@

# $(PROG): $(OBJS)
# 	$(CC) $(CFLAGS) $^ -I../common/common.a -I../libcs50/libcs50.a 
	# -I../libcs50/webpage.o -I../libcs50/file.o -o $@

# indexer.o: ../libcs50/bag.h ../libcs50/hashtable.h ../libcs50/webpage.h ../libcs50/memory.h \
 ../common/pagedir.h



# readline.o: ../libcs50/readlinep.h

	# $(CC) $(CFLAGS) $^ $(LIBS) -o $@
# # memory.o: ../libcs50/memory.h


.PHONY: test valgrind clean all

all: $(PROG)

indexer: indexer.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

# indexer.o: ../common/index.h ../common/pagedir.h ../common/word.h

indextest: indextest.o $(LIBS)
	$(CC) $(CFLAGS) $^ -o $@

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
