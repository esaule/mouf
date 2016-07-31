


#ifndef UNIXSTRINGFIND_HPP
#define UNIXSTRINGFIND_HPP

#include "StringFinder.hpp"

#include <string>

class UnixStringFinder : public StringFinder {
  //STUB IMPLEM
public:
  virtual std::string find(std::string& id ) ;

  virtual void clear() ;

  virtual std::string find(const char* id ) ;
};

#endif
