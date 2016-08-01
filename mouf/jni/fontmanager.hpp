#ifndef FONTMANAG_HPP
#define FONTMANAG_HPP

#include <cairo.h>
#include <string>

class FontManager {
public:
  cairo_font_face_t * getFont(const std::string & fontname);

  static FontManager* singleton;
};

#endif
