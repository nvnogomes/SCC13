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
    Aggregator( std::string dir );
    virtual ~Aggregator();

    virtual std::string getCwd();
    virtual int getAggSize();
    virtual void outputAggregate();
    virtual void outputAggregateToFile();
    virtual void run();
    virtual void setDirectory( std::string dir );
};


#endif // AGGREGATOR_H
