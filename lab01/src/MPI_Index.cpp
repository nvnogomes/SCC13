#include <dirent.h>
#include <vector>
#include <mpi.h>
#include "Aggregator.h"
#include "Indexer.h"


#define WORKTAG 1
#define DIETAG 2

/*
    to execute
    mpiexec -n #process binary
*/


/* files */
std::vector< std::string > fileList;
int fileEntry;



void listDocDirectory(std::string directory) {
    DIR *searchDirectory = opendir( directory.c_str() );
    dirent* entry;

    while( (entry = readdir( searchDirectory )) ){

        if( entry->d_type == DT_REG ){
            std::string fname = entry->d_name;
            if( fname.find( ".txt" ) != std::string::npos) {

                fileList.push_back( entry->d_name );
            }
        }
    }

    fileEntry = 0;
}


static int
getNextWorkItem(void) {

    if( ++fileEntry < (int)fileList.size() ) {
        return fileEntry;
    }
    else {
        return -1;
    }

}


static void
processResults() {

    Aggregator agg;
    agg.run();
    agg.outputAggregateToFile( "aggResult.txt" );

}


static int
doWork(int filenameIndex) {

    Indexer ind ( fileList[filenameIndex] );
    ind.run();
    ind.outputAggregateToFile();
    return 0;
}



static void
master(void) {
    int ntasks, rank;
    int work;
    int result;
    MPI_Status status;

    /* Find out how many processes there are in the default communicator */
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    /* Seed the slaves; send one unit of work to each slave. */
    for (rank = 1; rank < ntasks; ++rank) {

        /* Find the next item of work to do */
        work = getNextWorkItem();

        /* Send it to each rank */
        MPI_Send(&work,             /* message buffer */
                 1,                 /* one data item */
                 MPI_INT,           /* data item is an integer */
                 rank,              /* destination process rank */
                 WORKTAG,           /* user chosen message tag */
                 MPI_COMM_WORLD);   /* default communicator */
    }

    /* Loop over getting new work requests until there is no more work
     to be done */
    work = getNextWorkItem();
    while ( work != -1 ) {

        /* Receive results from a slave */
        MPI_Recv(&result,           /* message buffer */
                 1,                 /* one data item */
                 MPI_INT,        /* of type string */
                 MPI_ANY_SOURCE,    /* receive from any sender */
                 MPI_ANY_TAG,       /* any type of message */
                 MPI_COMM_WORLD,    /* default communicator */
                 &status);          /* info about the received message */

        /* Send the slave a new work unit */
        MPI_Send(&work,             /* message buffer */
                 1,                 /* one data item */
                 MPI_INT,           /* data item is an integer */
                 status.MPI_SOURCE, /* to who we just received from */
                 WORKTAG,           /* user chosen message tag */
                 MPI_COMM_WORLD);   /* default communicator */


        work = getNextWorkItem();
    }

    /* Tell all the slaves to exit by sending an empty message with the DIETAG. */
    for (rank = 1; rank < ntasks; ++rank) {
        MPI_Send(0, 0, MPI_INT, rank, DIETAG, MPI_COMM_WORLD);
    }

    /* There's no more work to be done, so receive all the outstanding
     results from the slaves. */
    for (rank = 1; rank < ntasks; ++rank) {
        MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE,
                 MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }


    processResults();
}


static void
slave(void) {
    int filenameIndex;
    int result;
    MPI_Status status;


    while (1) {

        MPI_Recv(&filenameIndex, 1, MPI_INT, 0, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);

        if (status.MPI_TAG == DIETAG) {
            return;
        }

        result = doWork( filenameIndex );

        MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
}




int
main(int argc, char **argv) {
    int myrank;

    listDocDirectory( "../data/Eca-Queiros/" );

    MPI_Init(&argc, &argv);

    MPI_Datatype MPIString;
    MPI_Type_contiguous(16, MPI::CHAR, &MPIString);
    MPI_Type_commit( &MPIString );

    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    if (myrank == 0) {
        master();
    } else {
        slave();
    }

    /* Shut down MPI */
    MPI_Finalize();
    return 0;
}
