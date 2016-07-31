#ifndef __SPRITE_COLECTION_H__
#define __SPRITE_COLECTION_H__

#include <assert.h>
#include <map>
#include "smart_ptr.hpp"
#include <GL/gl.h>


typedef unsigned char stbi_uc;

extern "C" {extern stbi_uc* stbi_load(char const*, int*, int*, int*, int);}
#define stbi_image_free free

struct Sprite
{
  int x, y;
  int nbcomp;
  stbi_uc* data;

  GLuint textureid;
  
  Sprite(const std::string& filename)
  {
    data = stbi_load(filename.c_str(), &x, &y, &nbcomp, 0);
    Log::log<<"loading \""<<filename<<"\" (nbcomp = "<<nbcomp<<")"<<Log::endl;

    if (data == NULL) return;
    
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &textureid);
    glBindTexture(GL_TEXTURE_2D, textureid);

    if  (nbcomp == 4)
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else
      {
	assert (nbcomp == 3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      }
    
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);    
  }

  ~Sprite()
  {
    if (data)
      {
	stbi_image_free(data);
      }
  }
};

class SpriteCollection
{
  std::map<std::string, smart_ptr<Sprite> > storage;

  const char* path;

  bool load(const std::string& name)
  {
    const bool debug = false;
    if (debug)
      Log::logerr<<"SpriteCollection. loading: "<<name<<Log::endl;
    std::string s=path;
    s += "/";
    s += name;

    smart_ptr<Sprite> sp = new Sprite(s);
    if (sp->data)
      {
	storage[name] = sp;
	return true;
      }
    return false;
  }
  
public:
  SpriteCollection (const char* p)
    :path(p)
  {
  }

  void print_info()
  {
    Log::logerr<<"Sprite Collection. size="<<storage.size()<<Log::endl;
  }

  smart_ptr<Sprite> getSprite(const std::string& name)
  {
    if (storage.count(name) != 0)
      {
	assert (storage.count(name) == 1);
	return storage[name];
      }
    else
      {
	if (load(name))
	  return storage[name];
	else
	  return NULL;
      }
  }
  
};


#endif
