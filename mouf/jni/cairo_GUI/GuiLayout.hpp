#ifndef GUI_LAYOUT_
#define GUI_LAYOUT_

#include "cairo_graphic_controller.hpp"
#include <map>
#include <algorithm>
#include "smart_ptr.hpp"

class GuiLayout {

protected:

  struct properties {
    int offsetX;
    int offsetY;
    int sizeX;
    int sizeY;
    bool show;
    bool input;
  };

  std::map<util::smart_ptr<CairoGraphicController>, properties > objects;

public:
  void addObject(util::smart_ptr<CairoGraphicController> o) {
    assert (o);
    assert (objects.find(o) == objects.end());

    
    properties p;
    p.offsetX = 0;
    p.offsetY = 0;
    p.sizeX = 100;
    p.sizeY = 100;
    p.show = false;
    p.input = false;
    objects.insert(std::pair<util::smart_ptr<CairoGraphicController>, properties> (o,p));
  }

  ///places an object at given coordinates.
  void placeObject(util::smart_ptr<CairoGraphicController> o, int x, int y) {
    assert (o);
    auto it = objects.find(o);
    assert (it != objects.end());

    it->second.offsetX = x;
    it->second.offsetY = y;
  }

  void placeObjectX(util::smart_ptr<CairoGraphicController> o, int x) {
    assert (o);
    auto it = objects.find(o);
    assert (it != objects.end());

    it->second.offsetX = x;
  }

  void placeObjectY(util::smart_ptr<CairoGraphicController> o, int y) {
    assert (o);
    
    auto it = objects.find(o);
    assert (it != objects.end());

    it->second.offsetY = y;
  }



  ///changes the size of a given object
  void sizeObject(util::smart_ptr<CairoGraphicController> o, int w, int h) {
    assert (o);
    
    auto it = objects.find(o);
    assert (it != objects.end());

    it->second.sizeX = w;
    it->first->setSizeX(w);
    it->second.sizeY = h;
    it->first->setSizeY(h);
  }

  ///changes the size of a given object in the x dimension
  void sizeObjectX(util::smart_ptr<CairoGraphicController> o, int w) {
    auto it = objects.find(o);
    assert (it != objects.end());

    it->second.sizeX = w;
    it->first->setSizeX(w);
  }

  ///changes the size of a given object in the y dimension
  void sizeObjectY(util::smart_ptr<CairoGraphicController> o, int h) {
    assert (o);
    
    auto it = objects.find(o);
    assert (it != objects.end());

    it->second.sizeY = h;
    it->first->setSizeY(h);
  }

  ///should an object been shown or not.
  void showObject(util::smart_ptr<CairoGraphicController> o, bool show) {
    assert (o);
    
    auto it = objects.find(o);
    assert (it != objects.end());

    it->second.show = show;
  }

  ///should an object receive inputs.
  void allowInput(util::smart_ptr<CairoGraphicController> o, bool input) {
    assert (o);
    
    auto it = objects.find(o);
    assert (it != objects.end());

    it->second.input = input;
  }

  void render(cairo_t* cr) {
    for (auto it = objects.begin(); it!= objects.end(); ++it) {
      assert (it->first);
      if (it->second.show) {
	cairo_save(cr);
	cairo_translate(cr, it->second.offsetX, it->second.offsetY);
	cairo_rectangle(cr, 0, 0, it->second.sizeX, it->second.sizeY);
	cairo_clip(cr);
	
	it->first->render(cr);
	cairo_restore(cr);

      }
    }
  }

  void clickat(int x, int y) {
    for (auto it = objects.begin(); it!= objects.end(); ++it) {
      assert (it->first);
     
      if (it->second.input)
	if (x >= it->second.offsetX && y >= it->second.offsetY
	    && x <= it->second.offsetX+it->second.sizeX && y <= it->second.offsetY+it->second.sizeY)
	  it->first->clickat(x-it->second.offsetX, y-it->second.offsetY);
    }
  }

  void clickmove(int x, int y) {
    for (auto it = objects.begin(); it!= objects.end(); ++it) {
      assert (it->first);
     
      if (it->second.input)
	if (x >= it->second.offsetX && y >= it->second.offsetY
	    && x <= it->second.offsetX+it->second.sizeX && y <= it->second.offsetY+it->second.sizeY)
	  it->first->clickmove(x-it->second.offsetX, y-it->second.offsetY);
    }
  }

  void clickrelease(int x, int y) {
    for (auto it = objects.begin(); it!= objects.end(); ++it) {
      assert (it->first);
     
      if (it->second.input)
	if (x >= it->second.offsetX && y >= it->second.offsetY
	    && x <= it->second.offsetX+it->second.sizeX && y <= it->second.offsetY+it->second.sizeY)
	  it->first->clickrelease(x-it->second.offsetX, y-it->second.offsetY);
    }
  }
};

#endif
