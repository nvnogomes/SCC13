#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include <vector>

struct Files {
  std::string begin;
  std::string end;
};


class Aggregator {

private:
  std::string directory_;

  std::vector<std::string> list_directory();
  void merge_cycle(std::vector<std::string>);

public:
  Aggregator( std::string dir );
  virtual ~Aggregator();

  virtual void run();
};


#endif // AGGREGATOR_H
