#include <tinyxml2.h>
#include <iostream>

void page(tinyxml2::XMLNode* page) {



  //std::cout<<page->Value()<<std::endl;
  
  tinyxml2::XMLNode* title = page->FirstChildElement("title");

  //std::cout<<title->Value()<<std::endl;

  const char* word = title->FirstChild()->ToText()->Value();

  std::cout<<word<<std::endl;
    
    
  tinyxml2::XMLNode* text = page->FirstChildElement("revision")->FirstChildElement("text");
  
  
  const char* definition = text->FirstChild()->ToText()->Value();
  
  std::cout<< definition <<std::endl;
}

int main() {
  tinyxml2::XMLDocument doc;
  doc.LoadFile( "wiki-fr.xml" );

  tinyxml2::XMLNode* root = doc.FirstChildElement("mediawiki");

  tinyxml2::XMLNode* currentpage = root->FirstChildElement("page");

  while (currentpage) {
    
    page(currentpage);

    //
    currentpage = currentpage->NextSiblingElement("page");
  }
  
  return 0;
}
