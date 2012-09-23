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
void printMap(std::map<std::string, int>& index) {

	std::ofstream fileout("index.txt");

	std::map<std::string, int>::iterator iter;
	for (iter = index.begin(); iter != index.end(); ++iter) {
		fileout << iter->first << " : " << iter->second << std::endl;
	}
	fileout.close();
}


/*
 * removes non alphanumeric characters
 * from the begining and end of the word
 */
std::string cleanWord(std::string& word ) {

	int chIndexBegin = 0;
	while( !isalnum( word[chIndexBegin] ) && chIndexBegin < (int)word.size() ){
		chIndexBegin++;
	}

	int chIndexEnd = word.size() - 1;
	while( !isalnum( word[chIndexEnd] ) && chIndexEnd > 0 ){
		chIndexEnd--;
	}

	int strLenght = chIndexEnd - chIndexBegin +1;

	return word.substr(chIndexBegin, strLenght );
}


int main(int argc, char* argv[]) {

	/* check correct usage */
//	if( argc != 2 ) {
//		std::cerr << "Usage: indexer file | directory" << std::endl;
//		return 1;
//	}

	std::cout << "Locale detected: " << std::locale("").name().c_str() << std::endl;

	std::ifstream input( "../data/Eca-Queiros/pg16384.txt" );

	std::map<std::string, int> index;
	if( input.is_open() ) {

		std::string word;

		while( input.good() ) {
			input >> word;
			index[ cleanWord( word ) ]++;
		}
		input.close();
	}
	else {
		std::cerr << "File not open" << std::endl;
	}

	printMap( index );
	std::cout << index.size() << std::endl;

	return 0;
}
