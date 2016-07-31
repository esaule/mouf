#ifndef UTLI__BACKTRACE
#define UTLI__BACKTRACE

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>

#include <sstream>

inline void string_trim_front(std::string & s)
{
	s.erase(0, s.find_first_not_of(" \t\n"));
}

inline void string_trim_rear(std::string & s)
{
	s.erase(s.find_last_not_of(" \t\n") + 1);
}

inline void string_trim(std::string & s)
{
	string_trim_front(s);
	string_trim_rear(s);
}

inline std::vector<std::string> string_tokenize(
    const std::string & str, const std::string & delimiters=" \t\n")
{
  std::vector<std::string> tokens;
  /// Skip delimiters at beginning.
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  /// Find first "non-delimiter".
  std::string::size_type pos = str.find_first_of(delimiters, lastPos);
  
  while (std::string::npos != pos || std::string::npos != lastPos) {
    /// Found a token, add it to the vector.
    tokens.push_back(str.substr(lastPos, pos - lastPos));
    /// Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    /// Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
  return tokens;
}

#include <string.h>

inline void tr(char* str, char search, char replace) {
  int size = strlen(str);
  for (int i=0; i< size; i++) {
    if (str[i] == search)
      str[i] = replace;
  }
}

inline std::vector<std::string> get_backtrace(bool use_addr2line=true) {
  std::vector<std::string> returned;
  
    void *array[30];
    size_t size;
    char **strings;
    size_t i;
    std::string addr2line;
    if (use_addr2line) {
      //      addr2line = find_executable("addr2line", false);
      addr2line = "/usr/bin/addr2line";///TODO: no hardcoding
    }
    char ptr[64];
    char fn_lineno[PATH_MAX];
    char fn_name[PATH_MAX];

    size = backtrace (array, 30);
    strings = backtrace_symbols (array, size);

    if (size > 40) {
      std::stringstream ss;
      ss<<"frames trimmed down to 40 from "<<size<<" stack frames";
      returned.push_back(ss.str());
      size = 40;
    }
    {
      std::stringstream ss;
      ss<<"begin stack trace of "<<size<<" stack frames";
      returned.push_back(ss.str());
    }


    
    for (i = 0; i < size; i++) {
        std::string lineno = "\n";
	if (!addr2line.empty()) {
	  sprintf(ptr, "%p", array[i]);
	  std::vector<std::string> bin_addr = string_tokenize(strings[i], "[");
	  std::vector<std::string> bin_func = string_tokenize(bin_addr[0], "(");
	  std::string bin = bin_func[0];
	  string_trim(bin);

	  std::string cmd = addr2line + " --exe=" + bin + " " + ptr + " -f -C";
	  
	  FILE * output = popen(cmd.c_str(), "r");
	  fgets(fn_name, sizeof(fn_name), output);
	  fgets(fn_lineno, sizeof(fn_lineno), output);
	  tr(fn_name, '\n', ' ');
	  tr(fn_lineno, '\n', ' ');
	  lineno += fn_lineno;
	  lineno += " in ";
	  lineno += fn_name;
	  string_trim(lineno);
	  lineno = " (" + lineno + ")";
	  pclose(output);
        }
        //fprintf(outf, "    %s%s", strings[i], lineno.c_str());
	std::string ret = lineno;
	ret +=  " ";
	ret.append(strings[i]);
	ret += "\n";
	returned.push_back(ret);
    }
    free(strings);

    return returned;
}

/* Obtain a backtrace and print it to `stdout'. */
inline void print_backtrace(FILE * outf=stdout, bool use_addr2line=true)
{
  auto tr = get_backtrace(use_addr2line);
  for (auto l : tr)
    fprintf(outf, "%s", l.c_str());
    
}

#endif
