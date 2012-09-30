#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include <locale>
#include <map>
#include <string>
#include <cctype>
#include <dirent.h>
#include <unistd.h>


class Aggregator {

    std::string directory;
    std::string outputString;
    std::map<std::string, std::list<std::pair<std::string,int> > > globalIndex;

private:
    std::list<std::string> listDirectory();
    void buildMapString();

public:
    Aggregator( std::string dir );
    virtual ~Aggregator();


    virtual std::string getCwd();
    virtual void outputAggregate();
    virtual void outputAggregateToFile( std::string file );
    virtual void run();
    virtual void setDirectory( std::string dir );

};


Aggregator::Aggregator(std::string dir):
    outputString(""),
    globalIndex()
{
    directory = dir.empty() ? "." : dir;
}

Aggregator::~Aggregator(){}


std::string Aggregator::getCwd() {
    return directory;
}

void Aggregator::setDirectory( std::string dir ) {
  directory = dir;
}


std::list<std::string> Aggregator::listDirectory() {
    DIR *searchDirectory = opendir( directory.c_str() );
    dirent* entry;
    std::list<std::string> list;

    while( (entry = readdir( searchDirectory )) ){

        if( entry->d_type == DT_REG ){
            std::string fname = entry->d_name;
            if( fname.find( ".index" ) != std::string::npos) {
                list.push_back( fname );
            }
        }
    }

    return list;
}


void Aggregator::run() {
    std::list<std::string> fileList = listDirectory();

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
}


void Aggregator::outputAggregate() {

    if( outputString.empty() ) {
        buildMapString();
    }

    std::cout << outputString << std::endl;
}

void Aggregator::outputAggregateToFile( std::string file ) {

    if( outputString.empty() ) {
        buildMapString();
    }

    std::ofstream fileOut( file );
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




/*
 * MAIN
 */
int main(int argc, char* argv[]) {

	std::string searchDirectory = ".";
	if( argc == 2 ) {
		searchDirectory = argv[1];
	}

    Aggregator agg ( searchDirectory );
    agg.run();
    agg.outputAggregateToFile( "aggregatedOutput.txt" );

	return 0;
}
