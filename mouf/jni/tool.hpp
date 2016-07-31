#ifndef __TOOL_H__
#define __TOOL_H__

#include "type.hpp"
#include <math.h>
#include <assert.h>

/* angle in 0.01 degree */
inline Position vectorFromAngle (int angle, int length)
{
  double angleinradian = angle;
  angleinradian /= 100;
  angleinradian = angleinradian / 180. * M_PI;
  int x = cos (angleinradian) * length;
  int y = sin (angleinradian) * length;

  return Position (x,y);
}

//returns the euclidian distance between s and e
inline Coord distance (const Position& s, const Position& e)
{
  double squared = (s.getX()-e.getX())*(s.getX()-e.getX())
    +(s.getY()-e.getY())*(s.getY()-e.getY());
  
  return (Coord)round( sqrt( squared));
}

#endif 
