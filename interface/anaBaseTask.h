#ifndef anaBaseTask_h
#define anaBaseTask_h

#include "TTask.h"
#include "TList.h"
#include "TClonesArray.h"
#include "TDirectory.h"

#include "UserCode/diall/interface/hiEventContainer.h"
#include "UserCode/diall/interface/triggerMap.h"

class anaBaseTask : public TTask {
   
public:
   anaBaseTask() {;}
   anaBaseTask(const char *name, const char *title);
   virtual ~anaBaseTask() {;}
   void Exec(Option_t *option="");

   void ConnectEventObject(TList *l)   {fEventObjects = l; }
   void ConnectOutput(TList *l)        {fOutput       = l; }

   virtual void CreateOutputObjects();
   
   TList *GetOutput()           const  { return fOutput; }

   void SetHiEvtName(TString name)       { fEvtName  = name ; }
   void SetTriggerMapName(TString name)  { fTriggerMapName  = name ; }
   void AddTriggerSel(std::string s)     { fTriggerList.push_back(s); }
   void AddTriggerSelSubset(std::string s) { fTriggerListSubset.push_back(s); }

   bool   SelectEvent() const;
   void   SetMaxZVertex(float z)         { fZVertexMax = z; }
   void   DoCollisionEventSel(bool b)    { fCollSel   = b; }
   void   DoHBHENoiseFilter(bool b)      { fHBHENoise = b; }
   void   DoHBHENoiseFilterLoose(bool b) { fHBHENoiseLoose = b; }
   void   DoPrimaryVertexFilter(bool b)  { fPrimaryVertex = b; }
   void   DoClusterCompatibilityFilter(bool b) {fClusterCompat = b;}
   void   DoHFCoincFilter(bool b)        { fHFCoinc = b; }
   
   void   SetCentralityRange(double min, double max) { fCentMin = min; fCentMax = max; }
   
   //temporary: need smarter way to store HLT triggers
   void   DoPFJet80(bool b)            { fPFJet80 = b; }
   void   DoPhoton30(bool b)           { fPhoton30 = b; }
   void   DoExcludePhoton30(bool b)    { fPhoton30Excl = b; }
   void   DoTrk24(bool b)              { fTrk24 = b; }
   void   DoTrk45(bool b)              { fTrk45 = b; }
   
 protected:
   Bool_t CheckInheritance(TClonesArray *c, TString clsName);
   TList      *fEventObjects;                   //!list with event objects
   Bool_t      fInitOutput;                     //output objects created
   TDirectory *fDir;                            //!output directory
   TList      *fOutput;                         //!list with output objects
   TString           fEvtName;                  //name of hi event container
   hiEventContainer *fHiEvent;                  //!event container
   TString           fTriggerMapName;           //name of trigger map
   triggerMap       *fTriggerMap;               //!trigger map
   std::vector<std::string> fTriggerList;       // triggers to accept event OR
   std::vector<std::string> fTriggerListSubset; // triggers to accept event AND with fTriggerList
   float       fZVertexMax;                     // max z-vertex to accept
   bool        fCollSel;                        //flag to do collision event sel
   bool        fHBHENoise;                      //HBHE noise filter
   bool        fHBHENoiseLoose;                 //HBHE noise filter loose
   bool        fPrimaryVertex;                  // primary vertex filter
   bool        fClusterCompat;                  // cluster compatibility
   bool        fHFCoinc;                        // HF coincidence filter

   double      fCentMin;                        //cent min
   double      fCentMax;                        //cent max
   bool        fPFJet80;                        //PFJet80
   bool        fPhoton30;                       //Photon30
   bool        fPhoton30Excl;                   //excludd Photon30 events
   bool        fTrk24;                          //Trk24
   bool        fTrk45;                          //Trk45
   
   ClassDef(anaBaseTask,1)
};
#endif
