#ifndef MERGER_H
#define MERGER_H

#include <map>
#include <string>
#include <list>

class Merger
{
private:

    std::string filename_;
    bool is_directory_;
    std::map<std::string, std::list<std::string> > letter_map_;
    std::list<std::string> list_directory();
    void merge_file(std::string);
    static bool is_file(const char* path);
    virtual void save(std::string);

public:
    Merger(std::string);
    virtual ~Merger();

    virtual void run();

};

#endif // MERGER_H
