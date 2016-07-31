#ifndef _INPUT_H__
#define _INPUT_H__

#include <iostream>
#include "type.hpp"

class Input
{
  bool q;
  bool leftdir,rightdir,updir,downdir;
  bool fa,sl;
  bool sh;
  mutable bool engine;

  bool pinfo;

  float px, py;

  Game* g;
public:
  Input()
    :q(false),
    leftdir(false),rightdir(false),updir(false),downdir(false),
     fa(false),sl(false),sh(false),engine(false),pinfo(false),
     px (-1), py(-1), g(NULL)
  {

  }

  void setPointer(float x, float y) {
    px = x;
    py = y;
  }

  void setLeft()
  {
    leftdir = true;
    rightdir = false;
  }


  void setGame(Game* ga) {
    g = ga;
  }

  void setRight()
  {
    leftdir = false;
    rightdir = true;
  }

  void reset()
  {
    q=false;
    leftdir=false;
    rightdir=false;
    updir=false;
    downdir=false;
    fa=false;
    sl=false;
    sh=false;
    engine=false;
    pinfo=false;
    px = -1;
    py = -1;
  }

  void handle()
  {
    fa = false;
    sl = false;

  }

  bool faster()const{return fa;}
  bool slower()const{return sl;}


  bool pause() const{return false;}

  // bool left()const{return leftdir;}
  // bool right()const{return rightdir;}
  bool left()const;
  bool right()const;
  bool down()const{return downdir;}
  bool up()const{return updir;}


  bool quit()const{return q;}

  //  bool shoot()const{return sh;}
  bool shoot()const{return true;}

  bool swapEngineMode() const
  {
    bool e = engine;
    engine = false;
    return e;
  }

  bool print_info()
  {
    if (pinfo)
      {
	pinfo = false;
	return true;
      }
    else
      return false;
  }
};

#endif
