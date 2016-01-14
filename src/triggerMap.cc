//
// container with trigger info
//

#include <iostream>
#include <map>

#include "UserCode/diall/interface/triggerMap.h"

ClassImp(triggerMap)

//__________________________________________________________
triggerMap::triggerMap() :
TNamed("triggerMap","triggerMap"),
  fMapTriggers()
{
  //default constructor
}

//__________________________________________________________
triggerMap::triggerMap(const char *name) :
  TNamed(name,name),
  fMapTriggers()
{
  //standard constructor
}


//__________________________________________________________
void triggerMap::PrintTriggers() const {
  
  //  std::map<std::string,int>::iterator it;
  std::map<std::basic_string<char>,int>::const_iterator it;
  for(it = fMapTriggers.begin(); it != fMapTriggers.end(); it++) {
    std::cout << it->first << "  " << it->second << std::endl;
  }
}
