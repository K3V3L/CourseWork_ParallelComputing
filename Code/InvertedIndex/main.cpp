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
#include <folderdialog.h>
#include <QApplication>
#include <filequeue.h>
#include <indextable.h>
#include <util.h>

using namespace std;




void threadFunc(fileQueue *fq, indexTable *table) {
  string *filename = nullptr;
  unsigned pos;
  while ((filename = fq->get())) {
    pos = 0;
    std::ifstream in(*filename);
    std::cout << *filename << std::endl;
    std::string contents((std::istreambuf_iterator<char>(in)),
                         std::istreambuf_iterator<char>());
    vector<std::string> *words = util::strToWords(contents);
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
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  folderDialog dialog;
  dialog.show();
  dialog.exec();
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
