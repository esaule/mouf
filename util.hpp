#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>

bool inline starts_with(const std::string& toCheck,
		 const std::string& prefix) {
  return (toCheck.rfind(prefix, 0) == 0);
}

#endif
