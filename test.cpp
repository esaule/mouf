#include <tinyxml2.h>
#include <iostream>
#include <sstream>

bool starts_with(const std::string& toCheck,
		 const std::string& prefix) {
  return (toCheck.rfind(prefix, 0) == 0);
}

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

bool process_definition(std::string def, std::string& out) {
  out = def;
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

void extract_definition(const std::string& wikicode) {

  std::stringstream ss(wikicode);

  std::string line;

  bool in_fr_section = false;
  bool in_noun_subsection = false;
  
  while (getline(ss, line , '\n')) {
    if (line.compare("== {{langue|fr}} ==") == 0) {
      in_fr_section = true;
      goto end;
    }
    if (starts_with(line, "== ")) {
      in_fr_section = false;
      goto end;
    }

    //here is where we really know if we are in the fr section or not
    //and we can skip if not
    if (!in_fr_section) goto end;

    //then search for nom sections
    if (starts_with(line, "=== {{S|nom|fr")) {
      in_noun_subsection = true;
      //std::cout<<"NEWNOUN :"<<line<<std::endl;
      goto end;
    }

    if (starts_with(line, "=== ")) {
      in_noun_subsection = false;
      goto end;
    }
    if (!in_noun_subsection) goto end;
    //search for info
    
    if (starts_with(line, "'''")) {
      //std::cout<<"GENRE :"<<line<<std::endl;
      std::cout<<genre(line)<<std::endl;
    }
    if (starts_with(line, "# ")) {
      std::cout<<"DEFINITION :"<<line<<std::endl;
      std::cout<<process_definition(line.substr(2, line.length()))<<std::endl;
    }
    if (starts_with(line, "#* ")) {
      //std::cout<<"EXAMPLE :"<<line<<std::endl;
    }
    
  end:;
    /*
    std::cout<<(in_fr_section?"infr":"outfr")<<" "
	     <<(in_noun_subsection?"nom":"notnom")<<" "
	     <<":"<<line<<std::endl;
    */
  }
  
}

bool page(tinyxml2::XMLNode* page, std::string target) {
  //std::cout<<page->Value()<<std::endl;
  
  tinyxml2::XMLNode* title = page->FirstChildElement("title");

  //std::cout<<title->Value()<<std::endl;

  std::string word ( title->FirstChild()->ToText()->Value());

  if (word.compare(target) != 0) return false;
  
  std::cout<<word<<std::endl;
    
  tinyxml2::XMLNode* text = page->FirstChildElement("revision")->FirstChildElement("text");
  
  std::string definition ( text->FirstChild()->ToText()->Value());
  
  // std::cout<< definition <<std::endl;

  // std::cout<<std::endl<<"======"<<std::endl;
  
  extract_definition (definition);

  return true;
}

int main(int argc, char* argv[]) {
  tinyxml2::XMLDocument doc;
  doc.LoadFile( "wiki-fr.xml" );

  std::cerr<<"xml loaded"<<std::endl;
  
  tinyxml2::XMLNode* root = doc.FirstChildElement("mediawiki");

  tinyxml2::XMLNode* currentpage = root->FirstChildElement("page");

  while (currentpage) {
    
    bool ret = page(currentpage, argv[1]);

    if (ret)
      break;
    
    currentpage = currentpage->NextSiblingElement("page");
  }
  
  return 0;
}
