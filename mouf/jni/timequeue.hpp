#ifndef __TIME_QUEUE__
#define __TIME_QUEUE__

#include <list>
#include <time.h>
#include <sys/time.h>
#include <algorithm>
#include "debug.h"

typedef int internaltime; //in 10^-4 seconds

class TimeEvent
{
 public:
  virtual internaltime nextEvent(const internaltime& actualTime)
    {return 0;} //time before next event
  virtual void processEvent(){}
  virtual void slowDown(){}
  virtual void speedUp(){}
};

class TimeQueue
{
  class EarliestEventComparator
    {
      internaltime currentTime;
    public:
      EarliestEventComparator (internaltime ct)
	:currentTime(ct)
	{}
      bool operator() (TimeEvent* a, TimeEvent* b)
	{
	  internaltime ta = a->nextEvent(currentTime);
	  internaltime tb = b->nextEvent(currentTime);
	  
	  return ( ta <= tb);
	}
    };

  std::list<TimeEvent*> events;

  int missed;
  struct timeval starttime;

  //elapsed time since the queue construction
  internaltime currentTime()
    {
      
      struct timeval ct;
      useconds_t dt;
      gettimeofday(&ct, NULL);
      dt = (ct.tv_sec - starttime.tv_sec)*1000000+(ct.tv_usec - starttime.tv_usec); /* 1000000 microseconds in a second... */
      return dt / 100;
    }
  
  //returns the timeevent to ding next
  TimeEvent* extractEarliest()
    {
      std::list<TimeEvent*>::iterator it = std::min_element (events.begin(), events.end(),
							     EarliestEventComparator(currentTime()));
      return *it;
    }

  //wait for a given amount of time
  void waitFor (internaltime del)
    {
      if (del > 0)
	{
	  useconds_t udel = del*100;

	  if (missed > 0)
	    missed = 0;
	  
	  if (udel <= 2000)
	    missed = 0;
	  else
	    missed --;

	  usleep (udel);
	}
      else
	{
	  if (missed<0)
	    missed=0;
	  missed ++;
	  if (missed % 100 == 0)
	    {
	      std::stringstream ss;
	      ss<<missed;
	      debug_message(ss.str());
	    }
	  if (missed >= 1000)
	    {
	      debug_message("too slow");
	    }
	}
    }

 public:
  TimeQueue()
    :missed(0)
    {
      resetStartTime();
    }

  void resetStartTime(){gettimeofday(&starttime, NULL);}
  void addEvent (TimeEvent* te)
    {
      events.push_front(te);

    }

  //blocking function that will wait untill exactly one event is processed
  void processNext ()
    {
      TimeEvent* te = extractEarliest();
      if (!te) return;
      waitFor (te->nextEvent(currentTime()));
      if (missed > 10)
	te->slowDown();
      else if (missed < 10)
	te->speedUp();
      te->processEvent();
    }

  void processBacklog()
  {
    TimeEvent* te = extractEarliest();
    int count = 0;
    if (te)
      {
	auto ct = currentTime();
	bool cont = true;
	while (cont) {
	  cont = false;
	  while (te->nextEvent(ct) <=0 ) {
	    cont = true;
	    te->processEvent();    
	    te = extractEarliest();
	    ++count;
	  }
	}
	//	Log::log<<count<<" events processed"<<Log::endl;
      }
  }

  static internaltime absolute_currentTime()
    {
      struct timeval ct;
      useconds_t dt;
      gettimeofday(&ct, NULL);
      dt = (ct.tv_sec)*1000000+(ct.tv_usec); /* 1000000 microseconds in a second... */
      return dt /100;
    }

};

#endif
