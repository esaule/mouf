#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <sstream>
#include <SDL/SDL.h>
#include "SpriteCollection.hpp"
#include <list>
#include "graphics.hpp"
#include <GL/gl.h>

class Display
{
  SDL_Surface* window;

  SpriteCollection sc;

  const int sizeX, sizeY;
  float zoom;

  //  cairo_surface_t* surf;

  int life;

  //  cairo_t* cr;

  int textlimit;
  std::list<std::string> textbuf;

  int score;
  double fps;

  void drawLife(int nblife);

  void drawScore(int score);

  void drawControls();

  void drawFps(double fps);


  bool flip;

public:
  Display();

  void setZoom(float f){zoom = f;}

  void setScore(int score)
  {
    this->score = score;
  }

  void setNbLife(int nblife)
  {
    this->life = nblife;
  }
  
  void setFPS(double fps)
  {
    this->fps = fps;
  }
  
  void blit (smart_ptr<Sprite> spr,
   	     double x, double y, 
	     double width, double height);

  void black();

  void drawObject(const Object& s,const Color& c);

  bool preload_sprite (const std::string& sprite);

  bool drawSprite(const Object& s, const std::string& sprite, const Offset& off);
  void white();

  void displaytext();

  void show();

  void printMessage(const std::string & s)
  {
    textbuf.push_back(s);
    while (textbuf.size() > textlimit)
      {
	textbuf.pop_front();
      }
  }

  void printText(float x, float y, const std::string& s);

  ///this is a debuging function
  void print_info()
  {
    sc.print_info();
  }

  ~Display()
  {
    SDL_Quit ();
  }
};


#endif
