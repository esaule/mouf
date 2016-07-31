//This file is part of Cassis.

//Copyright (C) 2012, Erik Saule

//Cassis is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Cassis is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with Cassis.  If not, see <http://www.gnu.org/licenses/>.

#ifndef CAIRO_SELECTOR_H
#define CAIRO_SELECTOR_H_

#include <fstream>

#ifdef ANDROID
#include "asset_ifstream.hpp"
#endif

#include "SpriteCollection_cairo.hpp"
#include "log.hpp"
#include "timestamp.hpp"
//#include "ShmupCairoWrapper.hpp"
#include "cairo_GUI/cairo_text_display.hpp"
#include "StringFinder.hpp"
#include "mouf_GUI_main.hpp"

#ifndef NOEXCEPTION
#include "exception.hpp"
#endif

#include "config.h"

#define COPYRIGHTTEXT "(c) Erik Saule, 2012. " VERSION_NUMBER "."

class CairoMenuSelector: public CairoGraphicController
{
  //  typedef const char * tagtype;
  typedef std::string tagtype;

  typedef enum
    {
      NONE,
      GAME,
      HELP
    } savetypes;

  cairo_pattern_t * bgcolor;
  cairo_pattern_t * fgcolor;
  cairo_pattern_t * optionbgcolor;

  const char* helptext;

  CairoGraphicController* current;

  int nboption;
  float interoption_height; //fraction of sizeY
  float optionheight; //fraction of sizeY
  float optionwidth; //fraction of sizeX
  float optionOffsetX;
  float optionOffsetY;
  float optionFontSize;
  float optionFontSizeRel;
  float titleFontSizeRel;
  float titleFontSize;
  tagtype* tags;

  savetypes currentobj;


  int highscore;
  bool currentisgame;

  bool checkcurrent()
  {
    if (! current) return false;
    //assert (current);
    if (current->quit())
      {
	if (currentisgame) {
	  //ShmupCairoWrapper* e = (ShmupCairoWrapper*) current;
	  //highscore = std::max(highscore, e->getScore());
	  saveHighScore();
	  reset_highscore_line();
	  currentisgame = false;
	}
	delete current;
	current = NULL;
	currentobj = NONE;
      }

    return (current != NULL);
  }

  std::string highscore_filename() const {return configurationroot+"/highscore";}

  void restoreHighScore() {
    std::ifstream in (highscore_filename().c_str());
    if (in.is_open()) {
      int hs;
      in >> hs;
      if (in) {
	highscore = hs;
      }
    }
    reset_highscore_line();
  }

  void reset_highscore_line()
  {
    std::stringstream ss;
    ss<<StringFinder::singleton->find("main_menu_highscore")<<highscore;
    highscore_line.setText(ss.str());
  }
  
  void saveHighScore() {
    std::ofstream out (highscore_filename().c_str());
    if (out.is_open()) {
      out << highscore;
    }    
  }

  cairo_text_line copyright;
  cairo_text_line title;
  cairo_text_line highscore_line;
public:
  CairoMenuSelector()
    :current(NULL), copyright(this), title(this), highscore_line(this)
  {
    currentisgame = false;
    highscore = 0;
    restoreHighScore();
    nboption = 2;
    interoption_height = .02;
    optionheight = .08;
    optionwidth = .7;

    currentobj = NONE;

    optionOffsetY = .3;
    optionOffsetX = .1;
    optionFontSizeRel = .05;
    
    optionFontSize = optionFontSizeRel*getSizeY();

    title.setOffsetX(.1);
    title.setOffsetY(.18);
    title.setFontSize(.06);
    title.setText(StringFinder::singleton->find("main_menu_title"));

    copyright.setOffsetX(.05);
    copyright.setOffsetY(.9);
    copyright.setFontSize(.015);
    copyright.setText(COPYRIGHTTEXT);

    highscore_line.setOffsetX (.35);
    highscore_line.setOffsetY (.85);
    highscore_line.setFontSize (.035);

    bgcolor = cairo_pattern_create_rgb(1,1,1);
    fgcolor = cairo_pattern_create_rgb(0,0,0);
    optionbgcolor = cairo_pattern_create_rgb(0,1,0);

    tags = new tagtype[nboption];

    
    tags[0] = StringFinder::singleton->find("main_menu_play");
    tags[1] = StringFinder::singleton->find("main_menu_credits");
    // tags[2] = "Medium";
    // tags[3] = "Expert";
    // tags[4] = "Help";
  }

  virtual void setSizeX(int sx)
  {
    CairoGraphicController::setSizeX(sx);
    if (checkcurrent()) {current->setSizeX(sx);}
  }

  virtual void setSizeY(int sy)
  {
    CairoGraphicController::setSizeY(sy);
    if (checkcurrent()) {current->setSizeY(sy);}
    optionFontSize = optionFontSizeRel*getSizeY();
  }

  void clickon(int opt)
  {
    if (opt == 0)
      {
	currentobj = GAME;
	current = new MoufGUI();
      }
    
    if (opt == 1)
      {
	currentobj = HELP;
	current = new CairoTextDisplay(StringFinder::singleton->find("credits_text"));
	((CairoTextDisplay*)current)->setFontSize(.03);
      }

  }

  virtual void clickmove(int x, int y)
  {
    if (checkcurrent()) {current -> clickmove (x,y); return;}
  }


  virtual void clickrelease(int x, int y)
  {
    if (checkcurrent()) {current -> clickrelease (x,y); return;}
  }

  virtual void clickat(int x, int y)
  {
    if (checkcurrent()) {current -> clickat (x,y); return;}

    for (int i=0; i<nboption; ++i)
      {
	int xlow = getSizeX()*optionOffsetX;
	int xhigh = getSizeX()*(optionwidth+optionOffsetX);
	int ylow = getSizeY()*(i*(optionheight+interoption_height)+optionOffsetY);
	int yhigh = getSizeY()*((i+1)*optionheight+i*interoption_height+optionOffsetY);

	if (x >= xlow && x <= xhigh
	    && y >= ylow && y <= yhigh) 
	  clickon(i);
      }    
  }

  virtual void render(cairo_t* cr)
  {
    const bool debug = false;
    if (debug)
      Log::log<<"render "<<util::timestamp()<<Log::endl;
    if (checkcurrent()) {current -> render (cr); return;}

    //    cairo_surface_t* bgsprite = SpriteCollection::sc.getSprite("wood3.png");
    cairo_surface_t* bgsprite = NULL;
    if (bgsprite != NULL)
      {
	blit(cr,bgsprite,0,0, getSizeX(), getSizeY());
      }
    else{     
      cairo_set_source(cr, bgcolor);
      cairo_paint(cr);
    }

    if (debug)
      Log::log<<"background: done "<<util::timestamp()<<Log::endl;

    cairo_set_source(cr, fgcolor);

    title.render(cr);
    
    cairo_save(cr);

    for (int i=0; i<nboption; ++i)
      {
	cairo_rectangle (cr,
			 getSizeX()*optionOffsetX, getSizeY()*(optionOffsetY+i*(optionheight+interoption_height)),
			 getSizeX()*optionwidth, getSizeY()*optionheight);

	//inside
	cairo_set_source (cr, optionbgcolor);
	cairo_fill_preserve(cr);

	//contour
	cairo_set_source (cr, fgcolor);
	cairo_stroke(cr);

	//print tag
	cairo_move_to(cr,
		      getSizeX()*(optionOffsetX+optionwidth/4.),
		      getSizeY()*(optionOffsetY+i*(optionheight+interoption_height)
				  +(optionheight+optionFontSizeRel)/2.));
	show_text(cr, tags[i], optionFontSize);
      }

    cairo_restore(cr);

    if (debug)
      Log::log<<"options: done "<<util::timestamp()<<Log::endl;


    copyright.render(cr);

    highscore_line.render(cr);


    if (debug)
      Log::log<<"end render "<<util::timestamp()<<Log::endl;
  }

  virtual ~CairoMenuSelector()
  {
    if (current) delete current;
    current = NULL;
    
    if (tags) delete[] tags;
    tags = NULL;

    cairo_pattern_destroy(bgcolor);
    cairo_pattern_destroy(fgcolor);
    cairo_pattern_destroy(optionbgcolor);
  }

  virtual bool quit() const {return false;}

  virtual void deserialize(const char* c)
  {
    currentobj = *((savetypes*) c);
    c += sizeof(currentobj);
    highscore = *((int*) c);
    c += sizeof(highscore);
    reset_highscore_line();

    switch (currentobj)
      {
      case NONE:
	break;
      default:
#ifndef NOEXCEPTION
	throw DeserializeException();
#endif
	currentobj = NONE;
	break;
      }
  }

  virtual void serialize(char* c) const
  {
    *((savetypes*) c) = currentobj;
    c += sizeof(currentobj);
    *((int*) c) = highscore;
    c += sizeof(highscore);
    if (current != NULL)
      current->serialize(c);
  }
  
  virtual size_t serializesize() const
  {
    size_t size = sizeof(currentobj);
    size += sizeof(highscore);
    if (current != NULL)
      size += current->serializesize();

    return size;
  }

};

#endif
