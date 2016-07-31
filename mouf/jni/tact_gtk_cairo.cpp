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

#define GTK
#include <math.h>
#include <gtk/gtk.h>
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "cairo.h"

#include <gtk/gtk.h>
#include "UnixStringFinder.hpp"

//#include "engine/controller.hpp"
#include "exception.hpp"

GdkImage *im;
GtkWidget *imWind;



#include "cairo_menu_selector.hpp"


bool quit;

void
destroy (void)
{
  quit = true;
  gtk_main_quit ();
}

int
main (int argc, char *argv[])
{
  gtk_init (&argc, &argv);
  GtkWidget *window;

  StringFinder::singleton = new UnixStringFinder();
  
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

  g_set_application_name("cairo gtk");

  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC (destroy), NULL);
  gtk_container_border_width (GTK_CONTAINER (window), 10);

  
  gtk_widget_add_events (window, GDK_BUTTON_RELEASE_MASK);
  gtk_widget_add_events (window, GDK_POINTER_MOTION_MASK);

  //  CassisDisplay g;
  CairoMenuSelector g;

  imWind = gtk_label_new("You should not see this message!");

  g_signal_connect (G_OBJECT (imWind), "expose_event",
		    G_CALLBACK (CairoGraphicController::expose), &g);


  g_signal_connect (G_OBJECT (window), "key-press-event",
		    G_CALLBACK (CairoGraphicController::key_press), &g);
  
  g_signal_connect (G_OBJECT (window), "button-press-event",
		    G_CALLBACK (CairoGraphicController::button_press), &g);
  
  g_signal_connect (G_OBJECT (window), "button-release-event",
		    G_CALLBACK (CairoGraphicController::button_release), &g);

  g_signal_connect (G_OBJECT (window), "motion-notify-event",
		    G_CALLBACK (CairoGraphicController::motion_notify), &g);
  
  
  gtk_widget_add_events(window, GDK_VISIBILITY_NOTIFY_MASK);

  g_signal_connect (G_OBJECT (window), "visibility-notify-event",
		    G_CALLBACK (CairoGraphicController::visibility_notify), &g);


  GTK_WIDGET_SET_FLAGS(window, GTK_CAN_FOCUS);
  gtk_widget_add_events (window, GDK_KEY_PRESS_MASK);
  gtk_widget_add_events (window, GDK_BUTTON_PRESS_MASK);



  gtk_widget_show (imWind);

  gtk_container_add(GTK_CONTAINER(window), imWind);
  gtk_widget_show (window);

  //  gtk_main ();
  try {
    quit = false;
    while (!quit)
      {
	gtk_main_iteration_do(false);
	
	int microsecondsinasecond = 1000*1000;
	usleep(microsecondsinasecond/100);
	//usleep(10000);
	gtk_widget_queue_draw (imWind);
      }
  }
  catch (BaseException* be) {
    std::cerr<<"Uncaught BaseException!"<<std::endl;
    std::cerr<<"Info:"<<be->info()<<std::endl;

    std::cerr<<"================Stack trace================"<<std::endl;
    be->print_backtrace();
    std::cerr<<"==========================================="<<std::endl;

    delete be;
  }

  return 0;
}

