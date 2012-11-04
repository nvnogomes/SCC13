#include <fstream>
#include <iostream>
#include <sstream>
#include <cctype>
#include <dirent.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <locale>

#include "Aggregator.h"


Aggregator::Aggregator(std::string dir):
    outputString(""),
    globalIndex()
{
    setDirectory( dir );
}

Aggregator::~Aggregator(){}


int Aggregator::getAggSize() {
    return globalIndex.size();
}

std::string Aggregator::getCwd() {
    return directory;
}

void Aggregator::setDirectory( std::string dir ) {
    directory = dir;
}


std::list<std::string> Aggregator::listDirectory() {
    DIR *searchDirectory;
    struct dirent *entry;
    std::list<std::string> list;

    searchDirectory = opendir( directory.c_str() );

    if( searchDirectory == NULL ) {
	printf("Error opening directory: Agg");
        exit(2);
    }

    while( (entry = readdir( searchDirectory )) ){

        if( entry->d_type == DT_REG ){
            std::string fname = entry->d_name;
            if( fname.find( ".index" ) != std::string::npos) {
                list.push_back( directory +""+ fname );
            }
        }
    }
    closedir( searchDirectory );


    return list;
}


void Aggregator::run() {
    std::list<std::string> fileList = listDirectory();

    std::list<std::string>::iterator fileIter;
    for ( fileIter = fileList.begin(); fileIter != fileList.end(); fileIter++) {
        std::string currentFilename = *fileIter;

        currentFilename.resize( currentFilename.length() - 6);

        std::ifstream input( (*fileIter).c_str(), std::ios::in );
        int entries;
        std::string word;
        int wordCount;

        input >> entries;
        for( int j = 0; j < entries; j++ ) {
            input >> word >> wordCount;
            globalIndex[word].push_back( std::make_pair(currentFilename, wordCount) );
        }
    }
}


void Aggregator::outputAggregate() {

    if( outputString.empty() ) {
        buildMapString();
    }

    std::cout << outputString << std::endl;
}

void Aggregator::outputAggregateToFile() {

    if( outputString.empty() ) {
        buildMapString();
    }

    std::string outputFile = directory +"masterIndex.txt";

    std::ofstream fileOut( outputFile.c_str(), std::ios::out );
    fileOut << outputString;
    fileOut.close();
}


void Aggregator::buildMapString() {

    std::ostringstream buf ("");

    buf << globalIndex.size() << std::endl;
    std::map<std::string, std::list<std::pair<std::string,int> > >::iterator iter;
    for (iter = globalIndex.begin(); iter != globalIndex.end(); ++iter) {
        buf << iter->first << " [";

        std::list<std::pair<std::string,int> >::iterator listIter;
        for( listIter = iter->second.begin(); listIter != iter->second.end(); ++listIter) {
            buf << " (" << listIter->first << " " << listIter->second << ")";
        }
        buf << " ]" << std::endl;
    }

    outputString = buf.str();
}

