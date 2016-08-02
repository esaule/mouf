#include "genre.hpp"
#include "util.hpp"
#include <sstream>
#include <iostream>

char genre (const std::string& line) {
  //todo genre is more complicated than that
  
  std::stringstream ss (line);

  std::string tok;

  char ret = '?';

  std::cerr<<"genre ; "<<line<<std::endl;
  
  while (ss) {
    ss>>tok;

    if (ss) {
    
      bool processf = false;
      bool processm = false;
      
      //template f is described at https://fr.wiktionary.org/wiki/Mod%C3%A8le:f
      if (starts_with(tok, "{{f}}")) {
	processf = true;
      } else if (starts_with(tok, "{{fsing}}")) {
	processf = true;
      } else if (starts_with(tok, "{{fplur}}")) {
	processf = true;
      } else if (starts_with(tok, "{{f|")) {
	processf = true;
      }
      
      //template m is described at https://fr.wiktionary.org/wiki/Mod%C3%A8le:m
      if (starts_with(tok, "{{m}}")) {
	processm = true;
      } else if (starts_with(tok, "{{msing}}")) {
	processm = true;
      } else if (starts_with(tok, "{{mplur}}")) {
	processm = true;
      } else if (starts_with(tok, "{{m|")) {
	processm = true;
      }
      
      if (processf) {
	if (ret == 'm') return '?';
	ret = 'f';
      }
      if (processm) {
	if (ret == 'f') return '?';
	ret = 'm';
      }
      
    }
  }
    return ret;
}
