#include <dirent.h>
#include <vector>
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include "Aggregator.h"
#include "Indexer.h"


#define WORK_TAG 1
#define DIE_TAG 2
#define DATA_DIRECTORY "data/Eca-Queiros/"
//#define DATA_DIRECTORY "data/Wiki-1k/"
#define OUTPUT_DIRECTORY "output/"
#define TRACE true



/* files */
std::vector< std::string > fileList;
int fileEntry;
std::ostringstream buf ("");



void listDocDirectory(std::string directory) {
    DIR *searchDirectory;
    struct dirent *entry;

    searchDirectory = opendir( directory.c_str() );

    if( searchDirectory == NULL ) {
        printf("Error: Cannot open directory: %s", directory.c_str() );
	exit(27);
    }

    while( (entry = readdir( searchDirectory )) ){

        if( entry->d_type == DT_REG ){
            std::string fname = entry->d_name;
            if( fname.find( ".txt" ) != std::string::npos) {

                fileList.push_back( directory + entry->d_name );
            }
        }
    }
    closedir( searchDirectory );

    fileEntry = 0;
}


std::string getCurrentDate() {
    time_t rawtime;
    struct tm *timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    return asctime (timeinfo);
}

static int
getNextWorkItem(void) {

    if( fileEntry < (int)fileList.size() ) {
        return fileEntry++;
    }
    else {
        return -1;
    }

}


static void
processResults() {

    std::string prefix = (std::string) OUTPUT_DIRECTORY;

    Aggregator agg( prefix );
    agg.run();

    agg.outputAggregateToFile();

    printf("GIndex Size: %i\n", agg.getAggSize() );

    buf << "Global Index size: "<< agg.getAggSize() << " entries." << std::endl;
}


static int
doWork(int filenameIndex) {

    std::string filename = fileList[filenameIndex];

    Indexer ind ( filename );
    ind.run();
    ind.setOutputDirectory( (std::string) OUTPUT_DIRECTORY );
    ind.outputAggregateToFile();

    return 0;
}



static void
master(void) {
    int ntasks, rank, work, result;
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    buf << getCurrentDate()
        <<"Running with " << ntasks << " processes" << std::endl;

    // assign work to all processes
    for (rank = 1; rank < ntasks; ++rank) {

        work = getNextWorkItem();
        MPI_Send(&work,
                 1,
                 MPI_INT,
                 rank,
                 WORK_TAG,
                 MPI_COMM_WORLD);

        buf << "Master: W" << work << " sent to S" << rank << std::endl;
    }

    /* when the #work is bigger than #process we will
     * give more work to the process
     * this will be repeated until there is work to be done */
    work = getNextWorkItem();
    while ( work != -1 ) {

        /* Receive results from a slave */
        MPI_Recv(&result,
                 1,
                 MPI_INT,
                 MPI_ANY_SOURCE,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &status);

        buf << "Slave : S" << status.MPI_SOURCE << " finished." << std::endl;

        /* Send the slave a new work unit */
        MPI_Send(&work,
                 1,
                 MPI_INT,
                 status.MPI_SOURCE,
                 WORK_TAG,
                 MPI_COMM_WORLD);

        buf << "Master: W" << work << " sent to S" << status.MPI_SOURCE << std::endl;

        work = getNextWorkItem();
    }

    for (rank = 1; rank < ntasks; ++rank) {
        MPI_Send(0,
                 0,
                 MPI_INT,
                 rank,
                 DIE_TAG,
                 MPI_COMM_WORLD);
    }

    for (rank = 1; rank < ntasks; ++rank) {
        MPI_Recv(&result,
                 1,
                 MPI_INT,
                 MPI_ANY_SOURCE,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &status);

        buf << "Slave : S" << status.MPI_SOURCE << " finished." << std::endl;
    }

    processResults();
}


static void
slave(void) {
    int filenameIndex;
    int result;
    MPI_Status status;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    while (1) {

        MPI_Recv(&filenameIndex,
                 1,
                 MPI_INT,
                 0,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &status);

        if (status.MPI_TAG == DIE_TAG) {
            return;
        }

        result = doWork( filenameIndex );

        MPI_Send(&result,
                 1,
                 MPI_INT,
                 0,
                 0,
                 MPI_COMM_WORLD);
    }
}

void
outputTrace() {
    std::string aux = (std::string) DATA_DIRECTORY;
    std::string dir = (std::string) OUTPUT_DIRECTORY;
    aux.erase( aux.length()-1, 1);
    int fileIndex = aux.rfind("/") + 1;
    int strLenght = aux.size() - fileIndex;
    std::string outputFile = dir +""+ aux.substr( 
fileIndex, strLenght ) + "_trace.txt";

    std::ofstream fileOut( outputFile.c_str(), std::ios_base::app );
    buf << std::endl;
    fileOut << buf.str();
    fileOut.close();
}


int
main(int argc, char **argv) {
    int myrank;
    double start, finish;

    MPI_Init(&argc, &argv);

    listDocDirectory( DATA_DIRECTORY );

    MPI_Datatype MPIString;
    MPI_Type_contiguous(16, MPI::CHAR, &MPIString);
    MPI_Type_commit( &MPIString );

    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    if (myrank == 0) {
        start = MPI_Wtime();
        master();
        finish = MPI_Wtime();

        buf << "Time elapsed: " << finish - start << "s" <<std::endl;

        if( TRACE ) { outputTrace(); }

    } else {
        slave();
    }

    /* Shut down MPI */
    MPI_Finalize();

    return 0;
}
