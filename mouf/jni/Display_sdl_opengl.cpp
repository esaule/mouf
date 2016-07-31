#include "Display_sdl_opengl.hpp"


#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "external/stb_truetype.h"

static stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
static GLuint ftex;
static float fontsize;
//static   stbtt_fontinfo font;


static void my_stbtt_initfont(void)
{
  char* ttf_buffer = new char[1<<20];
  unsigned char *temp_bitmap = new unsigned char[512*512];
  fontsize = 15.0;
  
  //almost direct copy/paste from stb_truetype.h
  fread(ttf_buffer, 1, 1<<20, fopen("data/font/FreeSans.ttf", "rb"));
  stbtt_BakeFontBitmap((unsigned char*)ttf_buffer,0, fontsize,//font size
  		       temp_bitmap,512,512, 32,96, cdata); // no guarantee this fits!
  
  glGenTextures(1, &ftex);
  glBindTexture(GL_TEXTURE_2D, ftex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512,512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, temp_bitmap);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  
  delete[] temp_bitmap;
  delete[] ttf_buffer;

}

static void my_stbtt_print(float x, float y, const char *text, bool flip)
{
   // assume orthographic projection with units = screen pixels, origin at top left
  glEnable(GL_TEXTURE_2D);
  
  float fakex = 0.;
  float fakey = 0.;
  
  glBindTexture(GL_TEXTURE_2D, ftex);
  
  glBegin(GL_QUADS);
  
  while (*text) {
    if (*text >= 32 && *text < 128) {
      stbtt_aligned_quad q;
      stbtt_GetBakedQuad(cdata, 512,512, *text-32, &fakex,&fakey,&q,1);//1=opengl,0=old d3d
      
      //this is different from the code in stb_truetype.h becasue it assumes  0,0 is top left, but it is bottom,left here.
      if (flip)
	{
	  glTexCoord2f(q.s0,q.t1); glVertex2f(x+q.x0,y-q.y1);
	  glTexCoord2f(q.s1,q.t1); glVertex2f(x+q.x1,y-q.y1);
	  glTexCoord2f(q.s1,q.t0); glVertex2f(x+q.x1,y-q.y0);
	  glTexCoord2f(q.s0,q.t0); glVertex2f(x+q.x0,y-q.y0);
	}
      else
	{
	  glTexCoord2f(q.s0,q.t1); glVertex2f(x+q.x0,y+q.y1);
	  glTexCoord2f(q.s1,q.t1); glVertex2f(x+q.x1,y+q.y1);
	  glTexCoord2f(q.s1,q.t0); glVertex2f(x+q.x1,y+q.y0);
	  glTexCoord2f(q.s0,q.t0); glVertex2f(x+q.x0,y+q.y0);
	}
    }
    ++text;
  }
  glEnd();
}


void Display::printText(float x, float y, const std::string & s)
{
  my_stbtt_print(x,y,s.c_str(), flip);
}

void Display::show()
{
  SDL_GL_SwapBuffers();
}

void Display::drawLife(int nblife)
{
  std::stringstream ss;
  ss<<"Life : "<<nblife;
  
  glColor3f(0.,0.,0.);

  printText(10, -60, ss.str());
}

void Display::drawScore(int score)
{
  std::stringstream ss;
  ss<<"Score : "<<score;

  glColor3f(0.,0.,0.);

  my_stbtt_print(10, -20, ss.str().c_str(), flip);
}

void Display::drawControls()
{
  glPushMatrix();
  
  glTranslatef(10.,-280.,0.);
  int offset = 2;
  int line = 0;

  glColor3f(0.,0.,0.);

  printText(0,-((line++)*(fontsize+offset)),"Controls:");
  printText(0,-((line++)*(fontsize+offset)), "Arrows: direction");
  printText(0,-((line++)*(fontsize+offset)), "Space: shoot");
  printText(0,-((line++)*(fontsize+offset)), "S: Speed up");
  printText(0,-((line++)*(fontsize+offset)), "D: Slow down");
  printText(0,-((line++)*(fontsize+offset)), "Q: Quit");
  printText(0,-((line++)*(fontsize+offset)), "E: Engine Mode");

  glPopMatrix();
}

void Display::drawFps(double fps)
{
  std::stringstream ss;
  ss<<"FPS : "<<fps;

  glColor3f(0.,0.,0.);

  printText(10, -40, ss.str().c_str());
}

void Display::displaytext()
{
  glPushMatrix();

  glColor3f(0.,0.,0.);

  
  for (std::list<std::string>::const_iterator it = textbuf.begin();
       it != textbuf.end();
       it++)
    {
      const char* t = it->c_str();
      assert (t != NULL);
      glTranslatef(0.,-12.,0.);

      printText(0,0,t);
    }

  glPopMatrix();
}

bool Display::drawSprite(const Object& s, const std::string& sprite, const Offset& off)
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
  
  smart_ptr<Sprite> spr = sc.getSprite(sprite);
  if (spr)
    {
      blit (spr,
	    s.left()+off.first.getX(), s.bottom()+off.first.getY(),
	    off.second.getX()-off.first.getX(), off.second.getY()-off.first.getY());
      return true;
    }
  return false;
}

bool Display::preload_sprite (const std::string& sprite)
{
  sc.getSprite(sprite);
  //    cairo_surface_t* spr = sc.getSprite(sprite);
  //    return spr != NULL;
  return false;
}

void Display::drawObject(const Object& s,const Color& c)
{
  glDisable(GL_TEXTURE_2D);
  
  glBegin(GL_QUADS);
  glColor4f(c.getR()/255., c.getG()/255., c.getB()/255., 1.);
  
  glVertex3f(s.left(),  s.bottom(), 0.f);
  glVertex3f(s.right(), s.bottom(), 0.f);
  glVertex3f(s.right(), s.top(), 0.f);
  glVertex3f(s.left(),  s.top(), 0.f);
  
  glEnd();
}

void Display::black()
{
  flip = true;

  glEnable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);

  glViewport (0, 0, 450, 600);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  glShadeModel(GL_SMOOTH);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glClear(GL_COLOR_BUFFER_BIT);
	
  //right part of the screen
  {
    glPushMatrix();
    //setting view as pixel coordinates
    glViewport (625, 0, 175, 600);
    glTranslatef(-1., -1., 0.);
    glScalef (2., 2., 1.);
    glScalef (1/175., 1/600., 1.);
	
    //background color
    glBegin(GL_QUADS);
    {
      glColor3f(0., 0., 1.);
      glVertex3f(0.,   0.,   0.);
      glVertex3f(175., 0.,   0.);
      glVertex3f(175., 600., 0.);
      glVertex3f(0.,   600., 0.);
    }
    glEnd();
	
    glTranslatef(0.,600.,0.);

    displaytext();
    glPopMatrix();
  }

  //left part of the screen
  {
    glPushMatrix();
    //setting view as pixel coordinates
    glViewport (0, 0, 175, 600);
    glTranslatef(-1., -1., 0.);
    glScalef (2., 2., 1.);
    glScalef (1/175., 1/600., 1.);
	
    //background color
    glDisable(GL_BLEND);
    glBegin(GL_QUADS);
    {
      glColor3f(0., 0., 1.);
      glVertex3f(0.,   0.,   0.);
      glVertex3f(175., 0.,   0.);
      glVertex3f(175., 600., 0.);
      glVertex3f(0.,   600., 0.);
    }
    glEnd();
    glEnable(GL_BLEND);

    glTranslatef(0.,600.,0.);

    drawScore(score);
    drawControls();
    drawFps(fps);
    drawLife(life);
    glPopMatrix();
  }

  //main part of the screen (setup)
  glViewport (175, 0, 450, 600);

  glMatrixMode (GL_PROJECTION);
  glOrtho (0, 450, sizeY, 0, 0, 1);//orthogonal projection
  glScalef(zoom, zoom, 1.f);//proper zoom level
}

void Display::white()
{
  flip=false;

  glEnable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);

  glViewport (0, 0, 800, 600);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  glShadeModel(GL_SMOOTH);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glClear(GL_COLOR_BUFFER_BIT);

  glViewport (0, 0, 800, 600);
	
  glMatrixMode (GL_PROJECTION);
  glOrtho (0, 800, 600, 0, 0, 1);//orthogonal projection

  // glTranslatef(-1., -1., 0.);
  // glScalef (2., 2., 1.);
  // glScalef (1/175., 1/600., 1.);
  // glTranslatef(0.,600.,0.);
}


void Display::blit (smart_ptr<Sprite> spr,
		    double x, double y, 
		    double width, double height)
{
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, spr->textureid);

  glBegin(GL_QUADS);

  glColor4f(1.f,1.f,1.f,1.f);
  
  glTexCoord2f(0.0f, 0.0f); glVertex3d(x,y,0.);
  glTexCoord2f(0.0f, 1.0f); glVertex3d(x,y+height,0.);
  glTexCoord2f(1.0f, 1.0f); glVertex3d(x+width,y+height,0.);
  glTexCoord2f(1.0f, 0.0f); glVertex3d(x+width,y,0.);    
  
  glEnd();
}


Display::Display()//450
  :sc("data/sprite/"), sizeX(800), sizeY(600), zoom(0.01), life(0)
{
  if (SDL_Init (SDL_INIT_VIDEO) == -1)
    {
      Log::logerr<<"error in sdl init"<<Log::endl;
    }
  
  SDL_GL_SetAttribute( SDL_GL_RED_SIZE,   8 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE,  8 );
  
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  
  window = SDL_SetVideoMode (sizeX,
			     sizeY,
			     0,
			     SDL_OPENGL | SDL_FULLSCREEN);
  
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  if (window == NULL)
    Log::logerr<<"error in sdl createwindow"<<Log::endl;
  
  textlimit = 59;
  score = 0;

  my_stbtt_initfont();
}
