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

using namespace std;
class indexTable{
   vector< string, vector<string, vector< unsigned >>> table;
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
    while((filename = fq->get())){
        std::ifstream in(*filename);
        std::cout << *filename << std::endl;
        std::string contents((std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>());
        vector<std::string> * words = strToWords(contents);
        for( std::string word : *words){
            std::cout << word << std::endl;
        }
    }
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
