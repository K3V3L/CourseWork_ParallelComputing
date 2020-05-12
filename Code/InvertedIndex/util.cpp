#include "util.h"

#include <algorithm>
#include <cstring>

util::util() {}

std::vector<std::string>* util::strToWords(std::string string) {
  std::vector<std::string>* res = new std::vector<std::string>;
  int n = string.length();
  char char_array[n + 1];
  char* token;
  strcpy(char_array, string.c_str());
  char* rest = char_array;
  // std::cout << std::endl << char_array << std::endl;
  std::string sToken;
  while ((token = strtok_r(rest, " #_$[]():;!?,.\"", &rest))) {
    sToken = std::string(token);
    transform(sToken.begin(), sToken.end(), sToken.begin(), ::tolower);
    res->push_back(sToken);
  }
  return res;
}
