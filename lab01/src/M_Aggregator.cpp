#include <fstream>
#include <iostream>
#include <list>
#include <locale>
#include <map>
#include <string>
#include <cctype>


/*
 * prints the map to file
 */
void printMap(std::map<std::string, std::list<std::pair<std::string,int>>> agg) {

	std::ofstream fileout( "resultList.txt" );

	fileout << agg.size() << std::endl;
	std::map<std::string, std::list<std::pair<std::string,int>>>::iterator iter;
	for (iter = agg.begin(); iter != agg.end(); ++iter) {
		fileout << iter->first << " [";

		std::list<std::pair<std::string,int>>::iterator listIter;
		for( listIter = iter->second.begin(); listIter != iter->second.end(); ++listIter) {
			fileout << " (" << listIter->first << " " << listIter->second << ")";
		}
		fileout << " ]" << std::endl;
	}
	fileout.close();
}

/*
 * MAIN
 */
int main(int argc, char* argv[]) {

	/* check correct usage */
	if( argc < 2 ) {
		std::cerr << "Usage: indexer file [file ...]" << std::endl;
		return 1;
	}


	std::map<std::string, std::list<std::pair<std::string,int>>> globalIndex;
	for(int i = 0; i < argc; i++) {
		std::string currentFile = (std::string)argv[i];
		currentFile = currentFile.substr(6, currentFile.length() - 5);

		std::ifstream input( argv[i] );
		int entries = 0;
		input >> entries;

		std::string word;
		int counter;
		for( int j = 0; j < entries; j++ ) {

			input >> word >> counter;
			globalIndex[word].push_back( std::make_pair(currentFile, counter) );
		}

	}

	printMap( globalIndex );
	std::cout << globalIndex.size() << std::endl;

	return 0;
}
