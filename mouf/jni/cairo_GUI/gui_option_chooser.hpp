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

#ifndef OPTIONCHOOSER_CAIRO
#define OPTIONCHOOSER_CAIRO

#include "cairo_graphic_controller.hpp"
#include "SpriteCollection_cairo.hpp"
#include "log.hpp"
#include "gui_interface.hpp"
#include "exception.hpp"

class NoSuchOptionException : public BaseException {
public:
  typedef std::string string;
  string opt;
  NoSuchOptionException(const string& str)
    :BaseException("NoSuchOptionException"), opt(str)
  {}
};

class GuiOptionChooser : public CairoGraphicController
{
private:
  VerbListener& vl;

  cairo_pattern_t* bgColor;
  cairo_pattern_t* fontColor;
  cairo_pattern_t* borderColor;
  cairo_pattern_t* highlightColor;


  float optionOffsetX;
  float optionOffsetY;

  float interoptionHeight; //fraction of sizeY
  float interoptionWidth; //fraction of sizeY
  float optionHeight; //fraction of sizeY
  float optionWidth; //fraction of sizeX
  float optionWidthInternal; //fraction of sizeX

  float optionFontSizeRel;
  float optionFontSize;

  struct properties {
    bool highlight;
  };

  std::vector< std::pair<VerbListener::Verb, properties> > options;

  bool trigger;

public:

  ///All messages are sent to v
  GuiOptionChooser(VerbListener& v)
    :vl(v)
  {
    bgColor = cairo_pattern_create_rgb(0., 0., 1.);
    fontColor = cairo_pattern_create_rgb(0., 0., 0.);
    borderColor = cairo_pattern_create_rgb(0., 0., 0.);
    highlightColor = cairo_pattern_create_rgba(0., 0., 0., 0.3);

    trigger = false;

    optionOffsetX = .1;
    optionOffsetY = .1;
    interoptionHeight = .05;
    interoptionWidth = .05;
    optionHeight = .1;
    optionWidth = .3;
    optionWidthInternal = .1;

    optionFontSizeRel = .05;
    optionFontSize = optionFontSizeRel*getSizeY();
  }

  ///@brief Position of the first box
  void setOffsetX (float ox) {optionOffsetX = ox;}

  ///@brief Position of the first box
  void setOffsetY (float oy) {optionOffsetY = oy;}

  ///@brief Spacing between start of components
  void setInterHeight (float ih) {interoptionHeight = ih;}

  ///@brief Spacing between start of components
  void setInterWidth (float iw) {interoptionWidth = iw;}

  ///@brief Size of the box
  void setOptionHeight (float oh) {optionHeight = oh;}

  ///@brief Size of the box
  void setOptionWidth (float ow) {optionWidth = ow;}

  ///@brief Space between border and start of writing within the box
  void setOptionWidthInternal (float ow) {optionWidthInternal = ow;}

  ///@brief Relative size of the font 
  void setFontSize (float fs) {optionFontSizeRel = fs;}


  virtual ~GuiOptionChooser()
  {
    cairo_pattern_destroy(bgColor);
    cairo_pattern_destroy(fontColor);
    cairo_pattern_destroy(borderColor);
    cairo_pattern_destroy(highlightColor);
  }

  ///@brief Remove all options
  void clearOptions() {
    options.clear();
  }

  ///@brief Adding a new option with text v
  void addOption(VerbListener::Verb v) {
    properties p;
    p.highlight = false;
    options.push_back(std::pair<VerbListener::Verb, properties> (v,p));
  }

  virtual void clickat(int , int ) {
    trigger = true;
  }

  virtual void clickmove(int, int ) {
  }

  virtual void clickrelease(int x, int y) {
    if (!trigger) return;

    for (unsigned int i=0; i<options.size(); ++i)
      {
	int xlow = getSizeX()*(i*(interoptionWidth)+optionOffsetX);
	int xhigh = getSizeX()*(i*interoptionWidth+optionOffsetX + optionWidth);
	int ylow = getSizeY()*(i*(interoptionHeight)+optionOffsetY);
	int yhigh = getSizeY()*(optionHeight+i*interoptionHeight+optionOffsetY);

	if (x >= xlow && x <= xhigh
	    && y >= ylow && y <= yhigh)
	  {
	    trigger = false;
	    clickon(i);
	    return;
	  }
      }
    
  }

  ///@brief Set the highlight of option s to h.
  ///
  ///Might send a NoSuchOptionException.
  void setHighlight (VerbListener::Verb s, bool h) {
    for (auto it = options.begin(); it != options.end(); ++it) {
      if (it->first == s) {
	it->second.highlight = h;
	return;
      }
    }
#ifndef NOEXCEPTION
    throw NoSuchOptionException (s);
#endif
  }


private:
  void clickon(int i) {
    vl.say(this, options[i].first);
  }

public:

  ///@brief Tells whether option s is highlighted.
  ///
  ///Might send a NoSuchOptionException.
  ///@return true if option s is highlighted.
  bool isHighlighted (VerbListener::Verb s) {
    for (auto it = options.begin(); it != options.end(); ++it) {
      if (it->first == s) {
	return it->second.highlight;
      }
    }
#ifndef NOEXCEPTION
    throw NoSuchOptionException (s);
#endif
  }

  ///tells whether an option is there.
  bool isPresent (VerbListener::Verb s) {
    for (auto it = options.begin(); it != options.end(); ++it) {
      if (it->first == s) {
	return true;
      }
    }
    return false;
  }



  virtual void setSizeX(int sx)
  {
    CairoGraphicController::setSizeX(sx);
  }

  virtual void setSizeY(int sy)
  {
    CairoGraphicController::setSizeY(sy);
    optionFontSize = optionFontSizeRel*getSizeY();
  }



  virtual void render(cairo_t* cr)
  { 
    //Log::log<<"rendering opt"<<Log::endl;
    cairo_save(cr);

    for (unsigned int i=0; i<options.size(); ++i)
      {
	cairo_rectangle (cr,
			 getSizeX()*(optionOffsetX+i*(interoptionWidth)), getSizeY()*(optionOffsetY+i*(interoptionHeight)),
			 getSizeX()*optionWidth, getSizeY()*optionHeight);

	//inside
	cairo_set_source (cr, bgColor);
	cairo_fill_preserve(cr);

	if (options[i].second.highlight) {
	  cairo_set_source (cr, highlightColor);
	  cairo_fill_preserve(cr);
	}

	//contour
	cairo_set_source (cr, borderColor);
	cairo_stroke(cr);

	//print tag
	cairo_set_source (cr, fontColor);
	cairo_move_to(cr,
		      getSizeX()*(optionOffsetX+i*(interoptionWidth)+optionWidthInternal),
		      getSizeY()*(optionOffsetY+i*(interoptionHeight)
				  +(optionHeight+optionFontSizeRel)/2.));
	show_text(cr, options[i].first, optionFontSize);
      }

    cairo_restore(cr);

  }


  virtual bool quit() const{return false;}

  virtual void deserialize(const char* ) 
  {
    std::cerr<<"NYE"<<std::endl;
  }

  virtual void serialize(char* ) const
  {
    std::cerr<<"NYE"<<std::endl;
  }
  
  virtual size_t serializesize() const
  {
    std::cerr<<"NYE"<<std::endl;
    return 1;
  }
};

#endif
