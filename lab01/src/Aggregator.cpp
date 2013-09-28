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


Aggregator::Aggregator(std::string dir):
    directory_( dir )
{
}


Aggregator::~Aggregator(){}


void *merge(void *file) {

    struct Files *f = (struct Files *) file;

    std::cout << "COPYING " << f->begin << " < " << f->end << std::endl;

    const int LEN=8192;
    char buffer_out[LEN];
    char buffer_in[LEN];

    std::ifstream src;
    std::ofstream dest;

    src.rdbuf()->pubsetbuf(buffer_in, LEN );
    dest.rdbuf()->pubsetbuf(buffer_out, LEN);

    src.open(f->begin, std::ios::in | std::ios::binary);
    dest.open(f->end, std::ios::out | std::ios::app | std::ios::binary);
    dest << src.rdbuf();
    src.sync();

    pthread_exit(NULL);
}


void Aggregator::merge_cycle(std::vector<std::string> file_list) {

    std::cout << file_list.size() << " ";
    std::cout << file_list[0] << std::endl;

    if( file_list.size() < 2 ) {
        return;
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
        indexes.begin = file_list[i];
        indexes.end = file_list[i+1];
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




std::vector<std::string> Aggregator::list_directory() {
    DIR *search_directory;
    struct dirent *entry;
    std::vector<std::string> list;

    search_directory = opendir( this->directory_.c_str() );

    if( search_directory == NULL ) {
        printf("Error opening directory: %s", this->directory_.c_str());
        exit(1);
    }

    while( (entry = readdir( search_directory )) ){

        if( entry->d_type == DT_REG ){
            std::string filename = entry->d_name;
            if( filename.find( ".index" ) != std::string::npos
                    && filename.size() == 7) {
                list.push_back( this->directory_ +""+ filename );
            }
        }
    }
    std::sort(list.begin(), list.end());

    closedir( search_directory );

    return list;
}





void Aggregator::run() {
    std::vector<std::string> file_list = list_directory();

    this->merge_cycle(file_list);
}
