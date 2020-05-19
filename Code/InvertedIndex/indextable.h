#ifndef INDEXTABLE_H
#define INDEXTABLE_H
#include <tableentry.h>

#include <QMutex>
#include <map>
#include <string>
#include <vector>

class indexTable {
 private:
  QMutex m;
  std::vector<std::multimap<std::string, tableEntry>*> table;

 public:
  indexTable();
  std::vector<std::string>* get(std::string key);
  void merge(std::multimap<std::string, tableEntry>* tmpTable);
};

#endif  // INDEXTABLE_H
