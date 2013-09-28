#include <cmath>
#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <sstream>
#include <string>

#include "Indexer.h"
#include "Aggregator.h"
#include "Merger.h"


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


void run_index(int files) {

    for (int index = 0; index < files; ++index) {
        Indexer i( FILES[index], "../output/");
        i.run();
        i.save();
        std::cout << i.get_filename() << " " << i.histogram_size() << std::endl;
    }
}


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

void test_indexmerge(int argc, char** argv) {
    int index_count = 1;
    if( argc > 1 ) {
        index_count = atoi(argv[1]);
    }
    run_index(index_count);
    Merger mg ("../output/");
    mg.run();
}


void testindexes() {
    std::string my_str[] = {"1","2","3","4","5","6","7","8","9","10","11"};
    std::vector<std::string> pooey (my_str, my_str + sizeof(my_str) / sizeof(std::string) );

    run_random(pooey);
}


int main(int argc, char** argv) {
    std::cout << "Init sandbox" << std::endl;

    Aggregator ag("../output/");
    ag.run();

    return 0;
}



