#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include <vector>

struct Files {
  std::string base;
  std::string append;
};


class Aggregator {

private:
  std::string directory_;

  std::vector<std::string> list_directory();
  void merge_cycle(std::vector<std::string>);
  void cleanup_directory(std::vector<std::string>);
  void merge(void *file);

public:
  Aggregator();
  Aggregator( std::string dir );
  virtual ~Aggregator();

  virtual void run();
};


#endif // AGGREGATOR_H
