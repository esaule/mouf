#ifndef STRING_FINDER_HPP
#define STRING_FINDER_HPP


#include <stdbool.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <string>

class StringFinder {
public:
  virtual std::string find(std::string& id ) = 0;

  virtual void clear() = 0;

  virtual std::string find(const char* id ) = 0;

  static StringFinder* singleton;
};



#endif
