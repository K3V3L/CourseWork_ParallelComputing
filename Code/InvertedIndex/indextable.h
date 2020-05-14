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
  std::multimap<std::string, tableEntry> table;

 public:
  indexTable();
  void insert(std::string word, tableEntry* entry);
  void print();
  std::vector<std::string>* get(std::string key);
  void merge(std::multimap<std::string, tableEntry>* tmpTable);
};

#endif  // INDEXTABLE_H
