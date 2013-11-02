# Condor compiler version: 3.4.4 (current 4.7)
CC = g++

# mpi compiler wrap
MPICC = mpic++

# flags for showing warnings and be utf8 aware
CFLAGS  = -g -Wall

# c++11
STDFLAG = -std=c++11 

# POSIX THREADS
PTHREADS = -lpthread


.IGNORE:	# ignore problems (as far as possible)
.SILENT:	# don't echo commands executed


all: index agg merger sandbox mpi
clean: clean_output clean_bin
alpha: index sandbox
beta: merger sandbox
delta: agg sandbox


index:
	$(CC) $(STDFLAG) $(CFLAGS) -o ../bin/indexer.o -c Indexer.cpp

agg:
	$(CC) $(STDFLAG) $(PTHREADS)  $(CFLAGS) -o ../bin/aggregator.o -c Aggregator.cpp

merger:
	$(CC) $(STDFLAG) $(CFLAGS) -o ../bin/merger.o -c Merger.cpp

mpi:
	$(MPICC) $(CFLAGS) -o ../bin/mpi ../bin/indexer.o ../bin/aggregator.o ../bin/merger.o MPI_Index.cpp


sandbox:
	$(CC) $(PTHREADS) $(CFLAGS) -o ../bin/sandbox ../bin/aggregator.o ../bin/indexer.o ../bin/merger.o sandbox.cpp




clean_output:
	cd ../output/; $(RM) *

clean_bin:
	cd ../bin/; $(RM) *

