#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <sstream>
#include <cairo.h>
#include "SpriteCollection.hpp"
#include <list>
#include "graphics.hpp"

#include "StringFinder.hpp"

struct DisplayBackEnd {
  virtual cairo_surface_t* init() = 0;
  virtual cairo_t* getCairoContext() = 0;
  virtual void flip() = 0;
  virtual void end() = 0;
}; 

#ifndef ANDROID
#include <SDL/SDL.h>
struct SDL_BackEnd : public DisplayBackEnd {
  SDL_Surface* window;
  cairo_surface_t* surf;

  virtual cairo_surface_t* init() {
    int sizeX = 800;
    int sizeY = 600;

    if (SDL_Init (SDL_INIT_VIDEO) == -1)
      {
	Log::logerr<<"error in sdl init"<<Log::endl;
      }
    
    window = SDL_SetVideoMode(sizeX, sizeY, 0, SDL_HWSURFACE
			      |SDL_FULLSCREEN
                              |SDL_DOUBLEBUF);
    
    if (window == NULL)
      Log::logerr<<"error in sdl setvideomode"<<Log::endl;
    
    surf = cairo_image_surface_create_for_data ((unsigned char*)window->pixels,
						CAIRO_FORMAT_RGB24,
						window->w,
						window->h,
						window->pitch);
    
     return surf;
  }
  
  virtual cairo_t* getCairoContext() {
    return cairo_create(surf);
  }

  virtual void flip() {
    if (SDL_Flip(window) == -1)
      {
    	debug_message("error flip");
      }

    SDL_UpdateRect(window, 0, 0, 0, 0);
  }

  virtual void end() {
    SDL_Quit();
  }
};
#else
struct Android_BackEnd : public DisplayBackEnd {
  cairo_t* cc;
  void setContext(cairo_t* c){cc = c;}

  virtual cairo_surface_t* init(){return NULL;}
  virtual cairo_t* getCairoContext() {return cc;}
  virtual void flip(){}
  virtual void end(){}
};
#endif


class Display
{
  DisplayBackEnd * be;
  SpriteCollection sc;

  float zoomX;
  float zoomY;

  int life;

  cairo_t* cr;

  int textlimit;
  std::list<std::string> textbuf;

  std::string background;
  float background_offset; //in [0:1] vertical scroll

  int score;
  double fps;

  void drawLife(int nblife)
  {
    cairo_save(cr);

    cairo_set_source_rgb (cr, 0., 0., 0.);

    cairo_move_to(cr, 10, 60);

    cairo_set_font_size(cr, 15);

    std::stringstream ss;
    ss<<"Life : "<<nblife;

    cairo_show_text (cr, ss.str().c_str());
    
    cairo_restore(cr);
  }

  void drawScore(int score)
  {
    cairo_save(cr);

    cairo_set_source_rgb (cr, 0., 0., 0.);

    cairo_move_to(cr, 10, 20);

    cairo_set_font_size(cr, 15);

    std::stringstream ss;
    ss<<StringFinder::singleton->find("game_score") <<score;

    cairo_show_text (cr, ss.str().c_str());
    
    cairo_restore(cr);
  }

  void drawControls()
  {
    cairo_save(cr);

    cairo_set_source_rgb (cr, 0., 0., 0.);

    cairo_move_to(cr, 10, 20);

    cairo_translate(cr, 0, 260);
    cairo_move_to(cr, 0, 0);
    cairo_set_font_size(cr, 20);
    cairo_show_text (cr, "Controls:");
    cairo_set_font_size(cr, 15);
    cairo_translate(cr, 0, 15);
    cairo_move_to(cr, 0, 0);
    cairo_show_text (cr, "Arrows: direction");
    
    cairo_translate(cr, 0, 15);
    cairo_move_to(cr, 0, 0);
    cairo_show_text (cr, "Space: shoot");
    
    cairo_translate(cr, 0, 15);
    cairo_move_to(cr, 0, 0);
    cairo_show_text (cr, "S: Speed up");

    cairo_translate(cr, 0, 15);
    cairo_move_to(cr, 0, 0);
    cairo_show_text (cr, "D: Slow down");
    
    cairo_translate(cr, 0, 15);
    cairo_move_to(cr, 0, 0);
    cairo_show_text (cr, "Q: Quit");
    
    cairo_translate(cr, 0, 15);
    cairo_move_to(cr, 0, 0);
    cairo_show_text (cr, "E: Engine Mode");    

    cairo_restore(cr);
  }

  void drawFps(double fps)
  {
    cairo_save(cr);

    cairo_set_source_rgb (cr, 0., 0., 0.);

    cairo_move_to(cr, 10, 40);

    cairo_set_font_size(cr, 15);

    std::stringstream ss;
    ss<<"FPS : "<<fps;

    cairo_show_text (cr, ss.str().c_str());
    
    cairo_restore(cr);
  }

  bool hud;

  int realscreenX;
  int realscreenY;

  bool respforbackend;
public:
  void setZoom(float z)
  {
    setZoomX(z);
    setZoomY(z);
  }

  void setZoomX(float z) {zoomX = z;}
  void setZoomY(float z) {zoomY = z;}

  void setBackground (std::string s) {
    background = s;
    background_offset = 0;
  }
  
  void setBackgroundOffset(float f) {
    background_offset = f;
  }

  void setRealScreenSizeX(int sx) {
    realscreenX = sx;
  }

  void setRealScreenSizeY(int sy) {
    realscreenY = sy;
  }

  int getRealScreenSizeX() {
    return realscreenX;
  }

  int getRealScreenSizeY() {
    return realscreenY;
  }


  Display(DisplayBackEnd* b = NULL)
    :sc("data/sprite"), life(0), be(b),realscreenX(400),realscreenY(600)
  {
    sc.setFormat(SpriteCollection::sc.getFormat());
    setZoom(0.01);
    hud = true;
    respforbackend = false;
    if (be == NULL){
#ifndef ANDROID
      be = new SDL_BackEnd();
#else
      be = new Android_BackEnd();
#endif
      respforbackend = true;
    }
    be->init(); //no need to keep the return value.
    textlimit = 59;
    score = 0;
  }

  void setScore(int score)
  {
    this->score = score;
  }

  void displayHUD(bool hudD)
  {
    hud = hudD;
  }

  void setNbLife(int nblife)
  {
    this->life = nblife;
  }
  
  void setFPS(double fps)
  {
    this->fps = fps;
  }

  ///blit src to cr on the rectangle (x,y,x+width,y+height)
  void blit (cairo_t* cr, cairo_surface_t* src,
	     double x, double y, 
	     double width, double height)
  {
      cairo_save(cr);

      cairo_rectangle(cr,x,y,width,height);
      cairo_clip(cr);

      cairo_translate (cr,x,y);

      cairo_rectangle (cr,0,0,width,height);

      cairo_scale(cr,
		  width/cairo_image_surface_get_width(src),
		  height/cairo_image_surface_get_height(src));

      cairo_set_source_surface(cr, src, 0, 0);
      cairo_fill(cr);

      cairo_restore(cr);
  }

  void white()
  {
    sc.setFormat(SpriteCollection::sc.getFormat());
    cr = be -> getCairoContext();

    cairo_set_source_rgb(cr,1.0,1.0,1.0);
    cairo_paint(cr);
  }

  void black()
    {
      sc.setFormat(SpriteCollection::sc.getFormat());
      cr = be -> getCairoContext();

      if (hud)
	{
	  cairo_set_source_rgb(cr,0,0,1.0);
	  cairo_paint(cr);

	  //right part of the screen
	  cairo_save(cr);
	  cairo_rectangle(cr, 625, 0, 175, 600);
	  cairo_clip(cr);
	  cairo_translate(cr, 625,0);
	  displaytext();
	  cairo_restore(cr);
	  
	  //left part of the screen
	  cairo_save(cr);
	  cairo_rectangle(cr, 0, 0, 175, 600);
	  cairo_clip(cr);
	  drawScore(score);
	  drawControls();
	  drawFps(fps);
	  drawLife(life);
	  cairo_restore(cr);
	  
	  //middle part of the screen (setup)
	  cairo_rectangle(cr, 175, 0, 450, 600);
	  cairo_clip(cr);
	  
	  cairo_translate(cr, 175,0);
	  
	  cairo_set_source_rgb(cr,1.0,1.0,1.0);
	  cairo_paint(cr);
	  
	  cairo_scale(cr, zoomX, zoomY);
	}
      else
	{

	  if (background != "")
	    {
	      //int offset = (framenumber)%60000;
	      //	      d.drawSprite (Object(0,0,-60000+offset,0), "background.png", Offset(Position(0,0), Position(45000,60000)), true);

	      cairo_surface_t* spr;
	      spr = sc.getSprite(background, realscreenX, realscreenY);
	      int off = ((float)realscreenY)*background_offset;
	      blit (cr, spr,
		    0, off - realscreenY,
		    realscreenX, realscreenY);
	      blit (cr, spr,
		    0, off,
		    realscreenX, realscreenY);
	    }
	  else
	    {
	      cairo_set_source_rgb(cr,1.0,1.0,1.0);
	      cairo_paint(cr);
	    }
	  cairo_scale(cr, zoomX, zoomY);
	}	
    }

  // void drawObject(const Object& s,const Color& c)
  //   {
  //     cairo_rectangle(cr, s.left(), s.bottom(), s.width(), s.height());
  //     cairo_set_source_rgb(cr,c.getR()/255.,c.getG()/255.,c.getB()/255.);
  //     cairo_fill(cr);
  //   }

  bool preload_sprite (const std::string& sprite)
  {
    cairo_surface_t* spr = sc.getSprite(sprite);
    return spr != NULL;
  }

  bool preload_sprite (const std::string& sprite, int sizeX, int sizeY)
  {
    cairo_surface_t* spr = sc.getSprite(sprite, sizeX, sizeY);
    return spr != NULL;
  }

  ///draw the sprite of an object with particular printing offset.
  ///corner is given by (s.left()+off.first.getX(), s.bottom()+off.first.getY())
  ///off.second.getX()-off.first.getX(),off.second.getY()-off.first.getY(), gives the size of the sprite on screen
  // bool drawSprite(const Object& s, const std::string& sprite, const Offset& off, bool resize = false)
  // {
  //   const bool debug = false;
  //   if (sprite == "") {
  //     if (debug)
  // 	Log::log<<"empty sprite, aborting"<<Log::endl;
  //     return false;
  //   }

  //   if ( off.second.getX()-off.first.getX() <= 0) {
  //     if (debug)
  // 	Log::log<<"Size makes no sense. aborting"<<Log::endl;
  //     return false;
  //   } 
  //   if ( off.second.getY()-off.first.getY() <= 0) {
  //     if (debug)
  // 	Log::log<<"Size makes no sense. aborting"<<Log::endl;
  //     return false;
  //   } 
  //   if (debug)
  //     {
  // 	Log::logerr<<"draw object "<<s
  // 		 <<" with sprite "<<sprite
  // 		 <<" with offset "<<off
  // 		 <<" full coordinates ("
  // 		 <<s.left()+off.first.getX()<<", "
  // 		 <<s.bottom()+off.first.getY()<<", "
  // 		 <<off.second.getX()-off.first.getX()<<", "
  // 		 <<off.second.getY()-off.first.getY()<<")"
  // 		 <<Log::endl;
  //     }
  //   cairo_surface_t* spr;
  //   if (resize) {
  //     spr = sc.getSprite(sprite, (off.second.getX()-off.first.getX())*zoomX, (off.second.getY()-off.first.getY())*zoomY);
  //     if (debug)
  // 	if (spr == NULL) {
  // 	  Log::logerr<<"could not get resized sprite"<<Log::endl;
  // 	}
  //   }
  //   else {    
  //     spr = sc.getSprite(sprite);
  //   }
    
  //   if (spr)
  //     {
  // 	blit (cr, spr,
  // 	      s.left()+off.first.getX(), s.bottom()+off.first.getY(),
  // 	      off.second.getX()-off.first.getX(), off.second.getY()-off.first.getY());
  // 	return true;
  //     }
  //   return false;
  // }

  void setColor (Color& c) {
    cairo_set_source_rgb(cr, c.getR()/255., c.getG()/255., c.getB()/255.);
  }

  void displaytext()
  {
    cairo_set_source_rgb(cr,0.,0.,0.);
    cairo_translate(cr, 0, 10);
 
    for (std::list<std::string>::const_iterator it = textbuf.begin();
	 it != textbuf.end();
	 it++)
      {
	//	Log::log<<it->c_str();
	const char* t = it->c_str();
	assert (t != NULL);
	cairo_show_text(cr, t);
	cairo_translate(cr, 0, 10);
	cairo_move_to(cr,0,0);
      }
  }

  void printText(float x, float y, const std::string& s, Color c = Color(0,0,0), int fontsize = 10) {
    cairo_save(cr);

    cairo_set_source_rgb(cr, c.getR()/255., c.getG()/255., c.getB()/255.);
    
    cairo_translate(cr, x, y);

    cairo_scale(cr, 1/zoomX, 1/zoomY);
    
    cairo_set_font_size(cr, fontsize);

    cairo_show_text(cr, s.c_str());

    cairo_stroke(cr);
	
    cairo_restore(cr);
  }

  void show()
  {
    be->flip();
    //    cairo_destroy(cr);
  }

  void printMessage(const std::string & s)
  {
    textbuf.push_back(s);
    while (textbuf.size() > textlimit)
      {
	textbuf.pop_front();
      }
  }

  ///this is a debuging function
  void print_info()
  {
    sc.print_info();
  }

  ~Display()
  {
    be->end();
    if (respforbackend)
      delete be;
  }
};


#endif
