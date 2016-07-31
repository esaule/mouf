#ifndef KEYBOARD_CAIRO_
#define KEYBOARD_CAIRO_

#include "cairo_graphic_controller.hpp"
#include "gui_option_chooser.hpp"
#include "cairo_text_display.hpp"

struct KeyboardListener {
  virtual void KeyboardText(std::string s) = 0;

  virtual ~KeyboardListener(){}
};

class Keyboard : public CairoGraphicController,
		 public VerbListener {
protected:

  std::string currenttext;

  GuiLayout gl;

  util::smart_ptr<CairoTextDisplay> currenttext_disp;
  
  util::smart_ptr<GuiOptionChooser> line1;
  util::smart_ptr<GuiOptionChooser> line2;
  util::smart_ptr<GuiOptionChooser> line3;
  util::smart_ptr<GuiOptionChooser> line4;

  util::smart_ptr<KeyboardListener> kl;

public:
  void setKeyboardListener(util::smart_ptr<KeyboardListener> kl) {
    this->kl = kl;
  }

  Keyboard() {
    currenttext_disp = new CairoTextDisplay("Test");
    currenttext_disp->setTextOffsetX(0.01);
    currenttext_disp->setTextOffsetY(0.1);
    currenttext_disp->setFontSize(0.7);
    currenttext_disp->changeText(currenttext.c_str());
    gl.addObject(currenttext_disp);
    gl.showObject(currenttext_disp, true);
    gl.allowInput(currenttext_disp, false);

    line1 = new GuiOptionChooser (*this);
    line1->setOffsetX(.005);
    line1->setOffsetY(.01);
    line1->setInterHeight(0.);
    line1->setInterWidth(1./13);
    line1->setOptionHeight(.8);
    line1->setOptionWidth(1./14.);
    line1->setOptionWidthInternal(.01);
    line1->setFontSize(1./5);

    line1->addOption("1");
    line1->addOption("2");
    line1->addOption("3");
    line1->addOption("4");
    line1->addOption("5");
    line1->addOption("6");
    line1->addOption("7");
    line1->addOption("8");
    line1->addOption("9");
    line1->addOption("0");
    line1->addOption("-");
    line1->addOption("=");
    line1->addOption("Back");

    gl.addObject(line1);
    gl.showObject(line1, true);
    gl.allowInput(line1, true);


    //    

    line2 = new GuiOptionChooser (*this);
    line2->setOffsetX(.005);
    line2->setOffsetY(.01);
    line2->setInterHeight(0.);
    line2->setInterWidth(1./13);
    line2->setOptionHeight(.8);
    line2->setOptionWidth(1./14.);
    line2->setOptionWidthInternal(.01);
    line2->setFontSize(1./5);

    line2->addOption("q");
    line2->addOption("w");
    line2->addOption("e");
    line2->addOption("r");
    line2->addOption("t");
    line2->addOption("y");
    line2->addOption("u");
    line2->addOption("i");
    line2->addOption("o");
    line2->addOption("p");
    line2->addOption("[");
    line2->addOption("]");
    line2->addOption("\\");

    gl.addObject(line2);
    gl.showObject(line2, true);
    gl.allowInput(line2, true);

    //    

    line3 = new GuiOptionChooser (*this);
    line3->setOffsetX(.005);
    line3->setOffsetY(.01);
    line3->setInterHeight(0.);
    line3->setInterWidth(1./13);
    line3->setOptionHeight(.8);
    line3->setOptionWidth(1./14.);
    line3->setOptionWidthInternal(.01);
    line3->setFontSize(1./5);

    line3->addOption("a");
    line3->addOption("s");
    line3->addOption("d");
    line3->addOption("f");
    line3->addOption("g");
    line3->addOption("h");
    line3->addOption("j");
    line3->addOption("k");
    line3->addOption("l");
    line3->addOption(";");
    line3->addOption("'");
    line3->addOption("Ret");

    gl.addObject(line3);
    gl.showObject(line3, true);
    gl.allowInput(line3, true);

    //

    line4 = new GuiOptionChooser (*this);
    line4->setOffsetX(.005);
    line4->setOffsetY(.01);
    line4->setInterHeight(0.);
    line4->setInterWidth(1./13);
    line4->setOptionHeight(.8);
    line4->setOptionWidth(1./14.);
    line4->setOptionWidthInternal(.01);
    line4->setFontSize(1./5);

    line4->addOption("z");
    line4->addOption("x");
    line4->addOption("c");
    line4->addOption("v");
    line4->addOption("b");
    line4->addOption("n");
    line4->addOption("m");
    line4->addOption(",");
    line4->addOption(".");
    line4->addOption("/");

    gl.addObject(line4);
    gl.showObject(line4, true);
    gl.allowInput(line4, true);

  }

  virtual void setSizeX(int sx) {
    CairoGraphicController::setSizeX(sx);

    gl.placeObjectX(currenttext_disp, 0);
    gl.sizeObjectX(currenttext_disp, sx);

    gl.placeObjectX(line1, 0);
    gl.sizeObjectX(line1, sx);

    gl.placeObjectX(line2, 0);
    gl.sizeObjectX(line2, sx);

    gl.placeObjectX(line3, 0);
    gl.sizeObjectX(line3, sx);

    gl.placeObjectX(line4, 0);
    gl.sizeObjectX(line4, sx);
  }

  virtual void setSizeY(int sy) {
    int nbobj = 5;

    CairoGraphicController::setSizeY(sy);

    gl.placeObjectY(currenttext_disp, 0*sy/nbobj);
    gl.sizeObjectY(currenttext_disp, sy/nbobj);

    gl.placeObjectY(line1, 1*sy/nbobj);
    gl.sizeObjectY(line1, sy/nbobj);

    gl.placeObjectY(line2, 2*sy/nbobj);
    gl.sizeObjectY(line2, sy/nbobj);

    gl.placeObjectY(line3, 3*sy/nbobj);
    gl.sizeObjectY(line3, sy/nbobj);

    gl.placeObjectY(line4, 4*sy/nbobj);
    gl.sizeObjectY(line4, sy/nbobj);
  }

  void setText (std::string s) {
    currenttext = s;
    currenttext_disp->changeText(currenttext.c_str());
  }

  virtual void say (const GuiOptionChooser*, VerbListener::Verb verb){
    if (verb == "Back") {
      if (currenttext.length() > 0) {
	currenttext = currenttext.substr(0, currenttext.length()-1);
      }
    }
    else if (verb == "Ret") {
      if (kl) {
	kl->KeyboardText(currenttext);
      }
    }
    else {
      currenttext.append(verb);
      
      unsigned int maxlength = 20;
      if (currenttext.length() > maxlength) {
	currenttext = currenttext.substr(0, maxlength);
      }
    }

    currenttext_disp->changeText(currenttext.c_str());
  }

  virtual void clickat(int x, int y) {
    gl.clickat(x, y);
  }
  
  virtual void clickmove(int x, int y) {
    gl.clickmove(x, y);
  }

  virtual void clickrelease(int x, int y) {
    gl.clickrelease(x, y);
  }
  
  virtual void render(cairo_t* cr) {
    cairo_rectangle(cr, 0, 0, getSizeX(), getSizeY());
    cairo_fill(cr);
    gl.render(cr);
  }
  
  virtual bool quit() const
  { return false; }


};

#endif
