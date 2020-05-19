#include "indextable.h"
#include <future>
#include <map>

#include "iostream"
#include "util.h"

indexTable::indexTable() {}

void filter(std::multimap<std::string, unsigned>* tmpmap, std::string key,
            std::multimap<std::string, tableEntry>* table) {
  // for (auto r = tmpmap->begin(); r != tmpmap->end(); r++){
  //    std::cout << r->first << " " << r->second<< std::endl;
  //  }
  std::cout << "filtering: " << key << std::endl;
  auto range = table->equal_range(key);
  std::multimap<std::string, unsigned> keyMap;
  for (auto itr = range.first; itr != range.second; itr++) {
    keyMap.insert(
        std::pair<std::string, unsigned>(itr->second.path, itr->second.pos));
    // std::cout << itr->second.path << " " << itr->second.pos << std::endl;
  }
  std::multimap<std::string, unsigned>* res =
      new std::multimap<std::string, unsigned>;
  for (auto itr = tmpmap->begin(); itr != tmpmap->end(); itr++) {
    auto r = keyMap.equal_range(itr->first);
    for (auto rr = r.first; rr != r.second; rr++) {
      if (itr->second + 1 == rr->second) {
        res->insert(std::pair<std::string, unsigned>(rr->first, rr->second));
      }
    }
  }

  std::cout << "res size: " << res->size() << std::endl;
  for (auto r = res->begin(); r != res->end(); r++) {
    std::cout << r->first << " " << r->second << std::endl;
  }
  *tmpmap = *res;
  delete res;
  for (auto r = tmpmap->begin(); r != tmpmap->end(); r++) {
    std::cout << r->first << " " << r->second << std::endl;
  }
}

std::vector<std::string>* indexTable::get(std::string key) {
  std::vector<std::string>* res = new std::vector<std::string>;
  unsigned threads = this->table.size();

  if (key.find_first_of(" ") != std::string::npos) {  // it's a phrase

    std::vector<std::string>* keys = util::strToWords(key);

    std::cout << "phrase" << std::endl;
    std::future<std::multimap<std::string, unsigned>*>* futures =
        new std::future<
            std::multimap<std::string, unsigned>*>[this->table.size()];

    for (unsigned i = 0; i < threads; i++) {
      futures[i] = std::async(
          std::launch::async,
          [](std::multimap<std::string, tableEntry>* tbl,
             std::vector<std::string>* keys) {
            std::multimap<std::string, unsigned>* tmpMap =
                new std::multimap<std::string, unsigned>;
            auto range = tbl->equal_range(keys->at(0));
            for (auto it = range.first; it != range.second; it++) {
              tmpMap->insert(std::pair<std::string, unsigned>(it->second.path,
                                                              it->second.pos));
            }
            for (unsigned i = 1; i < keys->size(); i++) {
              filter(tmpMap, keys->at(i), tbl);
            }
            return tmpMap;
          },
          this->table.at(i), keys);
    }
    std::multimap<std::string, unsigned>* m;
    for (unsigned i = 0; i < threads; i++) {
      m = futures[i].get();
      for (auto itr = m->begin(); itr != m->end(); itr++) {
        res->push_back(itr->first);
      }
    }

    delete keys;

  } else {  // it's a keyword

    std::cout << "keyword" << std::endl;
    transform(key.begin(), key.end(), key.begin(), ::tolower);
    std::future<std::map<std::string, unsigned>*>* futures =
        new std::future<std::map<std::string, unsigned>*>[this->table.size()];
    for (unsigned i = 0; i < threads; i++) {
      futures[i] = std::async(
          std::launch::async,
          [](std::string key, std::multimap<std::string, tableEntry>* tbl) {
            std::map<std::string, unsigned>* scoreTable =
                new std::map<std::string, unsigned>;
            auto range = tbl->equal_range(key);
            for (auto itr = range.first; itr != range.second; itr++) {
              auto element = scoreTable->find(itr->second.path);
              if (element == scoreTable->end()) {
                scoreTable->insert(
                    std::pair<std::string, unsigned>(itr->second.path, 1));
              } else {
                element->second++;
              }
            }
            return scoreTable;
          },
          key, this->table.at(i));
    }
    std::vector<std::pair<std::string, unsigned>> pairs;

    std::map<std::string, unsigned>* tmpScoreTable;
    for (unsigned i = 0; i < threads; i++) {
      tmpScoreTable = futures[i].get();
      for (auto itr = tmpScoreTable->begin(); itr != tmpScoreTable->end();
           itr++) {
        pairs.push_back(*itr);
        std::cout << itr->first << std::endl;
      }
    }
    sort(pairs.begin(), pairs.end(),
         [=](std::pair<std::string, unsigned>& a,
             std::pair<std::string, unsigned>& b) {
           return a.second < b.second;
         });
    for (auto i : pairs) {
      res->emplace(res->begin(), i.first);
    }
  }

  return res;
}
void indexTable::merge(std::multimap<std::string, tableEntry>* tmpTable) {
  m.lock();
  this->table.insert(this->table.begin(), tmpTable);
  m.unlock();
}
