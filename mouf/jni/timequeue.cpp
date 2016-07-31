#include <string>
#include <iostream>
#include "timequeue.hpp"

class Ding : public TimeEvent
{
  std::string msg;
  int fps;
  internaltime next;
public:
  Ding (std::string s, int rythm)
    :msg(s), fps(rythm), next(0)
  {
  }
  
  virtual internaltime nextEvent(const internaltime& actualTime)
  {
    return next - actualTime;
  }
  
  virtual void processEvent()
  {
    Log::log<<msg<<Log::endl;
    next += 10000/fps;
  }

};

int main()
{
  Ding a ("toto", 4);
  Ding b ("tata", 6);

  TimeQueue tq;
  tq.addEvent (&a);
  tq.addEvent (&b);

  while (true)
    {
      tq.processNext();
    }
  return 0;
}
