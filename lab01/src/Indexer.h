#ifndef INDEXER_H
#define INDEXER_H

#include <map>


class Indexer {

    std::string filename;
    std::string directory;
    std::string outputString;
    std::map<std::string, float> histogram;

private:
    virtual void buildMapString();
    virtual std::string buildOutputFilename();
    virtual std::string trimWord(std::string);

public:
    Indexer( std::string filename );
    virtual ~Indexer();

    virtual std::string getFile();
    virtual int getOutputSize();
    virtual void outputHistogram();
    virtual void outputHistogramToFile();
    virtual void run();
    virtual void setOutputDirectory(std::string);
    virtual void setFile( std::string file );


};


#endif // INDEXER_H
