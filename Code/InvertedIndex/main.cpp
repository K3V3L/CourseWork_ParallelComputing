#include <iostream>
#include <string>
#include <dirent.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
using namespace std;
class indexTable{
   vector< string, vector<string, vector< unsigned >>> table;
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
void * threadFunc(vector<string> * queue){

}
int main()
{
    vector<string> * queue = new vector<string>;
    unsigned threads = 1;
    string dirname = "/home/koval/CourseWork_ParallelComputing/dataset/";
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (dirname.c_str())) != NULL) {
      /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
          if(ent->d_name[0] == '.')
              continue;
          std::cout << ent->d_name << std::endl;
          queue->insert(queue->begin(), ent->d_name);
      }
      closedir (dir);
    } else {
      /* could not open directory */
      perror ("");
      return EXIT_FAILURE;
    }

    std::ifstream in(dirname + queue->front());
    std::cout << queue->front() << std::endl;
    std::string contents((std::istreambuf_iterator<char>(in)),
        std::istreambuf_iterator<char>());
    std::cout << "str: " << contents;
    vector<std::string> * words = strToWords(contents);
    for( std::string word : *words){
        std::cout << word << std::endl;
    }
    /*ifstream is(queue.front());
    string str;
    char * cstr;
    while(getline(is, str)){
       cstr = new char[str.size() + 1];
       strcpy(cstr, str.c_str());
    }
    cout << "cstr:" <<  cstr;
    */
        return(0);
}
