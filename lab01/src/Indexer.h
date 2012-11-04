#ifndef INDEXER_H
#define INDEXER_H

#include <map>


class Indexer {

    std::string filename;
    std::string directory;
    std::string outputString;
    std::map<std::string, int> indexMap;

private:
    virtual void buildMapString();
    virtual std::string buildOutputFilename();
    virtual std::string trimWord(std::string);

public:
    Indexer( std::string filename );
    virtual ~Indexer();

    virtual std::string getFile();
    virtual int getOutputSize();
    virtual void outputAggregate();
    virtual void outputAggregateToFile();
    virtual void run();
    virtual void setOutputDirectory(std::string);
    virtual void setFile( std::string file );


};


#endif // INDEXER_H
