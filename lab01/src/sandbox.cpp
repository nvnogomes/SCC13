#include <iostream>
#include <string>
#include <sstream>

#include "Indexer.h"
#include "Aggregator.h"



int main() {
    std::cout << "Init sandbox" << std::endl;
    Indexer i("../data/Eca-Queiros/pg16384.txt");
    i.run();
    i.outputHistogram();
    i.outputHistogramToFile();
    std::cout << i.getOutputSize() << std::endl;

	return 0;
}
