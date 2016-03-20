#ifndef anaJetQA_h
#define anaJetQA_h

#include "TString.h"
#include "TH3F.h"

#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/lwJetContainer.h"

class anaJetQA : public anaBaseTask {
   
public:
   anaJetQA() {;}
   anaJetQA(const char *name, const char *title);
   virtual ~anaJetQA() {;}
   void Exec(Option_t *option="");
   void CreateOutputObjects();
   
   void SetJetsName(TString name) { fJetsName = name ; } 
   void SetJetIdType(int t)       { fJetIdType = t   ; }
   
 protected:
   TString          fJetsName;    //name of jet container
   lwJetContainer  *fJetsCont;    //!jet container
   int              fJetIdType;   //jet id type to be used
   
   
   TH1F            *fh1NJets;     //!number of jets in event
   TH3F            *fh3PtEtaPhi;  //!jet pt vs eta vs phi
   TH3F            *fh3PtEtaPhiJetID;  //!jet pt vs eta vs phi with jet id
   TH3F            *fh3PtEtaArea; //!jet pt vs eta vs area

   TH3F            *fh3PtEtaCHF; //!
   TH3F            *fh3PtEtaNHF; //!
   TH3F            *fh3PtEtaCEF; //!
   TH3F            *fh3PtEtaNEF; //!
   TH3F            *fh3PtEtaMUF; //!

   TH3F            *fh3PtEtaCHM; //!
   TH3F            *fh3PtEtaNHM; //!
   TH3F            *fh3PtEtaCEM; //!
   TH3F            *fh3PtEtaNEM; //!
   TH3F            *fh3PtEtaMUM; //!

   TH3F            *fh3PtEtaCHFCut; //!
   TH3F            *fh3PtEtaNHFCut; //!
   TH3F            *fh3PtEtaCEFCut; //!
   TH3F            *fh3PtEtaNEFCut; //!
   TH3F            *fh3PtEtaMUFCut; //!

   TH3F            *fh3PtEtaCHMCut; //!
   TH3F            *fh3PtEtaNHMCut; //!
   TH3F            *fh3PtEtaCEMCut; //!
   TH3F            *fh3PtEtaNEMCut; //!
   TH3F            *fh3PtEtaMUMCut; //!
   
   ClassDef(anaJetQA,1)
};
#endif
