#ifndef GUI_INTERFACE
#define GUI_INTERFACE

#include <string>

class GuiOptionChooser;

struct VerbListener {
  //  typedef tact::engine::string Verb;
  typedef std::string Verb;

  virtual void say(const GuiOptionChooser* from, Verb verb)=0;
};

#endif
