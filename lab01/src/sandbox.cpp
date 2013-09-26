#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>

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


int main(int argc, char** argv) {
    std::cout << "Init sandbox" << std::endl;

    int index_count = 1;
    if( argc > 1 ) {
        index_count = atoi(argv[1]);
    }
    run_index(index_count);
    Merger mg ("../output/Z.index");
    mg.run();

	return 0;
}



