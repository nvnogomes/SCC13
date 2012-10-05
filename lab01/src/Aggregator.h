#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include <list>
#include <map>


class Aggregator {

    std::string directory;
    std::string outputString;
    std::map<std::string, std::list<std::pair<std::string,int> > > globalIndex;

private:
    std::list<std::string> listDirectory();
    void buildMapString();

public:
    Aggregator();
    virtual ~Aggregator();

    virtual std::string getCwd();
    virtual void outputAggregate();
    virtual void outputAggregateToFile( std::string file );
    virtual void run();
    virtual void setDirectory( std::string dir );
};


#endif // AGGREGATOR_H
