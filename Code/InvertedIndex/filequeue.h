#ifndef FILEQUEUE_H
#define FILEQUEUE_H
#include <QMutex>
#include <queue>
#include <string>

class fileQueue {
 private:
  QMutex mtx;
  std::queue<std::string> q;

 public:
  fileQueue();
  std::string* get();
  void add(std::string file);
  unsigned getSize();
};

#endif  // FILEQUEUE_H
