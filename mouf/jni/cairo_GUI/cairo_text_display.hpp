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

#ifndef CAIRO_TEXT_DISPLAY_H
#define CAIRO_TEXT_DISPLAY_H

#include "cairo_graphic_controller.hpp"
#include <string.h>

class CairoTextDisplay : public CairoGraphicController
{
  bool quitting;
  float textFontSize;
  float textFontSizeRel;
  float textOffsetX;
  float textOffsetY;
  char* text;
  int sizetext;

  float font_width_to_height_ratio; ///probably font dependent.

  bool need_to_cut;

  cairo_pattern_t * bgcolor;
  cairo_pattern_t * fgcolor;

  std::string s;
    
public:
  CairoTextDisplay(std::string text_to_display)
  {
    quitting = false;

    setFontSize(.07);
    font_width_to_height_ratio = .4;


    textOffsetX = .05;
    textOffsetY = .05;
    
    bgcolor = cairo_pattern_create_rgb(1,1,1);
    fgcolor = cairo_pattern_create_rgb(0,0,0);

    textFontSizeRel = .1;
    
    setText(text_to_display);
  }

  void setText(std::string text_to_display) {
    s = text_to_display;
    const char* te = s.c_str();
    sizetext = strlen(te);
    text = new char[sizetext+1];
    memcpy(text, te, sizeof(char)*(sizetext+1));

    need_to_cut = true;

  }
  
  void setFontSize(float f){
    textFontSizeRel = f;
    textFontSize = textFontSizeRel*getSizeY();
  }

  virtual void clickat(int , int )
  {
    quitting = true;
  }

  virtual void clickmove(int , int ){}
  virtual void clickrelease(int , int ){}

  virtual bool quit() const
  {
    return quitting;
  }


  virtual void setSizeY(int sy)
  {
    CairoGraphicController::setSizeY(sy);
    textFontSize = textFontSizeRel*getSizeY();
    
    need_to_cut = true;
  }

  virtual void setSizeX(int sx)
  {
    CairoGraphicController::setSizeX(sx);

    need_to_cut = true;
  }


  void resetCut()
  {
    //change all '\0' to ' '
    for (int i=0; i<sizetext; ++i)
      if (text[i] == '\0')
	text[i] = ' ';
  }

  void reCutAt(int maxsize)
  {
    resetCut();
    
    
    char* t = text;
    char *oldt = t;
    t += maxsize;
    
    
    while (t < text+sizetext)
      {
	while (*t != ' ' && *t != '\0') {
	  --t;
	}
	if (t <= oldt)
	    return;
	*t = '\0';
	oldt = t;
	t += maxsize;
      }
  }

  virtual void render(cairo_t* cr)
  {
    if (need_to_cut)
      {
	float lettersizey = textFontSize;
	float lettersizex = font_width_to_height_ratio*lettersizey;
	float availablesize = (1-2.*textOffsetX)*getSizeX();
	float cutsize = availablesize/lettersizex;
		
	reCutAt(cutsize);
	need_to_cut = false;
      }

    char* t = text;
    
    //    cairo_surface_t* bgsprite = SpriteCollection::sc.getSprite("wood3.png");
    cairo_surface_t* bgsprite = NULL;
    if (bgsprite != NULL)
      {
	blit(cr,bgsprite,0,0, getSizeX(), getSizeY());
      }
    else
      {
	cairo_set_source(cr, bgcolor);
	cairo_paint(cr);
      }
    
    cairo_set_source(cr, fgcolor);
    
    int i=1;
    while (t < text+sizetext)
      {
	cairo_move_to(cr,
		      getSizeX()*(textOffsetX),
		      getSizeY()*(textOffsetY+i*textFontSizeRel));

	show_text(cr, t, textFontSize);

	t += strlen(t) + 1;
	++i;
      }
    
//     cairo_rectangle (cr,
// 		     getSizeX()*(textOffsetX), getSizeY()*(textOffsetY+textFontSizeRel), textFontSize, textFontSize
// 		     );
//     cairo_stroke(cr);

//     cairo_rectangle (cr,
// 		     getSizeX()*(textOffsetX), getSizeY()*(textOffsetY+textFontSizeRel), 
// 		     getSizeX()*(1-2.*textOffsetX)/(textFontSizeRel*width_to_height_ratio)
// , textFontSize
// 		     );
//     cairo_stroke(cr);

  }
  
  //there is nothing to serialize.
  virtual void deserialize(const char* )
  {
  }

  virtual void serialize(char* ) const
  {
  }

  virtual size_t serializesize() const
  {
    return 0;
  }

  virtual ~CairoTextDisplay()
  {
    delete[] text;
  }
};

#endif
