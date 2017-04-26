//
// producer of container with heavy-ion event properties
//

#include "UserCode/diall/interface/triggerObjectProducer.h"

#include <iostream>
using namespace std;

ClassImp(triggerObjectProducer)

//__________________________________________________________
triggerObjectProducer::triggerObjectProducer() :
inputBase("triggerObjectProducer"),
  fTriggerMapName("hiEventContainer"),
  fTriggerMap(0),
  fTrigObjPt(),
  fTrigObjEta(),
  fTrigObjPhi(),
  fTrigObjPtBranch(),
  fTrigObjEtaBranch(),
  fTrigObjPhiBranch()
{
  //default constructor
}

//__________________________________________________________
triggerObjectProducer::triggerObjectProducer(const char *name) :
  inputBase(name),
  fTriggerMapName("triggerObjectMap"),
  fTriggerMap(0),
  fTrigObjPt(),
  fTrigObjEta(),
  fTrigObjPhi(),
  fTrigObjPtBranch(),
  fTrigObjEtaBranch(),
  fTrigObjPhiBranch()
 {
  //standard constructor
}

//__________________________________________________________
void triggerObjectProducer::SetInput(TChain *chain) {

  inputBase::SetInput(chain);
  //Init(); 
}

//__________________________________________________________
Bool_t triggerObjectProducer::Init() {

  if(!inputBase::Init()) return kFALSE;
  
  fChain->SetBranchAddress("pt",&fTrigObjPt,&fTrigObjPtBranch);
  fChain->SetBranchAddress("eta",&fTrigObjEta,&fTrigObjEtaBranch);
  fChain->SetBranchAddress("phi",&fTrigObjPhi,&fTrigObjPhiBranch);

  fInit = kTRUE;
  
  return kTRUE;
}
//__________________________________________________________
Bool_t triggerObjectProducer::InitEventObjects() {
  
  //Create event objects
  if(!fEventObjects) {
    Printf("%s: fEventObjects does not exist. Cannot store output",GetName());
    return kFALSE;
  } else {
    if(!fEventObjects->FindObject(fTriggerMapName)) {
      fTriggerMap = new triggerObjectMap(fTriggerMapName);
      fEventObjects->Add(fTriggerMap);
      Printf("Created triggerObjectMap object");
    }
  }
  return kTRUE;
}

//__________________________________________________________
Bool_t triggerObjectProducer::Run(Long64_t entry) {

  //run analysis
  Long64_t centry = LoadTree(entry);
  if(centry<0) return kFALSE;

  if(!InitEventObjects()) return kFALSE;

  if(fTrigObjPt->size()<1) return kFALSE;
  
  fTriggerMap->SetTriggerPt(fTrigObjPt->at(0));
  fTriggerMap->SetTriggerEta(fTrigObjEta->at(0));
  fTriggerMap->SetTriggerPhi(fTrigObjPhi->at(0));

  //fTriggerMap->PrintTriggerObject();

  return kTRUE; 
}

//__________________________________________________________
Long64_t triggerObjectProducer::LoadTree(Long64_t entry) {

  //overloaded LoadTree function
  if(!fChain) {
    Printf("fChain doesn't exist");
    return -5;
  }

  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Init();
    //Printf("%lld fCurrent: %d",entry,fCurrent);
  }
 
  // fChain->SetMakeClass(1);
 
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) {
    Printf("triggerObjectProducer: centry smaller than 0");
    return centry;  
  }
  
  fChain->GetEntry(entry);

  return centry;  
}
