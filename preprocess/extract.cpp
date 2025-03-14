#include <tinyxml2.h>
#include <iostream>
#include <sstream>
#include "util.hpp"
#include "definition.hpp"
#include "genre.hpp"
#include <vector>

struct dict_entry{
  typedef std::string string;
  
  string word;
  char genre;
  string definition;
};


void extract_definition(const std::string& wikicode, std::string w,
			std::vector<dict_entry>& out) {
  std::stringstream ss(wikicode);

  std::string line;

  std::string word = w;
  std::string def;
  char gen = '?';
  
  bool in_fr_section = false;
  bool in_noun_subsection = false;
  bool got_genre = false;
  bool got_definition = false;

  
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
      got_definition = false;
      got_genre = false;
      gen = '?';
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
      //std::cout<<genre(line)<<std::endl;
      gen = genre(line);
      got_genre = true;
    }
    if (starts_with(line, "# ")) {
      //      std::cout<<"DEFINITION :"<<line<<std::endl;
      //std::cout<<process_definition(line.substr(2, line.length()))<<std::endl;
      def = trim(process_definition(line.substr(2, line.length())));
      got_definition = true;
    }
    if (starts_with(line, "#* ")) {
      //std::cout<<"EXAMPLE :"<<line<<std::endl;
    }
    
  end:
    if (got_genre && got_definition) {
      if (gen == '?')
	std::cerr<<word<<"\t"<<gen<<"\t"<<def<<std::endl;
      else {
	dict_entry de;
	de.word = word;
	de.genre = gen;
	de.definition = def;
	out.push_back(de);
	//	std::cout<<word<<"\t"<<gen<<"\t"<<def<<std::endl;
      }
      
      got_genre = false;
      got_definition = false;
    }
    /*
    std::cout<<(in_fr_section?"infr":"outfr")<<" "
	     <<(in_noun_subsection?"nom":"notnom")<<" "
	     <<":"<<line<<std::endl;
    */
  }
  
}

bool page(tinyxml2::XMLNode* page, std::string target,
	  std::vector<dict_entry>& out) {
  //std::cout<<page->Value()<<std::endl;
  
  tinyxml2::XMLNode* title = page->FirstChildElement("title");

  if (title == nullptr) return false;
  
  //std::cout<<title->Value()<<std::endl;

  std::string word ( title->FirstChild()->ToText()->Value());

  if (target.compare("-") != 0
      && word.compare(target) != 0) return false;
  
  //std::cout<<word<<std::endl;

  auto rev = page->FirstChildElement("revision");

  if (rev == nullptr) return false;
  
  tinyxml2::XMLNode* text = rev->FirstChildElement("text");

  if (text == nullptr) return false;

  auto realtext = text->FirstChild();

  if (realtext == nullptr) return false;
  
  std::string definition ( realtext->ToText()->Value());
  
  // std::cout<< definition <<std::endl;

  // std::cout<<std::endl<<"======"<<std::endl;
  
  extract_definition (definition, word, out);

  return true;
}

int main(int argc, char* argv[]) {
  tinyxml2::XMLDocument doc;
  doc.LoadFile( "wiki-fr.xml" );

  std::cerr<<"xml loaded"<<std::endl;
  
  tinyxml2::XMLNode* root = doc.FirstChildElement("mediawiki");

  tinyxml2::XMLNode* currentpage = root->FirstChildElement("page");

  std::string target = (argc>1?argv[1]:"-");

  std::vector<dict_entry> dict;
  
  while (currentpage) {
    
    bool ret = page(currentpage, target, dict);

    if (ret && target.compare("-") != 0)
      break;
    
    currentpage = currentpage->NextSiblingElement("page");
  }

  for (auto p : dict) {
    if (p.genre != '?') {
      if (p.definition.size() > 3) {
	std::cout<<p.word
		 <<"\t"<<p.genre
		 <<"\t"<<p.definition<<'\n';
      }
    }
  }
  std::cout<<std::flush;
  
  return 0;
}
