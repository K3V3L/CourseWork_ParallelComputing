#ifndef INDEXTABLE_H
#define INDEXTABLE_H
#include <QMutex>
#include <string>
#include <map>
#include <vector>
#include <tableentry.h>

class indexTable
{
private:
  QMutex m;
  std::multimap<std::string, tableEntry> table;

public:
  indexTable();
  void insert(std::string word, tableEntry *entry);
  void print();
  std::vector<std::string> *get(std::string key);
};

#endif // INDEXTABLE_H
;
