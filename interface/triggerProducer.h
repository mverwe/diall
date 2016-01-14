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

  const char* GetHIEventContName() const { return fTriggerMapName.Data() ; }
  
 protected:
  TString                      fTriggerMapName;  // name of trigger map
  triggerMap                  *fTriggerMap;      //! trigger map
  
 private:
  triggerProducer(const triggerProducer& obj); // copy constructor
  triggerProducer& operator=(const triggerProducer& other); // assignment
  
  ClassDef(triggerProducer,1)
};
#endif
