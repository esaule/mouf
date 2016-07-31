#include "log.hpp"
#include "graphics.hpp"
#include <stdlib.h>
#include <iostream>
#include "input.hpp"
#include "Game.hpp"
#include <vector>
#include "timequeue.hpp"
#include "debug.h"
#include "timeevent.hpp"
#include "play.hpp"
#include "DefaultStringFinder.hpp"

class MultipleChoice
{
  std::vector<std::string> choices;

  Display& d;
  Input& i;

public:
  MultipleChoice(Display& d, Input& i)
    :d(d), i(i)
  {
  }

  void addChoice (const std::string& choice)
  {
    choices.push_back(choice);
  }

  std::string getChoice()
  {
    i.reset();
    int selection=0;
    assert (choices.size()>0);

    d.setZoom(1.);

    while (!i.shoot())
      {
	//input management
	i.reset();
	i.handle();
	if (i.up()) selection --;
	if (i.down()) selection ++;


	if (selection < 0) selection = 0;
	if (selection >= choices.size()) selection = choices.size()-1;
	
	//display
	d.white();

	d.drawSprite(Object(0,800,0,600),
		     "background-menu.png",
		     Offset(Position(0,0),Position(800,600)));
	
	int line = 0;
	int linesize = 20;

	int xoff = 400;
	int yoff = 400;
	Color fc (0, 0, 0);
	d.setColor(fc);
	for (auto it = choices.begin(); it != choices.end(); it++)
	  {
	    if (line == selection) //display marker
	      {
		Object o = Object(xoff-25, xoff-10, yoff+(line-1)*linesize, yoff+(line-1)*linesize+15);
		d.drawSprite(o,
			     "ship.png",
			     Offset(Position(0,0),Position(30,30)));
	      }


	    d.printText(xoff, yoff+line*linesize, *it);
	    line++;
	  }

	d.show();
	usleep (10);
      }
    i.reset();

    return choices[selection];
  }
};

std::string chooselevel (Display& d, Input& i)
{
  MultipleChoice mc (d,i);
  mc.addChoice("data/levels/enemySRotate-hard.timed");
  mc.addChoice("data/levels/laby.timed");
  mc.addChoice("quit");

  return mc.getChoice();
}

int main(int argc, char* argv[])
{
  if ((argc == 2 && strcmp(argv[1],"-h") == 0) || argc > 2)
    {
      Log::logerr<<"To only play a level:"<<Log::endl;
      Log::logerr<<"usage: "<<argv[0]<<" filename"<<Log::endl;
      Log::logerr<<"To choose levels graphically:"<<Log::endl;
      Log::logerr<<"usage: "<<argv[0]<<Log::endl;
      Log::logerr<<"To display help:"<<Log::endl;
      Log::logerr<<"usage: "<<argv[0]<<" -h"<<Log::endl;
      
      return -1;
    }

  StringFinder::singleton = new DefaultStringFinder();

  if (argc == 1)
    {
      Display d;
      debug_setDisplay(&d);
      Input i;

      while (1)
	{
	  std::string level = chooselevel(d, i);

	  if (level == "quit")
	    break;
	  
	  play (d,i,level.c_str());
	}

      Log::log<<"Quitting"<<Log::endl;
    }

  if (argc == 2)
    {
      Display d;
      debug_setDisplay(&d);
      Input i;
      
      play (d,i,argv[1]);

      Log::log<<"Quitting"<<Log::endl;
    }

  return 0;
}
