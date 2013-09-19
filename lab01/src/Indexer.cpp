#include <algorithm>
#include <fstream>
#include <iostream>
#include <istream>
#include <sstream>
#include <locale>
#include <string>
#include <cctype>

#include "Indexer.h"


Indexer::Indexer(std::string filename):
    outputString(""),
    histogram()
{
    setFile( filename );
    setOutputDirectory( "../output/" );
}

Indexer::~Indexer(){}


void Indexer::buildMapString() {

    std::ostringstream buf ("");

    buf << this->histogram.size() << std::endl;
    std::map<std::string, float>::iterator iter;
    for (iter = this->histogram.begin(); iter != this->histogram.end(); ++iter) {
        buf << iter->first << " " << iter->second << std::endl;
    }

    this->outputString = buf.str();
}


std::string Indexer::buildOutputFilename() {

    int fileIndex = this->filename.rfind("/") + 1;
    int strLenght = this->filename.size() - fileIndex;
    return this->directory +""+ this->filename.substr( fileIndex, strLenght ) +
".index";
}


std::string Indexer::getFile() {
    return this->filename;
}

int Indexer::getOutputSize() {
    return this->histogram.size();
}


void Indexer::outputHistogram() {

    if( this->outputString.empty() ) {
        buildMapString();
    }

    std::cout << this->outputString << std::endl;
}


void Indexer::outputHistogramToFile() {

    if( this->outputString.empty() ) {
        buildMapString();
    }

    std::string outputFile = buildOutputFilename();
    std::ofstream fileOut;
    fileOut.open( outputFile.c_str(), std::ios::out );
    fileOut << this->outputString;

    fileOut.close();
}


void Indexer::run() {

    std::ifstream input( filename.c_str(), std::ios::in );

    if( input.is_open() ) {

        std::string word;

        while( input.good() ) {
            input >> word;
            word = trimWord( word );
            if( word.length() > 1 ) {
                this->histogram[ word ]++;
            }
        }
        input.close();

        // histogram
        std::map<std::string, float>::iterator it;
        for(it = this->histogram.begin(); it != this->histogram.end(); it++) {
           this->histogram[it->first] = (it->second / this->histogram.size()) * 1000;
        }

    }
    else {
        std::cerr << "Error: Could not open file." << std::endl;
    }
}


void Indexer::setFile(std::string file) {
    this->filename = file;
}


void Indexer::setOutputDirectory( std::string dir ) {
    this->directory = dir;
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

