#include "indextable.h"
#include "util.h"
#include "iostream"

#include <future>

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
  std::vector<std::string> * keys = util::strToWords(key);
  if (key.find_first_of(" ") != std::string::npos) {  // it's a phrase
    std::cout << "phrase" << std::endl;
    std::future<std::vector<tableEntry>> * futures = new std::future<std::vector<tableEntry>>[keys->size()];

    for( unsigned i = 0; i < keys->size(); i++){
        futures[i] = std::async(std::launch::async, [&](std::string w){

            std::cout << w << std::endl;
            std::vector<tableEntry> res;
            auto range = this->table.equal_range(w);
            for (auto itr = range.first; itr!= range.second; itr++){
          res.push_back(itr->second);
        }
            return res;
          }, keys->at(i));
    }
    std::map<std::string, unsigned> score;
    std::vector<tableEntry> * tmp = new std::vector<tableEntry>;
    for(unsigned i = 0; i < keys->size(); i++){
        *tmp = (futures[i]).get();
        for( auto i: *tmp){
           auto itr = score.find(i.path);
           if( itr == score.end() ){
               score.insert(std::pair<std::string,unsigned>(i.path,1));
             }else{
               itr->second++;
             }
          }
      }
    std::vector<std::pair<std::string, unsigned>> pairs;
    for (auto itr = score.begin(); itr != score.end(); ++itr)
        pairs.push_back(*itr);

    sort(pairs.begin(), pairs.end(), [=](std::pair<std::string, unsigned>& a, std::pair<std::string, unsigned>& b)
    {
        return a.second < b.second;
    }
    );
    for( auto i: pairs){
        std::cout << i.first << " : " << i.second << std::endl;
        res->emplace(res->begin(), i.first);
        //res->emplace_back(i.first);
      }

//            std::async(std::launch::async, [](std::string k){
//            std::cout << k << std::endl;
//      }, keys[i]);
//    for (auto& i : *(util::strToWords(key))) {
//      auto range = table.equal_range(i);
//      for (auto itr = range.first; itr != range.second; itr++) {
//          if (std::find(res->begin(), res->end(), itr->second.path) == res->end())
//                res->push_back(itr->second.path);
//      }
//    }
//      }
    }
  else {  // it's a keyword
    std::cout << "keyword" << std::endl;
    transform(key.begin(), key.end(),key.begin(), ::tolower);
    auto range = table.equal_range(key);
    for (auto itr = range.first; itr != range.second; itr++) {
      res->push_back(itr->second.path);
    }
  }
  delete keys;
  return res;
}
