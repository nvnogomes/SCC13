# Condor compiler version: 3.4.4 (current 4.7)
CC = g++

# mpi compiler wrap
MPICC = mpiCC

# flags for showing warnings and be utf8 aware
CFLAGS  = -g -Wall

# c++11
STDFLAG = -std=c++0x 



.IGNORE:	# ignore problems (as far as possible)
.SILENT:	# don't echo commands executed

all: index agg mpi

index:
	$(CC) $(STDFLAG) $(CFLAGS) -o ../bin/indexer.o -c Indexer.cpp

agg:
	$(CC) $(STDFLAG) $(CFLAGS) -o ../bin/aggregator.o -c Aggregator.cpp

mpi:
	$(MPICC) $(CFLAGS) -o ../bin/mpi ../bin/indexer.o ../bin/aggregator.o MPI_Index.cpp



