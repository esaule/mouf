#include "genre.hpp"
#include "util.hpp"
#include <sstream>

char genre (const std::string& line) {
  //todo genre is more complicated than that
  
  std::stringstream ss (line);

  std::string tok;

  char ret = '?';

  while (ss) {
    ss>>tok;

    //template f is described at https://fr.wiktionary.org/wiki/Mod%C3%A8le:f
    if (starts_with(tok, "{{f}}")) {
      ret = 'f';
      continue;
    }
    if (starts_with(tok, "{{fsing}}")) {
      ret = 'f';
      continue;
    }
    if (starts_with(tok, "{{fplur}}")) {
      ret = 'f';
      continue;
    }
    if (starts_with(tok, "{{f|")) {
      ret = 'f';
      continue;
    }

    //template m is described at https://fr.wiktionary.org/wiki/Mod%C3%A8le:m
    if (starts_with(tok, "{{m}}")) {
      ret = 'm';
      continue;
    }
    if (starts_with(tok, "{{msing}}")) {
      ret = 'm';
      continue;
    }
    if (starts_with(tok, "{{mplur}}")) {
      ret = 'm';
      continue;
    }

    if (starts_with(tok, "{{m|")) {
      ret = 'm';
      continue;
    }
  }
  return ret;
}
