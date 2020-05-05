#include <iostream>
#include <QMutex>
#include <dirent.h>
#include <fstream>
#include <map>
#include <queue>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;
class tableEntry {
public:
  string path;
  unsigned pos;
};
vector<std::string> *strToWords(std::string string);
class indexTable {
  QMutex m;
  multimap<string, tableEntry> table;

public:
  void insert(string word, tableEntry *entry) {
    m.lock();
    table.insert(pair<string, tableEntry>(word, *entry));
    m.unlock();
  }
  void print() {
    for (multimap<string, tableEntry>::const_iterator it = table.begin();
         it != table.end(); ++it) {
      std::cout << it->first << " " << it->second.path << " " << it->second.pos
                << "\n";
    }
  };
  vector<string> *get(string key) {
    vector<string> *res = new vector<string>;
    if (key.find_first_of(" ") != string::npos) { // it's a phrase
      cout << "phrase" << endl;
      for (auto &i : *(strToWords(key))) {
          auto range = table.equal_range(i);
          for ( auto itr = range.first; itr != range.second; itr++){
              res->push_back(itr->second.path);
          }
      }
    } else { // it's a keyword
      cout << "keyword" << endl;
      auto range = table.equal_range(key);
      for (auto itr = range.first; itr != range.second; itr++) {
        res->push_back(itr->second.path);
      }
    }
    return res;
  }
};
class fileQueue {
private:
  QMutex mtx;
  queue<string> q;

public:
  string *get() {
    mtx.lock();
    if (q.empty()) {
      mtx.unlock();
      return nullptr;
    }
    string *res = new string;
    *res = q.front();
    q.pop();
    mtx.unlock();
    return res;
  }
  void add(string file) {
    mtx.lock();
    q.push(file);
    mtx.unlock();
  }
};

vector<std::string> *strToWords(std::string string) {
  vector<std::string> *res = new vector<std::string>;
  int n = string.length();
  char char_array[n + 1];
  char *token;
  strcpy(char_array, string.c_str());
  char *rest = char_array;
  // std::cout << std::endl << char_array << std::endl;
  while ((token = strtok_r(rest, " #_$[]():;!?,.\"", &rest))) {
    res->push_back(std::string(token));
  }
  return res;
}
void threadFunc(fileQueue *fq, indexTable *table) {
  string *filename = nullptr;
  unsigned pos;
  while ((filename = fq->get())) {
    pos = 0;
    std::ifstream in(*filename);
    std::cout << *filename << std::endl;
    std::string contents((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    vector<std::string> *words = strToWords(contents);
    tableEntry *entry = new tableEntry;
    for (std::string word : *words) {
      // std::cout << word <<  *filename << std::endl;
      entry->pos = pos;
      entry->path = *filename;
      table->insert(word, entry);
      pos++;
    }
  }
}
int main() {
  fileQueue *fq = new fileQueue;
  unsigned threads = 1;
  string dirname = "/home/koval/CourseWork_ParallelComputing/dataset/";
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(dirname.c_str())) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir(dir)) != NULL) {
      if (ent->d_name[0] == '.')
        continue;
      //          std::cout << ent->d_name << std::endl;
      fq->add(string(dirname + ent->d_name));
    }
    closedir(dir);
  } else {
    /* could not open directory */
    perror("");
    return EXIT_FAILURE;
  }
  indexTable *table = new indexTable;
  threadFunc(fq, table);
  //    table->print();
  vector<string> *results = table->get("key error");

  cout << "Found " << results->size() << " matches" << endl;
  for (auto &i : *results)
    std::cout << i << endl;
  return (0);
}
