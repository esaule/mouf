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
#include "dictionary.hpp"

class MoufGUI : public CairoGraphicController,
		public VerbListener
{
  bool quitting;

  GuiLayout gl;

  util::smart_ptr<CairoTextDisplay> title;
  util::smart_ptr<CairoTextDisplay> definition;
  util::smart_ptr<GuiOptionChooser> mf;

  util::smart_ptr<GuiOptionChooser> metachoice;

  std::vector<dict_entry> dict;

  dict_entry current_entry;
  
  int entry_index;

  void nextword() {
    entry_index = (entry_index+1)%dict.size();
    
    current_entry = dict[entry_index];
    title->setText (current_entry.word);
    definition->setText (current_entry.definition);

    mf->setHighlight("M", false);
    mf->setHighlight("F", false);
  }
  
public:
  MoufGUI() {
    entry_index = 0;
    quitting = false;


    title = new CairoTextDisplay ("title");
    gl.addObject(title);
    gl.showObject(title, true);
    gl.allowInput(title, false);
    //title->setFontSize(2.);
    title->setFontSize(.4);

    definition = new CairoTextDisplay ("definition");
    gl.addObject(definition);
    gl.showObject(definition, true);
    gl.allowInput(definition, false);
    definition->setFontSize(.111);
    
    mf = new GuiOptionChooser (*this);
    gl.addObject(mf);
    gl.showObject(mf, true);
    gl.allowInput(mf, true);

    mf->addOption("M");
    mf->addOption("F");

    mf->setInterHeight(0);
    mf->setInterWidth(.51);
    mf->setOffsetX(0);
    mf->setOffsetY(0);
    mf->setOptionHeight(1.0);
    mf->setOptionWidth(.49);

    mf->setFontSize(.75);

    metachoice = new GuiOptionChooser (*this);
    gl.addObject(metachoice);
    gl.showObject(metachoice, true);
    gl.allowInput(metachoice, true);

    metachoice->addOption("Next");
    metachoice->addOption("Back");

    metachoice->setInterHeight(0);
    metachoice->setInterWidth(.51);
    metachoice->setOffsetX(0);
    metachoice->setOffsetY(0);
    metachoice->setOptionHeight(1.0);
    metachoice->setOptionWidth(.49);

    metachoice->setFontSize(.5);

    dict = load_dictionary("dictionary");
    nextword();
  }

  virtual void say(const GuiOptionChooser* , Verb verb) {
    std::cout<<"Say: "<<verb<<std::endl;

    if (verb.compare("M") == 0 || verb.compare("F") == 0) {
      if (current_entry.genre == 'f')
	mf->setHighlight("F", true);
      if (current_entry.genre == 'm')
	mf->setHighlight("M", true);
    }

    if (verb.compare("Next") == 0)
      nextword();

    if (verb.compare("Back") == 0)
      quitting = true;
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

    title->setSizeY (.25*sy);
    gl.placeObjectY(title, 0);
    gl.sizeObjectY(title, .25*sy);

    definition->setSizeY (.375*sy);
    gl.placeObjectY(definition, .25*sy);
    gl.sizeObjectY(definition, .375*sy);

    mf->setSizeY (.25*sy);
    gl.placeObjectY(mf, .625*sy);
    gl.sizeObjectY(mf, .25*sy);

    metachoice->setSizeY (.125*sy);
    gl.placeObjectY(metachoice, .875*sy);
    gl.sizeObjectY(metachoice, .125*sy);
    
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

  virtual ~MoufGUI()
  {
  }
};

#endif
