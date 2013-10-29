#ifndef INDEXER_H
#define INDEXER_H

#include <map>


class Indexer {

private:
    std::string filename_;
    std::string directory_;
    std::string output_string_;
    std::map<char,std::string> letters_;
    std::map<std::string, float> histogram_;

    virtual void build_histogram_string();
    virtual std::string build_output_filename();
    virtual std::string filter(std::string);
    virtual std::string trim_word(std::string);
    static bool start_match(std::string, std::string);

public:
    Indexer( std::string filename );
    Indexer( std::string filename, std::string output_directory);
    virtual ~Indexer();

    virtual std::string get_filename();
    virtual void print();
    virtual void run();
    virtual void save();

};
#endif // INDEXER_H
