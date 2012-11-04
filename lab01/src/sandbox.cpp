#include <iostream>
#include <string>
#include <sstream>

#include "Indexer.h"
#include "Aggregator.h"



int main() {
	printf("Init sandbox");
	Aggregator agg("../output/");
	agg.run();
	std::cout << agg.getAggSize() << std::endl;
	agg.outputAggregateToFile();

	return 0;
}
