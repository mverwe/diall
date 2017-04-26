//
// container with trigger object info
//

#include <iostream>
#include <map>

#include "UserCode/diall/interface/triggerObjectMap.h"

ClassImp(triggerObjectMap)

//__________________________________________________________
triggerObjectMap::triggerObjectMap() :
TNamed("triggerObjectMap","triggerObjectMap"),
  fTriggerPt(),
  fTriggerEta(),
  fTriggerPhi()
{
  //default constructor
}

//__________________________________________________________
triggerObjectMap::triggerObjectMap(const char *name) :
  TNamed(name,name),
  fTriggerPt(),
  fTriggerEta(),
  fTriggerPhi()
{
  //standard constructor
}

//__________________________________________________________
void triggerObjectMap::PrintTriggerObject() const {

  std::cout << GetName() << ":  pt: " << fTriggerPt << "  eta: " << fTriggerEta << "  phi: " << fTriggerPhi << std::endl;
  
  // //  std::map<std::string,int>::iterator it;
  // std::map<std::basic_string<char>,int>::const_iterator it;
  // for(it = fMapTriggerPt.begin(); it != fMapTriggerPt.end(); it++) {
  //   std::cout << it->first << "  " << it->second << std::endl;
  // }
}
