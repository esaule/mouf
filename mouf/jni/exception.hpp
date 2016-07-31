#ifndef EXCEPT_HPP
#define EXCEPT_HPP

//#include "util/stacktrace.hpp"
#include "log.hpp"

class BaseException {
  std::vector<std::string> stack;

  std::string type;
public:
  BaseException(const std::string& type = "BaseException")
    :type(type)
  {
    //  stack = get_backtrace();
  }   
  void print_backtrace() {
    for (auto l : stack)
      Log::log<<l<<Log::endl;

  }
  virtual std::string info(){return type;}
  virtual ~BaseException(){}
};

namespace tact {
  namespace engine {
    class InvalidFormat  : public BaseException {
      std::string what;
      std::string where;
    public:
      InvalidFormat()
	:BaseException("InvalidFormat"), what("??"), where("??")
      {}
      InvalidFormat(const std::string& what, const std::string& where)
	:what(what), where(where)
      {}

      virtual std::string info(){
	std::stringstream ss;
	ss<<"InvalidFormat of \""
	  <<what
	  <<"\" found in \""
	  <<where
	  <<"\"";
	return ss.str();
      }
    };
  }
}

class InvalidArgument  : public BaseException {
public:
  InvalidArgument()
    :BaseException("InvalidArgument")
  {}
};

class NoSuchFile  : public BaseException {
  std::string filename;
public:
  NoSuchFile(const std::string& file = "<unknown>")
    :BaseException("NoSuchFile"), filename(file)
  {}
  virtual std::string info(){
    std::stringstream ss;
    ss<<BaseException::info()<<" ("<<filename<<")";
    return ss.str();
  }
};

#endif
