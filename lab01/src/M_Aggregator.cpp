#include <fstream>
#include <iostream>
#include <list>
#include <locale>
#include <map>
#include <string>
#include <cctype>
#include <dirent.h>
#include <unistd.h>



std::list<std::string> listIndexFiles( std::string searchDir ){

	DIR *directory = opendir( searchDir.c_str() );
	dirent* entry;
	std::list<std::string> list;

	while( (entry = readdir( directory )) ){

		if( entry->d_type == DT_REG ){
			std::string fname = entry->d_name;
			if( fname.find( ".index" ) != std::string::npos) {
				std::cout << fname << std::endl;
			}
		}
	}

	return list;
}


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

	std::string searchDirectory = ".";
	if( argc == 2 ) {
		searchDirectory = argv[1];
	}

	std::list<std::string> fileList;
	std::map<std::string, std::list<std::pair<std::string,int>>> globalIndex;

	fileList = listIndexFiles( searchDirectory );
	std::list<std::string>::iterator fileIter;

	for ( fileIter = fileList.begin(); fileIter != fileList.end(); fileIter++) {
		std::string currentFilename = *fileIter;
		currentFilename.resize( currentFilename.length() - 6);

		std::ifstream input( *fileIter );
		int entries;
		std::string word;
		int wordCount;

		input >> entries;
		for( int j = 0; j < entries; j++ ) {

			input >> word >> wordCount;
			globalIndex[word].push_back( std::make_pair(currentFilename, wordCount) );
		}

	}

	printMap( globalIndex );
	std::cout << globalIndex.size() << std::endl;

	return 0;
}
