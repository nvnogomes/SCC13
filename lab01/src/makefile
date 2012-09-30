# Condor compiler version: 3.4.4 (current 4.7)
CC 		= g++

# flags for showing warnings and be utf8 aware
CFLAGS  = -std=c++0x -g -Wall


.IGNORE:                     # ignore problems (as far as possible)
.SILENT:                     # don't echo commands executed

all: index agg

index:
	$(CC) $(CFLAGS) -o ../bin/indexer M_Indexer.cpp

agg:
	$(CC) $(CFLAGS) -o ../bin/aggregator M_Aggregator.cpp
