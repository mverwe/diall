#include "UserCode/diall/interface/anaBaseTask.h"

#include "TClass.h"
#include "TDirectory.h"

ClassImp(anaBaseTask)
   
anaBaseTask::anaBaseTask(const char *name, const char *title) 
:TTask(name,title),
  fEventObjects(0),
  fInitOutput(0),
  fDir(0x0),
  fOutput(0),
  fEvtName(""),
  fHiEvent(),
  fTriggerMapName(""),
  fTriggerMap(),
  fTriggerList(0),
  fCollSel(0),
  fHBHENoise(0),
  fCentMin(-999),
  fCentMax(999),
  fPFJet80(0),
  fPhoton30(0),
  fPhoton30Excl(0),
  fTrk24(0),
  fTrk45(0)
{

}

//----------------------------------------------------------
void anaBaseTask::Exec(Option_t * /*option*/)
{
  if(!fInitOutput) CreateOutputObjects();

  //Get event properties
  if(!fHiEvent && !fEvtName.IsNull()) {
     fHiEvent = dynamic_cast<hiEventContainer*>(fEventObjects->FindObject(fEvtName.Data()));
     if(!fHiEvent) {
       Printf("%s: !!WARNING: Couldn't locate %s branch",GetName(),fEvtName.Data());
       return;
     }
  }

  //Get trigger map
  if(!fTriggerMap && !fTriggerMapName.IsNull()) {
    fTriggerMap = dynamic_cast<triggerMap*>(fEventObjects->FindObject(fTriggerMapName.Data()));
    if(!fTriggerMap) {
      Printf("%s: !!WARNING: Couldn't locate %s branch",GetName(),fTriggerMapName.Data());
      return;
    }
  }

  //if(!SelectEvent()) return;
}

//----------------------------------------------------------
void anaBaseTask::CreateOutputObjects() {

  if(!fDir) {
    TString dirName = GetName();
    fDir = new TDirectory(dirName.Data(),dirName.Data());
  }
  fDir->cd();
  if(!fOutput) fOutput = new TList();
  if(!fOutput) {
    Printf("%s: There is a problem. fOutput does not exist",GetName());
    return;
  }
  fOutput->SetName(GetName());
  
  fInitOutput = kTRUE;
}

//----------------------------------------------------------
Bool_t anaBaseTask::CheckInheritance(TClonesArray *c, TString clsName) {

  //check if inheritance is to expected type
  TString objname(c->GetClass()->GetName());
  TClass cls(objname);
  if (!cls.InheritsFrom(clsName)) {
    Printf("%s: Objects of type %s are not inherited from %s!", 
           GetName(), cls.GetName(), clsName.Data()); 
    return kFALSE;
  }
  return kTRUE;
}

//----------------------------------------------------------
bool anaBaseTask::SelectEvent() const {
  //
  //pcollision, hbhenoise
  //triggers

  bool accept = true;
  
  if(fHiEvent) {
    if(std::abs(fHiEvent->GetVz())>15.) accept = false;
    if(fCollSel && !fHiEvent->GetColl()) accept = false;
    //  printf("accept 1 %d %d %d\n", accept, fCollSel, !fHiEvent->GetColl()); }
    if(fHBHENoise && !fHiEvent->GetHBHENoise()) accept = false;
    //  printf("accept 2 %d %d %d\n", accept, fHBHENoise, !fHiEvent->GetHBHENoise()); }
    if(fCentMin>-1) {
      double cent = fHiEvent->GetCentrality();
      if(cent<fCentMin && cent>fCentMax) accept = false;
//	printf("accept 3 %d %f %d\n", accept, cent, !fHiEvent->GetCentrality()); }
    }
    
    if(fPFJet80 && !fHiEvent->GetPFJet80()) { accept = false;
  //    printf("accept 4 %d %d %d\n", accept, fPFJet80, !fHiEvent->GetPFJet80() );
    }
    if(fPhoton30 && !fHiEvent->GetPhoton30()) { accept = false;
    //  printf("accept 5 %d %d %d\n", accept, fPhoton30, !fHiEvent->GetPhoton30() );
    }
    if(fTrk24 && !fHiEvent->GetTrk24()) accept = false;
     // printf("accept 6 %d %d %d\n", accept, fTrk24, !fHiEvent->GetTrk24()); }
    if(fTrk45 && !fHiEvent->GetTrk45()) { accept = false;
     // printf("accept 7 %d %d %d\n", accept, fTrk45, !fHiEvent->GetTrk45() );
    }

    //special for express stream
    if(fPhoton30Excl && fHiEvent->GetPhoton30()) { accept = false;
 //     printf("accept 8 %d %d %d\n", accept, fPhoton30Excl, !fHiEvent->GetPhoton30() );
    }
  }

  if(fTriggerMap && accept) {
    bool passTrig = false;
    for(std::vector<std::string>::const_iterator s = fTriggerList.begin(); s != fTriggerList.end(); ++s) {
       int fire = fTriggerMap->TriggerFired(*s);
       if(fire>0) passTrig = true;
    }
    accept = passTrig;
  }

  return accept;
}
