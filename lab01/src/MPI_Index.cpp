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
#include "Merger.h"
#include "config.h"


/* files */
std::vector<std::string> data_files;
std::vector<std::string> partial_index;



/**
 * @brief list_data_directory searches the given directory for files with the
 *  filename name
 * @param directory
 * @param filename
 * @return
 */
std::vector<std::string>
list_data_directory(std::string directory, std::string filename) {

    DIR *data_directory;
    struct dirent *entry;
    std::vector<std::string> files;


    data_directory = opendir( directory.c_str() );

    if( data_directory == NULL ) {
        printf("Error: Cannot open directory: %s", directory.c_str() );
        exit(1);
    }

    while( (entry = readdir( data_directory )) ){

        if( entry->d_type == DT_REG ){
            std::string fname = entry->d_name;
            if( fname.find( filename ) != std::string::npos) {

                files.push_back( directory + entry->d_name );
            }
        }
    }
    closedir( data_directory );

    return files;
}

/**
 * @brief list_partial_index_files sets all partial index files
 * @param directory
 */
static void list_partial_index_files(std::string directory) {

    // bootstrap
    if( partial_index.size() < 26 ) {
        std::string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        for (int i = 0; i < 26; ++i) {
            std::ofstream outfile ( alpha.at(i) + PARTIAL_EXTENSION);
            outfile << "" << std::endl;
            outfile.close();
        }
    }

    partial_index = list_data_directory(directory, (std::string)PARTIAL_EXTENSION);
}


/**
 * @brief list_data_directory sets all the source data files
 * @param directory
 */
static void list_data_files(std::string directory) {

    data_files = list_data_directory(directory, ".txt");
}


/**
 * @brief getCurrentDate
 * @return
 */
std::string getCurrentDate() {
    time_t rawtime;
    struct tm *timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    return asctime (timeinfo);
}


/**
 * @brief processResults
 */
static void aggregate_results()
{
    Aggregator agg;
    agg.run();
}


/**
 * @brief index_file
 * @param file_index
 * @return
 */
static int index_file(int file_index)
{

    if( DEBUG ) {
        std::cout << "indexing (" << file_index << ") ";
        std::cout<< data_files[file_index] << std::endl;
    }

    std::string filename = data_files[file_index];

    Indexer ind ( filename );
    ind.run();

    return 0;
}


/**
 * @brief merge_file
 * @param file_index
 * @return
 */
static int merge_file(int file_index)
{

    if( DEBUG ) {
        std::cout << "merging (" << file_index << ") ";
        std::cout << partial_index[file_index] << std::endl;
    }


    std::string filename = partial_index[file_index];

    Merger mg ( filename );
    mg.run();

    return 0;

}


/**
 * @brief layoff_slaves
 */
void layoff_slaves()
{
    int ntasks, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    for (rank = 1; rank < ntasks; ++rank) {
        MPI_Send(0,
                 0,
                 MPI_INT,
                 rank,
                 DIE_TAG,
                 MPI_COMM_WORLD);
    }
}


/**
 * @brief slave_cycle
 * @param work_tag
 */
void slave_cycle(int work_tag, int load)
{
    if( DEBUG )
        std::cout << "Slave cycle " << work_tag << " (" << load << ")" << std::endl;

    int ntasks, rank, result;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    int counter = 0;
    int received = 0;

    // assign work to all processes
    for (rank = 1; rank < ntasks; ++rank) {

        MPI_Send( &counter,
                 1,
                 MPI_INT,
                 rank,
                 work_tag,
                 MPI_COMM_WORLD);

        if( DEBUG)
            std::cout << "Master: W" << counter << " sent to S" << rank << std::endl;

        counter++;
    }

    /* when the #work is bigger than #process we will
     * give more work to the process
     * this will be repeated until there is work to be done */
    while ( counter < load ) {

        /* Receive results from a slave */
        MPI_Recv(&result,
                 1,
                 MPI_INT,
                 MPI_ANY_SOURCE,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &status);
        received++;

        if( DEBUG ) {
            std::cout << "Slave : S" << status.MPI_SOURCE << " finished.";
            std::cout << " (" << received << ")" << std::endl;
        }

        /* Send the slave a new work unit */
        MPI_Send(&counter,
                 1,
                 MPI_INT,
                 status.MPI_SOURCE,
                 work_tag,
                 MPI_COMM_WORLD);

        if( DEBUG )
            std::cout << "Master: W" << counter << " sent to S" << status.MPI_SOURCE << std::endl;

        counter++;
    }

    // receive all distributed tasks
    while( received < load ) {
        MPI_Recv(&result,
                 1,
                 MPI_INT,
                 MPI_ANY_SOURCE,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &status);
        received++;

        if( DEBUG ) {
            std::cout << "Slave : S" << status.MPI_SOURCE << " finished.";
            std::cout << " (" << received << ")" << std::endl;
        }
    }
}


/**
 * @brief master
 */
static void master(void)
{
    int ntasks;
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);

    if( DEBUG )
        std::cout << getCurrentDate()
        <<"Running with " << ntasks << " processes" << std::endl;

    // index
    slave_cycle(INDEX_TAG, data_files.size());

    // merge
    list_partial_index_files( OUTPUT_DIRECTORY );
    slave_cycle(MERGE_TAG, partial_index.size());

    // kill slaves
    layoff_slaves();

    // aggregate
    aggregate_results();
}


static void slave(void)
{
    int file_index;
    int result;
    MPI_Status status;

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    while (1) {

        MPI_Recv(&file_index,
                 1,
                 MPI_INT,
                 0,
                 MPI_ANY_TAG,
                 MPI_COMM_WORLD,
                 &status);


        switch( status.MPI_TAG ) {
            case DIE_TAG:
                return;

            case INDEX_TAG:
                result = index_file( file_index );

                MPI_Send(&result,
                         1,
                         MPI_INT,
                         0,
                         0,
                         MPI_COMM_WORLD);
                break;

            case MERGE_TAG:
                result = merge_file( file_index );

                MPI_Send(&result,
                         1,
                         MPI_INT,
                         0,
                         0,
                         MPI_COMM_WORLD);
                break;

            default:
                std::cerr << "Unexpected task! Terminating..." << std::endl;
                break;

        }

    }
}


/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv)
{
    int myrank;
    double start, finish;

    MPI_Init(&argc, &argv);

    list_data_files( DATA_SET_3 );
    list_partial_index_files( OUTPUT_DIRECTORY );


    MPI_Datatype MPIString;
    MPI_Type_contiguous(16, MPI::CHAR, &MPIString);
    MPI_Type_commit( &MPIString );

    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    if (myrank == 0) {
        start = MPI_Wtime();
        master();
        finish = MPI_Wtime();

        std::cout << "Time elapsed: " << finish - start << "s" <<std::endl;

    } else {
        slave();
    }

    /* Shut down MPI */
    MPI_Finalize();

    return 0;
}
