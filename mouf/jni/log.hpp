#ifndef LOG_H_
#define LOG_H_

#ifdef NOSTL
#include "mysstream.hpp"
#else
#include <sstream>
#include <string>
#endif

#ifndef ANDROID
#include <iostream>
#else

#include <android/log.h>

#define  LOG_TAG    "shmup"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#endif

class ostream
{
public:
#ifndef NOSTL
  typedef std::string string;
#else
  typedef MyString string;
#endif  

  virtual void append(char) = 0;
  virtual void append(int) = 0;
  virtual void append(unsigned int) = 0;
  virtual void append(double) = 0;
  virtual void append(const char*) = 0;
  virtual void append(const void*) = 0;
  virtual void append(const string&) = 0;

  template<class T >
  ostream& operator<<(const T& t)
  {
    this->append(t);
    return *this;
  }
};


class Log: public ostream
{
#ifndef NOSTL
  std::stringstream ss;
#else
  stringstream ss;
#endif
public:
  struct c {int a;};
  static c endl; //end of log tag
  
  static Log log;
  static Log logerr;

  virtual void append(char c)
  {
    char buf[2];
    buf[0] = c;
    buf[1] = '\0';

    ss<<buf;
  }

  virtual void append(int i)
  {
    ss<<i;
  }

  virtual void append(unsigned int i)
  {
    ss<<i;
  }

  virtual void append(double d)
  {
    ss<<d;
  }

  virtual void append(const char* st)
  {
    ss<<st;
  }  

  virtual void append(const void* st)
  {
    ss<<st;
  }  

  virtual void append(const string& s)
  {
    ss<<s;
  }

#ifndef ANDROID
  void commit()
  {
    //    Log::log<<ss.str()<<Log::endl;
    std::cout<<ss.str()<<std::endl;
    ss.str("");
  }
#else
  void commit()
  {
    LOGI("%s", ss.str().c_str());
    ss.str("");
  }
#endif
};

static ostream& operator<< (ostream& o, Log::c & c)
{
  Log& l = (Log&) o;
  l.commit();
  return o;
}

#endif
