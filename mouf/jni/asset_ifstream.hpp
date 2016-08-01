#ifndef ASSET_IFSTREAM_H
#define ASSET_IFSTREAM_H

#include <string>

#include <android/asset_manager.h>
#include "log.hpp"

class AssetIfstream {
public:
  static AAssetManager* am;
private:
  AAsset* as;
  bool debug;
  bool error;
  const int length;
  char* buffer;
  int size;

  ///add the buffer to the string passed as a parameter.
  ///returns true if more processing necessary.
  bool processBuf (std::string& s) {
    int found = 0;
    for (; found < size; ++found) {
      if (buffer[found] == '\r') {
	buffer[found] = '\0';
      }
    
      if (buffer[found] == '\n') {
	buffer[found] = '\0';
	std::string temps(buffer);
	s += temps;
	
	memmove (buffer, buffer + found+1, size - found);
	size -= found+1;

	return false;
      }
    }
    std::string temps(buffer);
    s += temps;
	    
    buffer[0] = '\0';
    size = 0;
    return true;
  }

public:
  AssetIfstream (const std::string& filename)
    :length(512), buffer(new char[length+1]) {
    buffer[0] = '\0';
    size = 0;
    debug = true;
    error = false;
    as = NULL;

    if (am == NULL)
      {
	if (debug)
	  Log::log<<"AssetIfstream. No asset manager. aborting! "<<Log::endl;
	error = true;
	return;
      }

    as = AAssetManager_open (am, filename.c_str(), AASSET_MODE_STREAMING);

    if (as == NULL)
      {
	if (debug)
	  Log::log<<"can not load asset \""<<filename<<"\" aborting! "<<Log::endl;
	error = true;
	return;
      }

  }
  
  bool is_open() const {return as != NULL;}

  void getline(std::string & s) {
    s.clear();
    if (processBuf(s)) {
      int res;
      while (res = AAsset_read(as, buffer, length)) {
	if (res < 0) {
	  error = true;
	  return;
	}

	buffer[res] = '\0';
	size = res;

	if (!processBuf(s)) {
	  return;
	}
	
      }
      
      //EOF
      error = true;
    }
  }
  
  bool good()const {return !error;}

  operator bool ()const {return good();}

  bool eof() const {return error;}
  bool bad() const {return error;}
  bool fail() const {return error;}

  void close()
  {
    if (as) {
      AAsset_close(as);
      as = NULL;
    }
  }

  ~AssetIfstream() {
    close();
    delete[] buffer;
  }

};




void getline (AssetIfstream& ai, std::string& s); 

#endif
