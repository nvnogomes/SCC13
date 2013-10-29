#include <cmath>
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <string>

#include "Indexer.h"
#include "Aggregator.h"
#include "Merger.h"
#include "config.h"


/**
 * list of tst files of Eça
 */
const int FILE_COUNT = 8;
const std::string FILES[] = {
    "../data/Eca-Queiros/pg16384.txt",
    "../data/Eca-Queiros/pg17515.txt",
    "../data/Eca-Queiros/pg18220.txt",
    "../data/Eca-Queiros/pg22015.txt",
    "../data/Eca-Queiros/pg23145.txt",
    "../data/Eca-Queiros/pg25641.txt",
    "../data/Eca-Queiros/pg31347.txt",
    "../data/Eca-Queiros/pg31971.txt"
};


/**
 * index all the Eça files
 *
 * @brief run_index
 * @param files
 */
void run_index() {

    for (int index = 0; index < FILE_COUNT; ++index) {
        Indexer i( FILES[index] );
        i.run();
        i.save();
    }
}


/**
 *
 *
 * @brief run_random
 * @param file_list
 */
void run_random(std::vector<std::string> file_list) {

    if( file_list.size() == 1 ) {
        return;
    }

    std::vector<std::string> files_remaining;
    bool is_odd = false;
    int files_left = file_list.size();
    if( file_list.size() % 2 != 0 ) {
        files_left -= 1;
        is_odd = true;
    }

    for(int i = 0; i < files_left; i += 2) {
        std::cout << file_list[i] << " " << file_list[i+1] << std::endl;
        files_remaining.push_back(file_list[i]);
    }

    if( is_odd ) {
        files_remaining.push_back(file_list.back());
    }

    run_random(files_remaining);
}


/**
 * Merge all the index files in the output directory
 *
 * @brief test_indexmerge
 * @param argc
 * @param argv
 */
void run_merge() {

    Merger mg;
    mg.run();
}


/**
 * @brief run_aggregator
 */
void run_aggregator() {

    Aggregator agg;
    agg.run();
}


/**
 * @brief testindexes
 */
void testindexes() {
    std::string my_str[] = {"1","2","3","4","5","6","7","8","9","10","11"};
    std::vector<std::string> pooey (my_str, my_str + sizeof(my_str) / sizeof(std::string) );

    run_random(pooey);
}


/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char** argv) {
    std::cout << "Running sandbox..." << std::endl;

    run_index();
    run_merge();
    run_aggregator();

    return 0;
}



