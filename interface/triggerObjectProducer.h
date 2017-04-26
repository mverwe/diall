#ifndef triggerObjectProducer_h
#define triggerObjectProducer_h

//
// heavy-ion event container producer
//

#include <TNamed.h>
#include <TChain.h>
#include <TString.h>
#include <TBranch.h>

#include "UserCode/diall/interface/inputBase.h"
#include "UserCode/diall/interface/triggerObjectMap.h"

class triggerObjectProducer : public inputBase {
 public:
  triggerObjectProducer();
  triggerObjectProducer(const char *name);
  virtual ~triggerObjectProducer() {;}

  void     SetInput(TChain *chain);
  Bool_t   Init();
  Long64_t LoadTree(Long64_t entry);
  Bool_t   InitEventObjects();
  Bool_t   Run(Long64_t entry);

  void     SetTriggerMapName(TString n)    { fTriggerMapName = n; }
 
  const char* GetTriggerMapName() const { return fTriggerMapName.Data() ; }
  triggerObjectMap *GetTriggerMap()      const { return fTriggerMap; }
  
 protected:
  TString                      fTriggerMapName;        // name of trigger object map
  triggerObjectMap            *fTriggerMap;            //! trigger object map

  // Declaration of leaf types
  std::vector<double>  *fTrigObjPt;
  std::vector<double>  *fTrigObjEta;
  std::vector<double>  *fTrigObjPhi;
   
  // List of branches
  TBranch        *fTrigObjPtBranch;   //!
  TBranch        *fTrigObjEtaBranch;   //!
  TBranch        *fTrigObjPhiBranch;   //!
   
  
 private:
  triggerObjectProducer(const triggerObjectProducer& obj); // copy constructor
  triggerObjectProducer& operator=(const triggerObjectProducer& other); // assignment
  
  ClassDef(triggerObjectProducer,1)
};
#endif
