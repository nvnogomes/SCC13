#include <iostream>
#include <string>
#include <sstream>

#include "Indexer.h"
#include "Aggregator.h"



int main() {
    std::cout << "Init sandbox" << std::endl;
    Indexer i("../data/Eca-Queiros/pg16384.txt", "../output/");
    i.run();
    i.save();
    std::cout << i.histogram_size() << std::endl;

	return 0;
}
