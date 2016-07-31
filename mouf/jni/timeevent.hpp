#ifndef TIME_EVENT_H_
#define TIME_EVENT_H_

class RenderingFixedFPS : public TimeEvent
{
  Game* g;
  Display* d;

  float fps;
  internaltime next;
public:
  RenderingFixedFPS (Game* ga, Display* disp, int rythm)
    :g(ga), d(disp), fps(rythm), next(0)
  {
  }
  
  virtual internaltime nextEvent(const internaltime& actualTime)
  {
    return next - actualTime;
  }
  
  virtual void processEvent()
  {
    const int debugfps = 80;
    static int count = 0;
    static int acc = 0;

    internaltime a = TimeQueue::absolute_currentTime();

    g->render(*d);
    next += 10000/fps;

    internaltime b = TimeQueue::absolute_currentTime();
    acc += b-a;
    count ++;
    if (count % debugfps == 0)
      {
	std::stringstream ss;
	ss<<"rendering time : "<<((double)acc)/debugfps/10<<" ms";
	debug_message(ss.str());
	count = 0;
	acc = 0;
      }
  }

  virtual void slowDown()
  {
    fps *= 0.95;
    
    
    if (d)
      d->setFPS(fps);
    bool debug = false;

    if (debug)
      {
	std::stringstream ss;
	ss<<"New fps : "<<fps;
	debug_message(ss.str());
      }
  }

  virtual void speedUp()
  {
    fps *= 1.01;
    
    if (d)
      d->setFPS(fps);

    bool debug = false;

    if (debug)
      {
	std::stringstream ss;
	ss<<"New fps : "<<fps;
	debug_message(ss.str());
      }
  }
};

class SteppingVariableFPS : public TimeEvent
{
  Game* g;

  int fps;
  internaltime next;
public:
  SteppingVariableFPS (Game* ga)
    :g(ga), fps(0), next(0)
  {
  }
  
  virtual internaltime nextEvent(const internaltime& actualTime)
  {
    return next - actualTime;
  }
  
  virtual void processEvent()
  {
    const int debugfps = 400;
    static int count = 0;
    static int acc = 0;
    const bool debug = false;

    internaltime a = TimeQueue::absolute_currentTime();

    g->step();
    fps = g->refreshRate();
    next += 10000/fps;

    internaltime b = TimeQueue::absolute_currentTime();
    acc += b-a;
    count ++;
    if (count % debugfps == 0)
      {
	std::stringstream ss;
	if (debug)
	  ss<<"stepping time : "<<((double)acc)/debugfps/10<<" ms";
	debug_message(ss.str());
	count = 0;
	acc = 0;
      }
  }
};

#endif
