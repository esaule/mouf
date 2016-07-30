#include <string>
#include "util.hpp"


bool process_template(std::string def, std::string& out) {
  out = def;

  //page links are denoted between {{ and }}.
  
  size_t end = def.find("}}");

  if (end == std::string::npos) {
    //std::cerr<<"no template"<<std::endl;
    return false;
  }
  
  size_t first = def.find("{{");

  if (first == std::string::npos) {
    std::cerr<<"MALFORMEDTEMPLATE: "<<def<<std::endl;
    return false;
  }
  
  size_t check = def.rfind("{{", end);

  if (first != check) {
    std::cerr<<"RECURSIVETEMPLATE in :"<<def<<std::endl;
    return false;
  }

  //std::string templatestr = def.substr(check+2, (end-check)-2);

  //std::cerr<<"PAGELINK:" <<pagelink<<std::endl;

  out = def.substr(0,check) 
    + def.substr( end+2 , std::string::npos) ;

  return true;
}

bool process_pagelink(std::string def, std::string& out) {
  out = def;

  //page links are denoted between [[ and ]].
  //if they have no | then the display text should be the name of the page.
  //if they have one | then the first part is the page name and the second the display text.
  //if they have two |s, then it is an image link. 
  
  size_t end = def.find("]]");

  if (end == std::string::npos) {
    //std::cerr<<"no definition"<<std::endl;
    return false;
  }
  
  size_t first = def.find("[[");

  if (first == std::string::npos) {
    std::cerr<<"MALFORMEDLINK: "<<def<<std::endl;
    return false;
  }
  
  size_t check = def.rfind("[[", end);

  if (first != check) {
    std::cerr<<"RECURSIVE in :"<<def<<std::endl;
    return false;
  }

  std::string pagelink = def.substr(check+2, (end-check)-2);

  //std::cerr<<"PAGELINK:" <<pagelink<<std::endl;

  
  size_t firstpipe = pagelink.find("|");
  if (firstpipe == std::string::npos) {//no pipe
    //std::cerr<<"no pipe"<<std::endl;
    out = def.substr(0,check) 
      + pagelink 
      + def.substr( end+2 , std::string::npos) ;

    //std::cerr<<"PREVDEF: "<<def<<std::endl;
    //std::cerr<<"NEWDEF: "<<out<<std::endl;
    
    return true;
  } else {
    size_t secondpipe = pagelink.find("|", firstpipe+1);
    if (secondpipe == std::string::npos) {//one pipe
      //std::cerr<<"one pipe"<<std::endl;
      out = def.substr(0,check) 
	+ pagelink.substr(firstpipe+1, std::string::npos) 
	+ def.substr( end+2 , std::string::npos) ;

      //std::cerr<<"PREVDEF: "<<def<<std::endl;
      //std::cerr<<"NEWDEF: "<<out<<std::endl;
    
      
      return true;
    } else {
      //std::cerr<<"two pipes"<<std::endl;

      out = def.substr(0,check) 
	+ def.substr( end+2 , std::string::npos) ;

      //std::cerr<<"PREVDEF: "<<def<<std::endl;
      //std::cerr<<"NEWDEF: "<<out<<std::endl;
      return true;
    }
  }
  
  
  return false;
}


bool process_definition(std::string def, std::string& out) {
  if ( process_pagelink (def, out))
    return true;
  if ( process_template (def, out))
    return true;
  return false;
  
}


std::string process_definition(std::string def) {

  std::string out;

  while (process_definition(def, out)) {
    def = out;
    out = "";
  }


  
  return out;
}
