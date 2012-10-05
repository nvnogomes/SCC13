#include <fstream>
#include <iostream>
#include <sstream>
#include <locale>
#include <string>
#include <cctype>

#include "Indexer.h"



Indexer::Indexer(std::string filename):
    outputString(""),
    indexMap()
{
    setFile( filename );
}

Indexer::~Indexer(){}


void Indexer::buildMapString() {

    std::ostringstream buf ("");

    buf << indexMap.size() << std::endl;
    std::map<std::string, int>::iterator iter;
    for (iter = indexMap.begin(); iter != indexMap.end(); ++iter) {
        buf << iter->first << " " << iter->second << std::endl;
    }

    outputString = buf.str();
}


std::string Indexer::buildOutputFilename() {

    int fileIndex = filename.rfind("/") + 1;
    int strLenght = filename.size() - fileIndex;
    return filename.substr( fileIndex, strLenght ) + ".index";
}


std::string Indexer::getFile() {
    return filename;
}


void Indexer::outputAggregate() {

    if( outputString.empty() ) {
        buildMapString();
    }

    std::cout << outputString << std::endl;
}


void Indexer::outputAggregateToFile() {

    if( outputString.empty() ) {
        buildMapString();
    }

    std::string outputFile = buildOutputFilename();
    std::ofstream fileOut( outputFile );
    fileOut << outputString;

    fileOut.close();
}


void Indexer::run() {

    std::ifstream input( filename );

    if( input.is_open() ) {

        std::string word;

        while( input.good() ) {
            input >> word;
            word = trimWord( word );
            if( word.length() > 1 ) {
                indexMap[ word ]++;
            }
        }
        input.close();
    }
    else {
        std::cerr << "Error: Could not open file." << std::endl;
    }
}


void Indexer::setFile(std::string file) {
    filename = file;
}


/*
 * removes non alphanumeric characters
 * from the begining and end of the word */
std::string Indexer::trimWord( std::string word) {
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

