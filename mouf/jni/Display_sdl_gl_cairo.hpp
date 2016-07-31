#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <sstream>

#include "SDL.h"
#include "SDL_opengl.h"

#define GL_TEXTURE_RECTANGLE_ARB 0x84F5

class Display
{
  SDL_Surface* window;

  SpriteCollection sc;

  const int sizeX, sizeY;
  const float zoom;

  cairo_surface_t* surf;
  unsigned char* surf_data;

  int nblife;

  cairo_t* cr;

  int textlimit;
  std::list<std::string> textbuf;

  int score;
  unsigned int texture_id;

  void init_gl()
  {
    Log::logerr<<"OpenGL version: "<< glGetString (GL_VERSION)<<Log::endl;
    Log::logerr<<"OpenGL vendor: "<<glGetString (GL_VENDOR)<<Log::endl;
    Log::logerr<<"OpenGL renderer: "<< glGetString (GL_RENDERER);
    glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
    glDisable (GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable (GL_TEXTURE_RECTANGLE_ARB);

  }


  void
  resize_func (int           width,
	       int           height,
	       unsigned int* texture_id)
  {
    glViewport (0, 0, width, height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
    glClear (GL_COLOR_BUFFER_BIT);
    glDeleteTextures (1, texture_id);
    glGenTextures (1, texture_id);
    glBindTexture (GL_TEXTURE_RECTANGLE_ARB, *texture_id);
    glTexImage2D (GL_TEXTURE_RECTANGLE_ARB,
		  0,
		  GL_RGBA,
		  width,
		  height,
		  0,
		  GL_BGRA,
		  GL_UNSIGNED_BYTE,
		  NULL);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}



  cairo_t*
  create_cairo_context (int               width,
			int               height,
			int               channels,
			cairo_surface_t** surf,
			unsigned char**   buffer)
  {
    cairo_t* cr;
    
    // create cairo-surface/context to act as OpenGL-texture source
    *buffer = new unsigned char [channels * width * height];
    if (!*buffer)
      {
	printf ("create_cairo_context() - Couldn't allocate buffer\n");
	return NULL;
      }
    
    *surf = cairo_image_surface_create_for_data (*buffer,
						 CAIRO_FORMAT_ARGB32,
						 width,
						 height,
						 channels * width);
    
    if (cairo_surface_status (*surf) != CAIRO_STATUS_SUCCESS)
      {
	//		free (*buffer);
	delete[] *buffer;
	printf ("create_cairo_context() - Couldn't create surface\n");
	return NULL;
      }
    
    cr = cairo_create (*surf);
    
    if (cairo_status (cr) != CAIRO_STATUS_SUCCESS)
      {
	//free (*buffer);
	delete[] *buffer;
		printf ("create_cairo_context() - Couldn't create context\n");
		return NULL;
      }
    return cr;
  }


 	

  void
  draw_func (int            width,
	     int            height,
	     unsigned char* surf_data,
	     unsigned int   texture_id)
  {
    if (!surf_data)
      {
	printf ("draw_func() - No valid pointer to surface-data passed\n");
	return;
      }

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    glClear (GL_COLOR_BUFFER_BIT);
    glPushMatrix ();
    glBindTexture (GL_TEXTURE_RECTANGLE_ARB, texture_id);
    glTexImage2D (GL_TEXTURE_RECTANGLE_ARB,
		  0,
		  GL_RGBA,
		  width,
		  height,
		  0,
		  GL_BGRA,
		  GL_UNSIGNED_BYTE,
		  surf_data);
    glColor3f (0.25f, 0.5f, 1.0f);
    
    glBegin (GL_QUADS);
    glTexCoord2f (0.0f, 0.0f);
    glVertex2f (0.0f, 1.0f);
    glTexCoord2f ((GLfloat) width, 0.0f);
    glVertex2f (1.0f, 1.0f);
    glTexCoord2f ((GLfloat) width, (GLfloat) height);
    glVertex2f (1.0f, 0.0f);
    glTexCoord2f (0.0f, (GLfloat) height);
    glVertex2f (0.0f, 0.0f);
    glEnd ();
    glPopMatrix ();
    SDL_GL_SwapBuffers();
  }




public:
  Display()//450
    :sc("data/sprite/"), sizeX(800), sizeY(600), zoom(0.1), life(0)
  {
    //    debug_message("Display()");
    Log::logerr<<"Display()"<<Log::endl;
    if (SDL_Init (SDL_INIT_VIDEO) == -1)
      {
	Log::logerr<<"error in sdl init"<<Log::endl;
      }


    SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
    window = SDL_SetVideoMode (sizeX,
			       sizeY,
			       0,
			       SDL_OPENGL | SDL_FULLSCREEN);
    
    // did we get what we want?
    if (!window)
      {
	printf ("Couldn't open SDL-window: %s\n", SDL_GetError ());
	exit (-2);
      }
    
    //    debug_message("SDL-windoes opened");
    Log::logerr<<"SDL-windows opened"<<Log::endl;

    // create cairo-surface/context to act as OpenGL-texture source
    cr = create_cairo_context (sizeX, sizeY, 4, &surf, &surf_data);

    if (cr != NULL)
      Log::logerr<<"cairo context obtained"<<Log::endl;
    else
      Log::logerr<<"can not acquire cairo context"<<Log::endl;

    // setup "GL-context"
    init_gl ();

    Log::logerr<<"gl context initialized"<<Log::endl;

    resize_func (sizeX, sizeY, &texture_id);

    Log::logerr<<"texture_id acquired"<<Log::endl;

    textlimit = 59;
    score = 0;

    Log::logerr<<"Display initialized"<<Log::endl;
  }

  void setScore(int score)
  {
    this->score = score;
  }

  void setNbLife(int nblife)
  {
    this->nblife = nblife;
  }
  
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

  void black()
    {
      //      cr = cairo_create(surf);

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
      drawLife(life);
      cairo_restore(cr);

      //middle part of the screen (setup)
      cairo_save(cr);
      cairo_rectangle(cr, 175, 0, 450, 600);
      cairo_clip(cr);

      cairo_translate(cr, 175,0);

      cairo_set_source_rgb(cr,1.0,1.0,1.0);
      cairo_paint(cr);

      cairo_scale(cr, zoom, zoom);
    }

  void drawObject(const Object& s,const Color& c)
    {
      SDL_Rect r = s; 
      
      cairo_rectangle(cr,r.x, r.y,r.w,r.h);
      cairo_set_source_rgb(cr,c.getR()/255.,c.getG()/255.,c.getB()/255.);
      cairo_fill(cr);
    }

  void drawSprite(const Object& s, const char* sprite, const Offset& off)
  {
    const bool debug = false;
    if (debug)
      {
	Log::logerr<<"draw object "<<s
		 <<" with sprite "<<sprite
		 <<" with offset "<<off
		 <<" full coordinates ("
		 <<s.left()+off.first.getX()<<", "
		 <<s.bottom()+off.first.getY()<<", "
		 <<off.second.getX()-off.first.getX()<<", "
		 <<off.second.getY()-off.first.getY()<<")"
		 <<Log::endl;
      }

     blit (cr, sc.getSprite(sprite),
	   s.left()+off.first.getX(), s.bottom()+off.first.getY(),
	   off.second.getX()-off.first.getX(), off.second.getY()-off.first.getY());
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

  void drawScore(int score)
  {
    cairo_save(cr);

    cairo_set_source_rgb (cr, 0., 0., 0.);

    cairo_move_to(cr, 10, 20);

    cairo_set_font_size(cr, 15);

    std::stringstream ss;
    ss<<"Score : "<<score;

    cairo_show_text (cr, ss.str().c_str());
    
    cairo_restore(cr);
  }

  void drawLife(int nblife)
  {
    cairo_save(cr);

    cairo_set_source_rgb (cr, 0., 0., 0.);

    cairo_move_to(cr, 10, 200);

    cairo_set_font_size(cr, 15);

    std::stringstream ss;
    ss<<"Life : "<<nblife;

    cairo_show_text (cr, ss.str().c_str());
    
    cairo_restore(cr);
  }


  void show()
  {
    draw_func (sizeX, sizeY, surf_data, texture_id);
    cairo_restore(cr);

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
    glDeleteTextures (1, &texture_id);
    delete[] surf_data;
    cairo_destroy(cr);
    SDL_Quit ();
  }
};


#endif
