#ifndef triggerObjectMap_h
#define triggerObjectMap_h

//
// trigger info
//

#include <map>

#include <TNamed.h>

class triggerObjectMap : public TNamed {
 public:
  triggerObjectMap();
  triggerObjectMap(const char *name);
  virtual ~triggerObjectMap() {;}

  /* float    GetTriggerPt(std::string str) {std::map<std::string,float>::iterator it = fMapTriggerPt.find(str); if (it != fMapTriggerPt.end()) return it->second; else return -2; }; */
  /* float    GetTriggerEta(std::string str) {std::map<std::string,float>::iterator it = fMapTriggerEta.find(str); if (it != fMapTriggerEta.end()) return it->second; else return -2; }; */
  /* float    GetTriggerPhi(std::string str) {std::map<std::string,float>::iterator it = fMapTriggerPhi.find(str); if (it != fMapTriggerPhi.end()) return it->second; else return -2; }; */
  
  /* void     AddTriggerPt(std::string str,float a) { std::map<std::string,float>::iterator it = fMapTriggerPt.find(str); if (it != fMapTriggerPt.end()) it->second = a; else fMapTriggerPt.insert(std::pair<std::string,float>(str,a));} */

  /* void     AddTriggerEta(std::string str,float a) { std::map<std::string,float>::iterator it = fMapTriggerEta.find(str); if (it != fMapTriggerEta.end()) it->second = a; else fMapTriggerEta.insert(std::pair<std::string,float>(str,a));} */

  /* void     AddTriggerPhi(std::string str,float a) { std::map<std::string,float>::iterator it = fMapTriggerPhi.find(str); if (it != fMapTriggerPhi.end()) it->second = a; else fMapTriggerPhi.insert(std::pair<std::string,float>(str,a));} */

  double GetTriggerPt()  const {return fTriggerPt;}
  double GetTriggerEta() const {return fTriggerEta;}
  double GetTriggerPhi() const {return fTriggerPhi;}
  
  void SetTriggerPt(double pt)   {fTriggerPt = pt;}
  void SetTriggerEta(double eta) {fTriggerEta = eta;}
  void SetTriggerPhi(double phi) {fTriggerPhi = phi;}

  void     PrintTriggerObject() const;
  
 protected:
  double fTriggerPt;
  double fTriggerEta;
  double fTriggerPhi;
  //std::map<std::string,float> fMapTriggerPt;   //trigger object pts
  //std::map<std::string,float> fMapTriggerEta;  //trigger object etas
  //std::map<std::string,float> fMapTriggerPhi;  //trigger object phis
  
 private:
  triggerObjectMap(const triggerObjectMap& obj); // copy constructor
  triggerObjectMap& operator=(const triggerObjectMap& other); // assignment

  ClassDef(triggerObjectMap,1)
};
#endif
