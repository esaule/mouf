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

#ifndef MOUF_GUI_H
#define MOUF_GUI_H

#include "cairo_graphic_controller.hpp"
#include <string.h>

class CairoTemplate : public CairoGraphicController
{
  bool quitting;
  

public:
  CairoTemplate()
  {
    quitting = false;
  }

  
  virtual void clickat(int x, int y)
  {
    quitting = true;
  }

  virtual void clickmove(int x, int y){}
  virtual void clickrelease(int x, int y){}

  virtual bool quit() const
  {
    return quitting;
  }

  virtual void setSizeY(int sy)
  {
    CairoGraphicController::setSizeY(sy);
  }

  virtual void setSizeX(int sx)
  {
    CairoGraphicController::setSizeX(sx);
  }



  virtual void render(cairo_t* cr)
  {
  }
  
  //there is nothing to serialize.
  virtual void deserialize(const char* c)
  {
  }

  virtual void serialize(char* c) const
  {
  }

  virtual size_t serializesize() const
  {
    return 0;
  }

  virtual ~CairoTemplate()
  {
  }
};

#endif
