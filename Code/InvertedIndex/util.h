#ifndef UTIL_H
#define UTIL_H
#include <vector>
#include <string>

class util
{
public:
  util();
  static std::vector<std::string> *strToWords(std::string string);
};

#endif // UTIL_H
