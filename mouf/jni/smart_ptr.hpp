#ifndef __SMART_PTR_H__
#define __SMART_PTR_H__

#include <assert.h>
#include <iostream>

namespace util {
  
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
	    std::cerr<<"releasing. counter was "<<*counter<<std::endl;
	  assert (*counter > 0);
	  (*counter)--;
	  if (debug)
	    std::cerr<<"counter is now "<<*counter<<std::endl;
	  if (*counter == 0)
	    {
	      if (debug)
		std::cerr<<"freeing"<<std::endl;
	      if (obj) //it could be a smart_ptr to NULL
		delete obj;
	      delete counter;
	    }
	  obj = NULL;
	  counter = NULL;
	}
    }
  public:
    
    smart_ptr(T* o, int* c)
    {
      counter = c;
      obj = o;
      (*counter)++;
    }

    smart_ptr& operator= (const smart_ptr& p)
    {
      if (&p == this)
	return *this;

      if (debug)
	std::cerr<<"assign"<<std::endl;
      release();

      counter = p.counter;
      obj = p.obj;
      //      assert (((counter == NULL) && (obj == NULL)) || ((counter != NULL) && (obj != NULL)));
      if (counter)
	(*counter) ++;

      return *this;
    }
    
    template <class TT>
    operator smart_ptr<TT> () {
      smart_ptr<TT> np (obj, counter);
      return np;
    }

    smart_ptr()
    {
      if (debug)
	std::cerr<<"default cstr"<<std::endl;
      counter=NULL;
      obj=NULL;
    }

    smart_ptr(const T& o)
    {
      if (debug)
	std::cerr<<"ref cstr"<<std::endl;

      counter = new int(1);
      obj = new T (o);
    }

    smart_ptr(T* o)
    {
      if (debug)
	std::cerr<<"ref cstr"<<std::endl;

      counter = new int(1);
      obj = o;
    }

    smart_ptr(const smart_ptr& p)
    {
      if (debug)
	std::cerr<<"copy cstr"<<std::endl;
    
      counter = p.counter;
      obj = p.obj;
      if (counter != NULL)
	{
	  (*counter) ++;
	  if (debug)
	    std::cerr<<"counter is now "<<*counter<<std::endl;
	}
      else
	{
	  if (debug)
	    std::cerr<<"copying a NULL smart_ptr"<<std::endl;
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
	std::cerr<<"dstr"<<std::endl;
    
      release();
    }

    bool operator== (const smart_ptr & p) const{
      return obj == p.obj;
    }

    bool operator< (const smart_ptr& p) const{
      return obj < p.obj;
    }

    bool operator<= (const smart_ptr& p) const{
      return this->operator<(p) || this->operator==(p);
    }
    
    bool operator> (const smart_ptr& p) const{
      return !this->operator<=(p);
    }

    bool operator>= (const smart_ptr& p) const{
      return !this->operator<(p);
    }

    explicit operator T*() {
      return this->obj;
    }
  };
}

template<typename T>
bool operator== (const util::smart_ptr<T>& a, const util::smart_ptr<T>& b) {
  return a->operator==(b);
}

#endif
