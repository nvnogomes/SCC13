# Condor compiler version: 3.4.4 (current 4.7)
CC = g++

# mpi compiler wrap
MPICC = mpiCC

# flags for showing warnings and be utf8 aware
CFLAGS  = -g -Wall

# c++11
STDFLAG = -std=c++0x 



.IGNORE:                     # ignore problems (as far as possible)
.SILENT:                     # don't echo commands executed

all: index agg mpi

index: Indexer.h
	$(CC) $(STDFLAG) $(CFLAGS) -o ../bin/indexer -c Indexer.cpp

agg: Aggregator.h
	$(CC) $(STDFLAG) $(CFLAGS) -o ../bin/aggregator -c Aggregator.cpp

mpi: Indexer.h Indexer.cpp Aggregator.h Aggregator.cpp
	$(MPICC) $(CFLAGS) -o ../bin/mpi -c MPI_Index.cpp


lab: ../bin/indexer ../bin/aggregator ../bin/mpi
	$(MPICC) $(CFLAGS) -o lab ../bin/indexer ../bin/aggregator ../bin/mpi


