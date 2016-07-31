#ifndef __COLOR_H__
#define __COLOR_H__

#include "type.hpp"
#include "log.hpp"

class Color
{
  Uint8 r,g,b;
public:
  Color (Uint8 red, Uint8 green, Uint8 blue)
    :r(red), g(green), b(blue)
  {}

  operator Uint32 () const
  {
    return (r<<16)|(g<<8)|b;
  }

  Uint8 getR()const {return r;}
  Uint8 getG()const {return g;}
  Uint8 getB()const {return b;}

};

ostream& operator<< (ostream& out, const Color& c);


#endif
