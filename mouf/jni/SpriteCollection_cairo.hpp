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

#ifndef __SPRITE_COLECTION_H__
#define __SPRITE_COLECTION_H__

#include "cairo.h"
#include "log.hpp"

#include <assert.h>

#ifndef NOSTL
#include <map>
#else
#include "mymap.hpp"
#endif

#ifdef NOSTL
#include "mystring.hpp"
#else
#include <string>
#endif

#ifdef ANDROID
#include <android/asset_manager.h>
#include <android/log.h>

cairo_status_t android_png_cairo_wrapper_read (void *closure,
					       unsigned char *data,
					       unsigned int length);

#endif



class SpriteCollection
{


#ifdef NOSTL
  typedef MyString string;
#else
  typedef std::string string;
#endif

  struct sprite_id
  {
    string name;
    int sizeX;
    int sizeY;

    sprite_id()
      :name(""), sizeX(0), sizeY(0)
    {
    }

    sprite_id(const sprite_id& s)
      :name(s.name), sizeX(s.sizeX), sizeY(s.sizeY)
    {
    }

    sprite_id(string n, int sx, int sy)
      :name(n), sizeX(sx), sizeY(sy)
    {}

    bool operator== (const sprite_id& s)const
    {
      return (s.name == name && s.sizeX == sizeX && s.sizeY==sizeY);
    }

    bool operator!= (const sprite_id& s)const
    {
      return ! this->operator==(s);
    }

    bool operator< (const sprite_id& s) const
    {
      if (sizeX < s.sizeX) return true;
      if (sizeX > s.sizeX) return false;
      assert (sizeX == s.sizeX);
      if (sizeY < s.sizeY) return true;
      if (sizeY > s.sizeY) return false;
      assert (sizeY == s.sizeY);
      return name < s.name;
    }
  };

  cairo_format_t main_format;

#ifndef NOSTL
  std::map<string, cairo_surface_t*> storage;
  std::map<sprite_id, cairo_surface_t*> storage_scaled;
#else
  mymap<string, cairo_surface_t*> storage;
  mymap<sprite_id, cairo_surface_t*> storage_scaled;
#endif

  const string path;

#ifdef ANDROID
  cairo_surface_t* android_cairo_image_surface_create_from_png(const char* path)
  {
    const bool debug = false;

    if (debug)
      Log::log<<"SpriteCollection. trying to load from (asset) PNG: "<<path<<Log::endl;

    if (am == NULL)
      {
	if (debug)
	  Log::log<<"SpriteCollection. No asset manager. aborting! "<<Log::endl;
	return NULL;
      }

    AAsset* as = AAssetManager_open (am, path, AASSET_MODE_STREAMING);

    if (as == NULL)
      {
	if (debug)
	  Log::log<<"SpriteCollection. as is NULL"<<Log::endl;

	return NULL;
      }

    if (debug)
      Log::log<<"SpriteCollection. stream found: "<<path<<Log::endl;

    cairo_surface_t* sur = cairo_image_surface_create_from_png_stream (android_png_cairo_wrapper_read, as);

    AAsset_close(as);
    
    if (sur == NULL)
      {
	if (debug)
	  Log::log<<"can not make a surface out of the data."<<Log::endl;
	
	return NULL;
      }

    if (cairo_surface_status(sur) == CAIRO_STATUS_NO_MEMORY ||
	cairo_surface_status(sur) == CAIRO_STATUS_READ_ERROR)
      {
	cairo_surface_destroy(sur);
	if (debug)
	  Log::log<<"incorrect surface"<<Log::endl;
	return NULL;
      }

    if (debug)
      Log::log<<"SpriteCollection. android PNG succesfully read: "<<path<<Log::endl;

    return sur;
  }  
#endif

  ///clear the cache from all data.
  void clear_all() {
    for (auto it = storage.begin(); it!= storage.end(); ++it) {
      cairo_surface_destroy( it->second );
    }
    storage.clear();
    for (auto it = storage_scaled.begin(); it!= storage_scaled.end(); ++it) {
      cairo_surface_destroy( it->second );
    }
    storage_scaled.clear();
  }

  bool load(const string& name)
  {
    //LOGI("load(\"%s\")", name.c_str());

    const bool debug = true;
    if (debug)
      Log::log<<"SpriteCollection. loading: "<<name<<Log::endl;
    string s=path;
    s += "/";
    s += name;

    cairo_surface_t* sp = NULL;

#ifdef CAIRO_HAS_PNG_FUNCTIONS
    if (sp == NULL) 
      {
	sp = cairo_image_surface_create_from_png(s.c_str());

	switch (cairo_surface_status(sp))
	  {
	  case CAIRO_STATUS_NO_MEMORY :
	  case CAIRO_STATUS_FILE_NOT_FOUND :
	  case CAIRO_STATUS_READ_ERROR :
	    if (debug)
	      Log::logerr<<"can not load image (PNG FS) \""<<s<<"\""<<Log::endl;
	    cairo_surface_destroy(sp);
	    sp = NULL;
	  }
      }
#endif
    
#ifdef ANDROID
    if (sp == NULL)
      {
	sp = android_cairo_image_surface_create_from_png(s.c_str());

	if (sp != NULL) {
	  switch (cairo_surface_status(sp))
	    {
	    case CAIRO_STATUS_NO_MEMORY :
	    case CAIRO_STATUS_FILE_NOT_FOUND :
	    case CAIRO_STATUS_READ_ERROR :
	      Log::logerr<<"can not load image (androidPNGAsset) \""<<s<<"\""<<Log::endl;
	      cairo_surface_destroy(sp);
	      sp = NULL;
	    }
	}
	else {
	  if (debug) 
	    Log::log<<"Could not find in the assets \""<<s<<"\""<<Log::endl;
	}
      }
#endif
    
    if (sp == NULL) {
      if (debug)
	Log::log<<"SpriteCollection. could not load: "<<name<<Log::endl;

      return false;
    }

    switch (cairo_surface_status(sp))
      {
      case CAIRO_STATUS_NO_MEMORY :
      case CAIRO_STATUS_FILE_NOT_FOUND :
      case CAIRO_STATUS_READ_ERROR :
	if (debug)
	  Log::log<<"SpriteCollection. could not load: "<<name<<Log::endl;
	cairo_surface_destroy(sp);
	return false;
	break;
      default:
	if (debug)
	  Log::log<<"SpriteCollection. loaded: "<<name<<Log::endl;
      
	storage[name] = sp;
	return true;
	break;
      }
  }
  
public:


#ifdef ANDROID
  static AAssetManager* am;
#endif

  SpriteCollection (const char* p)
    :path(p)
  {
    main_format = CAIRO_FORMAT_INVALID;
  }

  void print_info()
  {
    //    Log::logerr<<"Sprite Collection. size="<<storage.size()<<Log::endl;
  }

  ///gets a surface that for the sprite name.
  ///the size of the sprite is the native size.
  cairo_surface_t* getSprite(const string& name)
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
	  {
	    return NULL;
	  }
      }
  }

  ///gets a surface for the sprite name that is resized to sizeX,
  ///sizeY. (should get a fast blit for that size.)
  cairo_surface_t* getSprite(const string& name, int sizeX, int sizeY)
  {
    sprite_id s(name, sizeX, sizeY);
    if (storage_scaled.count(s) != 0)
      {
	assert (storage_scaled.count(s) == 1);
	return storage_scaled[s];
      }
    else
      {
	Log::logerr<<"loading sprite \""<<name<<"\" with size "<<sizeX<<" "<<sizeY<<Log::endl;
	cairo_surface_t* base = getSprite(name);
	if (main_format == CAIRO_FORMAT_INVALID)
	  Log::logerr<<"main format hasn't been set"<<Log::endl;

	cairo_surface_t* out = cairo_image_surface_create(main_format, sizeX, sizeY);
	//TODO: check validity
	if (out == NULL)
	  Log::logerr<<"Sprite scaled invalid"<<Log::endl;
	else
	  switch (cairo_surface_status (out)) {
	  case CAIRO_STATUS_SUCCESS:
	    break;
	  case CAIRO_STATUS_NULL_POINTER:
	  case CAIRO_STATUS_NO_MEMORY:
	  case CAIRO_STATUS_READ_ERROR:
	  case CAIRO_STATUS_INVALID_CONTENT:
	  case CAIRO_STATUS_INVALID_FORMAT:
	  case CAIRO_STATUS_INVALID_VISUAL:
	  default:
	    Log::logerr<<"invalid surface: (reason: "<<cairo_status_to_string(cairo_surface_status(out))<<" )"<<Log::endl;
	    cairo_surface_destroy (out);
	    out = NULL;
	  }
	
	if (out != NULL){
	  cairo_t * cr = cairo_create(out);
	
	  cairo_rectangle (cr,0,0,sizeX, sizeY);
	  
	  cairo_scale(cr,
		      (double)(sizeX)/cairo_image_surface_get_width(base),
		      (double)(sizeY)/cairo_image_surface_get_height(base));
	  
	  cairo_set_source_surface(cr, base, 0, 0);
	  cairo_fill(cr);
	  
	  storage_scaled[s] = out;
	}
	return out;
      }
    
  }
 
  //all subsequent loads will be made in this format
  void setFormat(cairo_format_t f)
  {
    if (f != main_format)
      {
	Log::log<<"format changed"<<Log::endl;
	clear_all();
	main_format = f;
      }
  }
  
  cairo_format_t getFormat() const {
    return main_format;
  }

  static SpriteCollection sc;
};


#endif
