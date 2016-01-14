#ifndef triggerMap_h
#define triggerMap_h

//
// trigger info
//

#include <map>

#include <TNamed.h>

class triggerMap : public TNamed {
 public:
  triggerMap();
  triggerMap(const char *name);
  virtual ~triggerMap() {;}

  int      TriggerFired(std::string str)  {std::map<std::string,int>::iterator it = fMapTriggers.find(str); if (it != fMapTriggers.end()) return it->second; else return -2; }
  
  void     AddTrigger(std::string str,int i) { std::map<std::string,int>::iterator it = fMapTriggers.find(str); if (it != fMapTriggers.end()) it->second = i; else fMapTriggers.insert(std::pair<std::string,int>(str,i));}

  void     PrintTriggers() const;
  
 protected:
  std::map<std::string,int> fMapTriggers;   //trigger map

 private:
  triggerMap(const triggerMap& obj); // copy constructor
  triggerMap& operator=(const triggerMap& other); // assignment

  ClassDef(triggerMap,1)
};
#endif
