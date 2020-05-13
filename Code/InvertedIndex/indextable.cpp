#include "indextable.h"
#include "util.h"
#include "iostream"

indexTable::indexTable() {}

void indexTable::insert(std::string word, tableEntry* entry) {
  m.lock();
  table.insert(std::pair<std::string, tableEntry>(word, *entry));
  m.unlock();
}

void indexTable::print() {
  for (std::multimap<std::string, tableEntry>::const_iterator it =
           table.begin();
       it != table.end(); ++it) {
    std::cout << it->first << " " << it->second.path << " " << it->second.pos
              << "\n";
  }
}

std::vector<std::string>* indexTable::get(std::string key) {
  std::vector<std::string>* res = new std::vector<std::string>;
  if (key.find_first_of(" ") != std::string::npos) {  // it's a phrase
    std::cout << "phrase" << std::endl;
    for (auto& i : *(util::strToWords(key))) {
      auto range = table.equal_range(i);
      for (auto itr = range.first; itr != range.second; itr++) {
          if (std::find(res->begin(), res->end(), itr->second.path) == res->end())
                res->push_back(itr->second.path);
      }
    }
  } else {  // it's a keyword
    std::cout << "keyword" << std::endl;
    auto range = table.equal_range(key);
    for (auto itr = range.first; itr != range.second; itr++) {
      res->push_back(itr->second.path);
    }
  }
  return res;
}
