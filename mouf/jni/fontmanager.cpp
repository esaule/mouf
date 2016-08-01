#include "fontmanager.hpp"
#include <string>
#include <iostream>
#include <cairo-ft.h>

#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library*  library = NULL;
FT_Face     face;      /* handle to face object */

FontManager* FontManager::singleton = NULL;

cairo_font_face_t * dafont;

static void init() {
  library = new FT_Library;
  
  int error = FT_Init_FreeType( library );
  if ( error ) {
    std::cerr<<"error in freetype init"<<std::endl;
  }

  
  error = FT_New_Face( *library,
		       //		       "data/font/FreeSans.ttf",
		       "data/font/BloggerSans.ttf",
		       0,
		       &face );
  if ( error ) {
    std::cerr<<"error in freetype face init"<<std::endl;
  }

  
  dafont = cairo_ft_font_face_create_for_ft_face (face, 0);
}

cairo_font_face_t * FontManager::getFont(const std::string& ) {
  if (library == NULL)
    init();

  return dafont;
}
