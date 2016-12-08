#ifndef triggerProducer_h
#define triggerProducer_h

//
// heavy-ion event container producer
//

#include <TNamed.h>
#include <TChain.h>
#include <TString.h>
#include <TBranch.h>

#include "UserCode/diall/interface/inputBase.h"
#include "UserCode/diall/interface/triggerMap.h"

class triggerProducer : public inputBase {
 public:
  triggerProducer();
  triggerProducer(const char *name);
  virtual ~triggerProducer() {;}

  void     SetInput(TChain *chain);
  Bool_t   Init();
  Long64_t LoadTree(Long64_t entry);
  Bool_t   InitEventObjects();
  Bool_t   Run(Long64_t entry);
  
  void     SetTriggerMapName(TString n) { fTriggerMapName = n; }
  void     AddTrigger(std::string s)    { fTriggerList.push_back(s); }

  const char* GetTriggerMapName() const { return fTriggerMapName.Data() ; }
  triggerMap *GetTriggerMap()      const { return fTriggerMap; }
  
 protected:
  TString                      fTriggerMapName;  // name of trigger map
  triggerMap                  *fTriggerMap;      //! trigger map
  std::vector<std::string>     fTriggerList;     // triggers to add to event 
  int                          fTrigVal[20000];
  TBranch                     *fTrigBranch[20000];
  int                          fNTriggers;
 
 private:
  triggerProducer(const triggerProducer& obj); // copy constructor
  triggerProducer& operator=(const triggerProducer& other); // assignment
  
  ClassDef(triggerProducer,1)
};
#endif
