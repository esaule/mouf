#include "genre.hpp"

#include <sstream>

char genre (const std::string& line) {
  std::stringstream ss (line);

  std::string tok;
  while (ss) {
    ss>>tok;
  }

  if (tok.compare("{{f}}") == 0) {
    return 'f';
  }
  if (tok.compare("{{m}}") == 0) {
    return 'm';
  }
  return '?';
}
