#include "filequeue.h"

fileQueue::fileQueue()
{

}
  std::string *fileQueue::get() {
    mtx.lock();
    if (q.empty()) {
      mtx.unlock();
      return nullptr;
    }
    std::string *res = new std::string;
    *res = q.front();
    q.pop();
    mtx.unlock();
    return res;
  }
  void fileQueue::add(std::string file) {
    mtx.lock();
    q.push(file);
    mtx.unlock();
  }
