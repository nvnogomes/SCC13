#include <iostream>
#include <locale>

#include "Aggregator.h"
#include "Indexer.h"



int IndexerMain(int argc, char* argv[]) {

    AggregateMain();
    IndexerMain();

    return 0;
}



int AggregateMain() {

    std::string searchDirectory = ".";
    if( argc == 2 ) {
        searchDirectory = argv[1];
    }

    Aggregator agg ( searchDirectory );
    agg.run();
    agg.outputAggregateToFile( "aggregatedOutput.txt" );

    return 0;
}



int IndexerMain() {


    if( argc != 2 ) {
        std::cerr << "Usage: indexer file" << std::endl;
        return 1;
    }

    std::string inputFilename = argv[1];

    std::cout << "Locale detected: " << std::locale("").name().c_str() << std::endl;

    Indexer idx ( inputFilename );
    idx.run();
    idx.outputAggregateToFile();

    return 0;
}
