#ifndef __SMART_PTR_H__
#define __SMART_PTR_H__

#include <assert.h>
#include <iostream>
#include "log.hpp"

///implements ref counting. Use as a pointer.
template <typename T, bool debug=false>
class smart_ptr
{
  int* counter;
  T* obj;
  

private:

  void release()
  {
    if (counter != NULL)
      {
	if (debug)
	  Log::logerr<<"releasing. counter was "<<*counter<<Log::endl;
	assert (*counter > 0);
	(*counter)--;
	if (debug)
	  Log::logerr<<"counter is now "<<*counter<<Log::endl;
	if (*counter == 0)
	  {
	    if (debug)
	      Log::logerr<<"freeing"<<Log::endl;
	    delete obj;
	    delete counter;
	  }
	obj = NULL;
	counter = NULL;
      }
  }
public:
  smart_ptr& operator= (const smart_ptr& p)
  {
    if (debug)
      Log::logerr<<"assign"<<Log::endl;
    release();

    counter = p.counter;
    obj = p.obj;
    assert (counter != NULL);
    (*counter) ++;
  }

  smart_ptr()
  {
    if (debug)
      Log::logerr<<"default cstr"<<Log::endl;
    counter=NULL;
    obj=NULL;
  }

  smart_ptr(const T& o)
  {
    if (debug)
      Log::logerr<<"ref cstr"<<Log::endl;

    counter = new int(1);
    obj = new T (o);
  }

  smart_ptr(T* o)
  {
    if (debug)
      Log::logerr<<"ref cstr"<<Log::endl;

    counter = new int(1);
    obj = o;
  }

  smart_ptr(const smart_ptr& p)
  {
    if (debug)
      Log::logerr<<"copy cstr"<<Log::endl;
    
    counter = p.counter;
    obj = p.obj;
    if (counter != NULL)
      {
	(*counter) ++;
	if (debug)
	  Log::logerr<<"counter is now "<<*counter<<Log::endl;
      }
    else
      {
	if (debug)
	  Log::logerr<<"copying a NULL smart_ptr"<<Log::endl;
      }
  }
  
  T& operator* () const
  {
    assert (obj != NULL);
    return *obj;
  }

  T* operator-> () const
  {
    assert (obj != NULL);
    return obj;
  }

  operator bool() const
  {
    return (obj != NULL);
  }

  ~smart_ptr()
  {
    if (debug)
      Log::logerr<<"dstr"<<Log::endl;
    
    release();
  }
};

#endif
