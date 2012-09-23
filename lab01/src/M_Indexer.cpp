#include <fstream>
#include <iostream>
#include <locale>
#include <map>
#include <string>
#include <cctype>

/*
 * prints the map to file
 */
void printMap(std::map<std::string, int>& index, std::string inputFilename) {

	int fileIndex = inputFilename.rfind("/") + 1;
	int strLenght = inputFilename.size() - fileIndex;
	std::string outputFilename = "index_" + inputFilename.substr( fileIndex, strLenght );
	std::cout << outputFilename << std::endl;

	std::ofstream fileout( outputFilename );

	fileout << index.size() << std::endl;
	std::map<std::string, int>::iterator iter;
	for (iter = index.begin(); iter != index.end(); ++iter) {
		fileout << iter->first << " " << iter->second << std::endl;
	}
	fileout.close();
}


/*
 * removes non alphanumeric characters
 * from the begining and end of the word
 */
std::string cleanWord(std::string& word ) {

	int chIndexBegin = 0;
	while( !isalpha( word[chIndexBegin] ) && chIndexBegin < (int)word.size() ){
		chIndexBegin++;
	}

	int chIndexEnd = word.size() - 1;
	while( !isalpha( word[chIndexEnd] ) && chIndexEnd > 0 ){
		chIndexEnd--;
	}

	int strLenght = chIndexEnd - chIndexBegin +1;

	if( strLenght <= 0 ) {
		return "";
	}
	else {
		return word.substr(chIndexBegin, strLenght );
	}
}

/*
 * MAIN
 */
int main(int argc, char* argv[]) {

	/* check correct usage */
//	if( argc != 2 ) {
//		std::cerr << "Usage: indexer file | directory" << std::endl;
//		return 1;
//	}

	std::cout << "Locale detected: " << std::locale("").name().c_str() << std::endl;

//	std::string inputFilename = "../data/Eca-Queiros/pg16384.txt";
	std::string inputFilename = "../data/Eca-Queiros/pg17515.txt";
	std::ifstream input( inputFilename );

	std::map<std::string, int> index;
	if( input.is_open() ) {

		std::string word;

		while( input.good() ) {
			input >> word;
			word = cleanWord( word );
			if( word.length() > 1 ) {
				index[ cleanWord( word ) ]++;
			}
		}
		input.close();

		printMap( index, inputFilename );
		std::cout << index.size() << std::endl;
	}
	else {
		std::cerr << "File not open" << std::endl;
	}

	return 0;
}
