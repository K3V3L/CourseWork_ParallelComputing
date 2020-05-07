#ifndef FILEQUEUE_H
#define FILEQUEUE_H
#include <QMutex>
#include <string>
#include <queue>

class fileQueue
{
private:
  QMutex mtx;
  std::queue<std::string> q;
public:
  fileQueue();
  std::string *get();
  void add(std::string file);
};

#endif // FILEQUEUE_H
