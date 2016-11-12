#ifndef anaJetMass_h
#define anaJetMass_h

#include "TString.h"
#include "TH3F.h"

#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/lwJetContainer.h"

class anaJetMass : public anaBaseTask {
   
public:
   anaJetMass() {;}
   anaJetMass(const char *name, const char *title);
   virtual ~anaJetMass() {;}
   void Exec(Option_t *option="");
   void CreateOutputObjects();
   
   void SetJetsName(TString name) { fJetsName = name ; } 
   
 protected:
   TString          fJetsName;    //name of jet container
   lwJetContainer  *fJetsCont;    //!jet container
   
   TH1F            *fh1NJets;     //!number of jets in event
   TH3F            *fh3PtEtaPhi;  //!jet pt vs eta vs phi
   TH3F            *fh3PtEtaPhiJetID;  //!jet pt vs eta vs phi with jet id
   TH3F            *fh3PtEtaArea; //!jet pt vs eta vs area
   TH3F            *fh3PtEtaMass; //!jet pt vs eta vs mass
   
   ClassDef(anaJetMass,1)
};
#endif
