#ifndef __MY_TYPES_H__
#define __MY_TYPES_H__

#include <utility>
#include <math.h>
#include <iostream>

typedef short Int16;
typedef char Int8;
typedef unsigned char Uint8;
typedef int Int32;
typedef unsigned int Uint32;
typedef int Angle; //in 0.01 degree
typedef Int32 Coord;


class Display;
class Game;
class Entity;
class Input;
class MovingPattern;
class ShootingPattern;
class Condition;
class Animation;

class Position
{
  Coord x, y;
public:
  Position(const Position& p)
    :x(p.x), y(p.y)
  {    
  }

  Position(Coord xp, Coord yp)
    :x(xp), y(yp)
  {    
  }
  Coord getX()const{return x;}
  Coord getY()const{return y;}
  Coord setX(Coord nx){ x = nx;}
  Coord setY(Coord ny){ y = ny;}
  double getNormSquare()const{return x*x+y*y;}

  ///seeing the position as a vector, this function scales the vector so that it has a given norm.
  void setNorm(Coord n)
  {
    double norm = sqrt(getNormSquare());
    double ratio = n/norm;
    x *= ratio;
    y *= ratio;
  }
};

Position operator+ (const Position& a, const Position& b);
Position operator- (const Position& a, const Position& b);
Position operator* (const Position& a, const double& t);
Position& operator+= (Position& a, const Position& b);

class Shape
{
  Coord w, h;
 public:
  Shape(Coord width, Coord height)
    :w(width), h(height)
    {}
  Coord getWidth () const{return w;}
  Coord getHeight () const{return h;}
};

template <class ostream>
ostream& operator<< (ostream& out, const Position& p)
{
  out<<"("<<p.getX()<<","<<p.getY()<<")";
  return out;
}

template<class ostream>
ostream& operator<< (ostream& out, const Shape& s)
{
  out<<"("<<s.getWidth()<<","<<s.getHeight()<<")";
  return out;
}

typedef std::pair<Position,Position> Offset;

template<class ostream>
ostream& operator<< (ostream& out, const Offset& s)
{
  out<<"("<<s.first<<","<<s.second<<")";
  return out;
}

#endif
