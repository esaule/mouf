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

#include <android_native_app_glue.h>

#include <stdbool.h>
#include <errno.h>
#include <jni.h>
#include <sys/time.h>
#include <time.h>
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "cairo/src/cairo.h"

#include "config.h"

//#include "cassis_cairo.hpp"
#include "cairo_graphic_controller.hpp"
#include "cairo_menu_selector.hpp"
#include "SpriteCollection_cairo.hpp"

#include "log.hpp"

#include <iostream>
#include <fstream>

#include "asset_ifstream.hpp"

#include "StringFinder.hpp"
#include "AndroidStringFinder.hpp"

bool app_has_focus = false;
int  tick          = 0;

class CairoGraphicController;

static void draw_frame(ANativeWindow_Buffer *buffer, CairoGraphicController* cd);
static void handle_app_command(struct android_app* app, int32_t cmd);
static int32_t handle_input(struct android_app* app, AInputEvent* event);



static bool pressed;

CairoGraphicController* cd;

#include "myjni.hpp"


void android_main(struct android_app* app) {
  StringFinder::singleton = new AndroidStringFinder(app);

  app_dummy();  // Make sure glue isn't stripped.

  pressed = false;
  cd = NULL;

  app->userData = NULL;
  app->onAppCmd = handle_app_command;
  app->onInputEvent = handle_input;

  if (app->activity->internalDataPath)
    configurationroot = app->activity->internalDataPath;
  else {
    Log::log<<"internal path is NULL. Defaulting to JNI"<<Log::endl;
    JNIEnv *jni;
    app->activity->vm->AttachCurrentThread(&jni, NULL);

    jclass cls_Env = jni->FindClass("android/app/NativeActivity");
    jmethodID mid_getExtStorage = jni->GetMethodID(cls_Env, "getFilesDir","()Ljava/io/File;");
    jobject obj_File = jni->CallObjectMethod( app->activity->clazz, mid_getExtStorage);
    jclass cls_File = jni->FindClass("java/io/File");
    jmethodID mid_getPath = jni->GetMethodID(cls_File, "getPath","()Ljava/lang/String;");
    jstring obj_Path = (jstring) jni->CallObjectMethod(obj_File, mid_getPath);
    const char* path = jni->GetStringUTFChars(obj_Path, NULL);
    jni->ReleaseStringUTFChars(obj_Path, path);
    app->activity->vm->DetachCurrentThread();

    configurationroot = path;
  }

  //LOGI("asset: %p", app->activity->assetManager);
  SpriteCollection::am = app->activity->assetManager;
  AssetIfstream::am = app->activity->assetManager;

  //CassisDisplay cd2(0);
  CairoMenuSelector cms;
  cd = &cms;

  Log::log<<COPYRIGHTTEXT;
  Log::log.commit();

  Log::log<<"internal data path is: "<<configurationroot<<Log::endl;

  if (app->savedState != NULL) {
    Log::log<<"We are starting with a previous saved state; restore from it."<<Log::endl;
    // try
    // 	{
    //cms.deserialize((char*)app->savedState);
    // 	}
    // catch (DeserializeException de)
    // 	{
    // 	  LOGE("Can not deserialize state! Quitting");
    // 	  return;
    // 	}
    Log::log<<"No Code!"<<Log::endl;
  }

  //  StringFinder sf(app);
  //  Log::log<<StringFinder::singleton->find("app_name")<<Log::endl;
  //Log::log<<StringFinder::singleton->find("main_menu_play")<<Log::endl;



  while (1) {
    // Read all pending events. If app_has_focus is true, then we are going 
    // to read any events that are ready then render the screen. If we don't
    // have focus, we are going to block and spin around the poll loop until
    // we get focus again, preventing us from doing a bunch of rendering 
    // when the app isn't even visible.
    int ident;
    int events;
    struct android_poll_source* source;

    while ((ident=ALooper_pollAll(app_has_focus ? 0 : -1, NULL, &events, (void**)&source)) >= 0) {
      // Process this event.
      if (source != NULL) {
	source->process(app, source);
      }

      // Check if we are exiting.
      if (app->destroyRequested != 0) {
	LOGI("Engine thread destroy requested!");
	return;
      }
    }

    //    usleep(100);
    
    /* Now that we've delt with input, draw stuff */
    if (app->window != NULL) {
      ++tick;
      //            LOGI("Rendering frame %d", tick);
      ANativeWindow_Buffer buffer;
      if (ANativeWindow_lock(app->window, &buffer, NULL) < 0) {
	LOGW("Unable to lock window buffer");
	continue;
      }
      //Log::log<<util::timestamp()<<Log::endl;
      draw_frame(&buffer, cd);

      ANativeWindow_unlockAndPost(app->window);
    }
  }
}


static void handle_app_command(struct android_app* app, int32_t cmd) {
  /* app->userData is available here */
  
  char * buf = NULL;


  switch (cmd) {
  case APP_CMD_SAVE_STATE:
    //To save the state: write the size of the save in
    //app->savedStateSize and point to a buffer of that size with
    //app->savedState. Saving the state is important because all
    //change of application cause a save. Changing orientation causes
    //a save as well.
    //Log::log<<"Save State!"<<Log::endl;
    //app->savedStateSize = cd->serializesize();
    //buf = (char*) malloc(app->savedStateSize);
    //cd->serialize((char*)buf);
    //app->savedState = (void*)buf;
    break;

  case APP_CMD_CONTENT_RECT_CHANGED:
    Log::log<<"Content size changed to: "<<app->contentRect.left<<" "<<app->contentRect.right<<" "<<app->contentRect.top<<" "<<app->contentRect.bottom<<Log::endl;
    break;
  case APP_CMD_INIT_WINDOW:
    app_has_focus=true;
    break;
  case APP_CMD_LOST_FOCUS:
    Log::log<<"lose focus"<<Log::endl;
    app_has_focus=false;
    break;
  case APP_CMD_GAINED_FOCUS:
    Log::log<<"gain focus"<<Log::endl;
    app_has_focus=true;
    break;
  }
}


static int32_t handle_input(struct android_app* app, AInputEvent* event) {
  /* app->userData is available here */

  if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
    app_has_focus = true;

    //	LOGI("Motion Event, pressed = %d", (pressed?1:0));
	
    int nbevent = AMotionEvent_getPointerCount(event);
    if (nbevent > 1)
      Log::log<<nbevent<<" events"<<Log::endl;

    for (int i=0; i< nbevent; ++i)
      {
	int action = AMotionEvent_getAction(event) & AMOTION_EVENT_ACTION_MASK ;
	//  LOGI("Motion Event, action = %d", action);

	//	Log::log<<"event :"<<action<<Log::endl;

	float px = AMotionEvent_getX(event, i);
	float py = AMotionEvent_getY(event, i);
	    
	if ((action == AMOTION_EVENT_ACTION_DOWN)
	    ||(action == AMOTION_EVENT_ACTION_POINTER_DOWN))
	  {
	    cd->clickat(px,py);
	  }

	if ((action == AMOTION_EVENT_ACTION_MOVE))
	  {
	    cd->clickmove(px,py);
	  }	

	if ((action == AMOTION_EVENT_ACTION_UP) || 
	    (action == AMOTION_EVENT_ACTION_POINTER_UP) ||
	    (action == AMOTION_EVENT_ACTION_CANCEL) ||
	    (action == AMOTION_EVENT_ACTION_OUTSIDE)  )
	  {
	    cd->clickrelease(px, py);
	  }

      }
    return 1;
  } else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {
    LOGI("Key event: action=%d keyCode=%d metaState=0x%x",
	 AKeyEvent_getAction(event),
	 AKeyEvent_getKeyCode(event),
	 AKeyEvent_getMetaState(event));
  }

  return 0;
}



static void draw_frame(ANativeWindow_Buffer *buffer, CairoGraphicController* cd)
{
  const bool debug=false;
  if (debug)
    Log::log<<"draw"<<Log::endl;
  int pixel_size = 0;
  cairo_surface_t *surface = NULL;

  /* Setup our cairo surface to render directly to the native window buffer */
  if (buffer->format == WINDOW_FORMAT_RGB_565) {
    pixel_size = 2;
    surface = cairo_image_surface_create_for_data((unsigned char*)buffer->bits, CAIRO_FORMAT_RGB16_565,
						  buffer->width, buffer->height, buffer->stride*pixel_size);

    SpriteCollection::sc.setFormat(CAIRO_FORMAT_RGB16_565);
  } else if (buffer->format == WINDOW_FORMAT_RGBA_8888 || buffer->format == WINDOW_FORMAT_RGBX_8888) {
    pixel_size = 4;
    surface = cairo_image_surface_create_for_data((unsigned char*)buffer->bits, CAIRO_FORMAT_RGB24,
						  buffer->width, buffer->height, buffer->stride*pixel_size);


    SpriteCollection::sc.setFormat(CAIRO_FORMAT_RGB24);
  } else {
    LOGE("Unsupported buffer format: %d", buffer->format);
    return;
  }

  if (surface == NULL) {
    Log::log<<"can not acquire surface! quitting"<<Log::endl;
    exit(-1);
  }
  //  Log::log<<"surface acquired !"<<Log::endl;


  cairo_t         *cr      = cairo_create(surface);

  /* clear the screen */
  memset(buffer->bits, 0, buffer->stride*pixel_size*buffer->height);

  /* Normalize our canvas size to make our lives easier */
  //  cairo_scale(cr, buffer->width, buffer->height);


  cd->setSizeX (buffer->width);
  cd->setSizeY (buffer->height);
    
  cd->render(cr);

  cairo_destroy(cr);
  cairo_surface_destroy(surface);

  //in little endian cairo and android do not have the same definition
  //of RGB. Cairo uses a word base definition. android uses an
  //address based definition
#if _BYTE_ORDER == _LITTLE_ENDIAN
  if (buffer->format == WINDOW_FORMAT_RGBA_8888 ||
      buffer->format == WINDOW_FORMAT_RGBX_8888
      )
    {
      for (char * p = (char*)buffer->bits; p != (char*)buffer->bits+(4*buffer->stride*buffer->height); p+=4) {
	char x = p[0];
	p[0] = p[2];
	p[2] = x;
      }
    }
#endif
  if (debug)
    Log::log<<"end draw"<<Log::endl;
}
