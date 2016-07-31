#include "type.hpp"

Position operator+ (const Position& a, const Position& b)
{
  return Position(a.getX()+b.getX(), a.getY()+b.getY());
}

Position& operator+= ( Position& a, const Position& b)
{
  a.setX(a.getX()+b.getX());
  a.setY(a.getY()+b.getY());

  return a;
}


Position operator- (const Position& a, const Position& b)
{
  return Position(a.getX()-b.getX(), a.getY()-b.getY());
}

Position operator* (const Position& a, const double& b)
{
  return Position(b*a.getX(), b*a.getY());
}

