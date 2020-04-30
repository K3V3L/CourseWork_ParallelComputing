#include <iostream>
#include <string>
#include <dirent.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <QMutex>
#include <queue>
#include <map>

using namespace std;
class tableEntry{
public:
    string path;
    unsigned pos;
};

class indexTable{
    multimap<string, tableEntry> table;
public:
   void insert( string word, tableEntry *entry){
       table.insert(pair<string, tableEntry>(word, *entry));
   }
   void print(){
       for(multimap<string, tableEntry >::const_iterator it = table.begin(); it != table.end(); ++it){
                  std::cout << it->first << " " << it->second.path << " " << it->second.pos << "\n";
              }
          }
};

class fileQueue{
private:
    QMutex mtx;
    queue<string> q;
public:
    string * get(){
        mtx.lock();
        if(q.empty())
        {
            mtx.unlock();
            return nullptr;
        }
        string * res = new string;
        *res = q.front();
        q.pop();
        mtx.unlock();
        return res;
    }
    void add(string file){
        mtx.lock();
        q.push(file);
        mtx.unlock();
    }
};

vector<std::string> *  strToWords(std::string string){
    vector<std::string> * res = new vector<std::string>;
    int n = string.length();

        char char_array[n + 1];
        char * token;
        strcpy(char_array, string.c_str());
        char * rest = char_array;
        std::cout << std::endl << char_array << std::endl;
        while ( (token = strtok_r(rest, " #_$[]():;!?,.\"", &rest))){
            res->push_back(std::string(token));
        }
    return res;
}
void threadFunc(fileQueue * fq){
    string * filename = nullptr;
    indexTable table;
    unsigned pos;
    while((filename = fq->get())){
        pos = 0;
        std::ifstream in(*filename);
        std::cout << *filename << std::endl;
        std::string contents((std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>());
        vector<std::string> * words = strToWords(contents);
        tableEntry * entry= new tableEntry;
        for( std::string word : *words){
            std::cout << word <<  *filename << std::endl;
            entry->pos = pos;
            entry->path = *filename;
            table.insert(word, entry);
            pos++;
        }
    }
    table.print();
}
int main()
{
    fileQueue * fq = new fileQueue;
    unsigned threads = 1;
    string dirname = "/home/koval/CourseWork_ParallelComputing/dataset/";
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dirname.c_str())) != NULL) {
      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
          if(ent->d_name[0] == '.')
              continue;
//          std::cout << ent->d_name << std::endl;
          fq->add(string(dirname + ent->d_name));
      }
      closedir (dir);
    } else {
      /* could not open directory */
      perror ("");
      return EXIT_FAILURE;
    }
    threadFunc(fq);
        return(0);
}
