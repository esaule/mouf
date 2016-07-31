#include "log.hpp"
#include "graphics.hpp"
#include <iostream>

// ostream& operator<< (ostream& out, const Object& o)
// {
//   //  SDL_Rect r = o;
//   //  out<<"("<<r.x<<","<<r.y<<","<<r.w<<","<<r.h<<")";
//   out<<"("<<o.left()<<","<<o.bottom()<<","<<o.width()<<","<<o.height()<<")";
//   return out;
// }

ostream& operator<< (ostream& out, const Color& c)
{
  out<<"("<<(int)c.getR()<<","<<(int)c.getG()<<","<<(int)c.getB()<<")";
  return out;
}
