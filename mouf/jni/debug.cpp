#include "debug.h"
#include "graphics.hpp"

class debug_stream 
{
  Display *disp;

 public:
  debug_stream()
    :disp(NULL)
    {}
  void setDisplay(Display* d)
    {
      disp = d;
    }
  void message(const std::string& s);
};

debug_stream debugs;

void debug_setDisplay (Display* d)
{
  debugs.setDisplay(d);
}

#ifndef ANDROID
void debug_message (const std::string& s)
{
  debugs.message(s);
}
#endif

#ifdef ANDROID
#include "log.hpp"
void debug_message (const std::string& s)
{
  Log::log<<s<<Log::endl;
}
#endif

void debug_stream::message( const std::string& s)
{
  if (disp)
    disp->printMessage(s);
  else
    Log::logerr<<s<<Log::endl;
}

