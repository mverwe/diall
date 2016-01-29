//
// producer of container with heavy-ion event properties
//

#include "UserCode/diall/interface/triggerProducer.h"

#include <ostream>
using namespace std;

ClassImp(triggerProducer)

//__________________________________________________________
triggerProducer::triggerProducer() :
inputBase("triggerProducer"),
  fTriggerMapName("hiEventContainer"),
  fTriggerMap(0)
{
  //default constructor
}

//__________________________________________________________
triggerProducer::triggerProducer(const char *name) :
  inputBase(name),
  fTriggerMapName("triggerMap"),
  fTriggerMap(0)
{
  //standard constructor
}

//__________________________________________________________
void triggerProducer::SetInput(TChain *chain) {

  inputBase::SetInput(chain);
  //Init();
  
}

//__________________________________________________________
Bool_t triggerProducer::Init() {

  if(!inputBase::Init()) return kFALSE;

  fInit = kTRUE;
  
  return kTRUE;
}
//__________________________________________________________
Bool_t triggerProducer::InitEventObjects() {
  
  //Create event objects
  if(!fEventObjects) {
    Printf("%s: fEventObjects does not exist. Cannot store output",GetName());
    return kFALSE;
  } else {
    if(!fEventObjects->FindObject(fTriggerMapName)) {
      fTriggerMap = new triggerMap(fTriggerMapName);
      fEventObjects->Add(fTriggerMap);
      Printf("Created triggerMap object");
    }
  }
  return kTRUE;
}

//__________________________________________________________
Bool_t triggerProducer::Run(Long64_t entry) {

  //run analysis
  Long64_t centry = LoadTree(entry);
  if(centry<0) return kFALSE;

  if(!InitEventObjects()) return kFALSE;

  int val[20000] = {0};
  TBranch *br[20000];
  TObjArray *objarr = fChain->GetListOfBranches();
  for(int i = 0; i < objarr->GetEntries(); ++i) {
    TString str = objarr->At(i)->GetName();
    //if(!str.Contains("HLT_HIL2Mu15_v1")) continue;
    //Printf("%s val: %d",str.Data(),val[i]);
    // TBranch* branch = GetBranch(str.Data());
    // if(!branch) continue;
    
    if(fChain->SetBranchAddress(str.Data(),&val[i],&br[i])<0) continue;
    fChain->GetEntry(entry);
    //Printf("%s val: %d",str.Data(),val[i]);
    fTriggerMap->AddTrigger(str.Data(),val[i]);
  }
 
  //fTriggerMap->PrintTriggers();

  return kTRUE; 
}

//__________________________________________________________
Long64_t triggerProducer::LoadTree(Long64_t entry) {

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
    Printf("triggerProducer: centry smaller than 0");
    return centry;  
  }
  
  fChain->GetEntry(entry);
  
  return centry;  
}
