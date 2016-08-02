#ifndef UTIL_H
#define UTIL_H

#include <string>

static bool starts_with(const std::string& toCheck,
		 const std::string& prefix) {
  return (toCheck.rfind(prefix, 0) == 0);
}


static std::string trim(const std::string& str,
                 const std::string& whitespace = " ")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}


#endif
