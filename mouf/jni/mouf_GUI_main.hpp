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
#include "cairo_GUI/GuiLayout.hpp"
#include "cairo_GUI/gui_interface.hpp"
#include "cairo_GUI/gui_option_chooser.hpp"
#include "smart_ptr.hpp"

class MoufGUI : public CairoGraphicController,
		public VerbListener
{
  bool quitting;

  GuiLayout gl;

  util::smart_ptr<CairoTextDisplay> title;
  util::smart_ptr<CairoTextDisplay> definition;
  util::smart_ptr<GuiOptionChooser> mf;

  util::smart_ptr<GuiOptionChooser> metachoice;

  
public:
  MoufGUI() {
    quitting = false;


    title = new CairoTextDisplay ("title");
    gl.addObject(title);
    gl.showObject(title, true);
    gl.allowInput(title, false);
    //title->setFontSize(2.);
    
    definition = new CairoTextDisplay ("definition");
    gl.addObject(definition);
    gl.showObject(definition, true);
    gl.allowInput(definition, false);

    mf = new GuiOptionChooser (*this);
    gl.addObject(mf);
    gl.showObject(mf, true);
    gl.allowInput(mf, true);

    mf->addOption("M");
    mf->addOption("F");

    mf->setInterHeight(0);
    mf->setInterWidth(.49);
    mf->setOffsetX(0);
    mf->setOffsetY(0);
    mf->setOptionHeight(1.0);
    mf->setOptionWidth(.48);

    metachoice = new GuiOptionChooser (*this);
    gl.addObject(metachoice);
    gl.showObject(metachoice, true);
    gl.allowInput(metachoice, true);

    metachoice->addOption("Next");
    metachoice->addOption("Back");

    metachoice->setInterHeight(0);
    metachoice->setInterWidth(.49);
    metachoice->setOffsetX(0);
    metachoice->setOffsetY(0);
    metachoice->setOptionHeight(1.0);
    metachoice->setOptionWidth(.48);

  }

  virtual void say(const GuiOptionChooser* , Verb verb) {
    std::cout<<"Say: "<<verb<<std::endl;
  }
  
  
  virtual void clickat(int x, int y) {
    gl.clickat(x,y);
  }

  virtual void clickmove(int x, int y) {
    gl.clickmove(x,y);
  }
  virtual void clickrelease(int x, int y) {
    gl.clickrelease(x,y);
  }

  virtual bool quit() const
  {
    return quitting;
  }

  virtual void setSizeY(int sy)
  {
    CairoGraphicController::setSizeY(sy);

    title->setSizeY (sy/4);
    gl.placeObjectY(title, 0);
    gl.sizeObjectY(title, sy/4);

    definition->setSizeY (sy/4);
    gl.placeObjectY(definition, sy/4);
    gl.sizeObjectY(definition, sy/4);

    mf->setSizeY (sy/4);
    gl.placeObjectY(mf, 2*sy/4);
    gl.sizeObjectY(mf, sy/4);

    metachoice->setSizeY (sy/4);
    gl.placeObjectY(metachoice, 3*sy/4);
    gl.sizeObjectY(metachoice, sy/4);
    
  }

  virtual void setSizeX(int sx)
  {
    CairoGraphicController::setSizeX(sx);

    gl.placeObjectX(title, 0);
    gl.sizeObjectX(title, sx);
    title->setSizeX (sx);

    gl.placeObjectX(definition, 0);
    gl.sizeObjectX(definition, sx);
    definition->setSizeX (sx);


    gl.placeObjectX(mf, 0);
    gl.sizeObjectX(mf, sx);
    mf->setSizeX (sx);

    gl.placeObjectX(metachoice, 0);
    gl.sizeObjectX(metachoice, sx);
    metachoice->setSizeX (sx);

  }



  virtual void render(cairo_t* cr)
  {
    gl.render(cr);
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

  virtual ~MoufGUI()
  {
  }
};

#endif
