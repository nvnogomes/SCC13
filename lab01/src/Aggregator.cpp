#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>

#include "Aggregator.h"
#include "config.h"


/**
 * @brief Aggregator::Aggregator
 */
Aggregator::Aggregator():
    directory_( OUTPUT_DIRECTORY )
{
}


/**
 * @brief Aggregator::Aggregator
 * @param dir
 */
Aggregator::Aggregator(std::string dir):
    directory_( dir )
{
}


/**
 * @brief Aggregator::~Aggregator
 */
Aggregator::~Aggregator(){}


/**
 * @brief merge
 * @param file
 * @return
 */
void *merge(void *file) {

    struct Files *f = (struct Files *) file;

    if( DEBUG ) {
        std::cout << "APPENDING " << f->base << " with " << f->append << std::endl;
    }

    const int LEN=8192;
    char buffer_out[LEN];
    char buffer_in[LEN];

    std::ofstream src;
    std::ifstream app;

    src.rdbuf()->pubsetbuf(buffer_out, LEN );
    app.rdbuf()->pubsetbuf(buffer_in, LEN);

    app.open(f->append, std::ios::in | std::ios::binary);
    src.open(f->base, std::ios::out | std::ios::app | std::ios::binary);
    src << app.rdbuf();
    app.sync();
}


/**
 * @brief Aggregator::merge_cycle
 * @param file_list
 */
void Aggregator::merge_cycle(std::vector<std::string> file_list) {

    if( file_list.size() < 2 ) {
        return;
    }

    if( DEBUG ) {
        std::cout << file_list.size() << " " << file_list[0] << std::endl;
    }

    std::vector<std::string> files_remaining;
    bool is_odd = false;
    int files_left = file_list.size();
    if( file_list.size() % 2 != 0 ) {
        files_left -= 1;
        is_odd = true;
    }

    int num_threads = files_left / 2;
    int thread_counter = 0;
    pthread_t thread[ num_threads ];

    for(int i = 0; i < files_left; i+= 2) {
        struct Files indexes;
        indexes.base = file_list[i];
        indexes.append = file_list[i+1];
        pthread_create(&thread[thread_counter], NULL, merge, (void*)&indexes);
        files_remaining.push_back(file_list[i]);
        thread_counter++;
    }

    void *status;
    for(int i = 0; i < num_threads; i++) {
        pthread_join(thread[i], &status);
    }

    if( is_odd ) {
        files_remaining.push_back(file_list.back());
    }

    merge_cycle(files_remaining);
}



/**
 * List directory
 *
 * @brief Aggregator::list_directory
 * @return
 */
std::vector<std::string> Aggregator::list_directory() {

    if( DEBUG ) {
        std::cout << " reading directory " << this->directory_ << ".. " ;
    }

    DIR *search_directory;
    struct dirent *entry;
    std::vector<std::string> list;

    list.push_back( this->directory_ +""+ GLOBAL_INDEX_FILE );

    search_directory = opendir( this->directory_.c_str() );

    if( search_directory == NULL ) {
        printf("Error opening directory: %s", this->directory_.c_str());
        exit(1);
    }

    while( (entry = readdir( search_directory )) ){

        if( entry->d_type == DT_REG ){
            std::string filename = entry->d_name;
            if( filename.find( PARTIAL_EXTENSION ) != std::string::npos ) {
                list.push_back( this->directory_ +""+ filename );
            }
        }
    }
    std::sort(list.begin(), list.end());

    closedir( search_directory );

    if( DEBUG ) { std::cout << list.size() << " files found" << std::endl; }

    return list;
}


/**
 * Rename final index file and erase temporary indexes.
 * @brief Aggregator::cleanup_directory
 */
void Aggregator::cleanup_directory(std::vector<std::string> files) {

    if( DEBUG ) {
        std::cout << "Cleaning up " <<  this->directory_ << " directory" <<std::endl;
    }

    std::vector<std::string>::iterator it = files.begin();

    std::cout << it->c_str() << " " << ((std::string)GLOBAL_INDEX_FILE).c_str() << std::endl;
//    rename( files[0].c_str(), ((std::string)GLOBAL_INDEX_FILE).c_str() );

    it++;
//    for(; it != files.end(); it++) {
//        remove( it->c_str() );
//    }

}



/**
 * @brief Aggregator::run
 */
void Aggregator::run() {

    if( DEBUG ) {
        std::cout << "Running Aggregator..." << std::endl;
    }

    std::vector<std::string> file_list = list_directory();

    this->merge_cycle(file_list);

    this->cleanup_directory(file_list);

}
