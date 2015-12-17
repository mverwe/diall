// analysis to compare PF with calo jets
// Author: M. Verweij

#ifndef anaPFvsCaloJet_h
#define anaPFvsCaloJet_h

#include "TString.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TTree.h"

#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/lwJetContainer.h"

class anaPFvsCaloJet : public anaBaseTask {
   
public:
   anaPFvsCaloJet() {;}
   anaPFvsCaloJet(const char *name, const char *title);
   virtual ~anaPFvsCaloJet() {;}
   void Exec(Option_t *option="");
   void CreateOutputObjects();

   void SetPFJetsName(TString name)   { fJetsName = name ;  }
   void SetCaloJetsName(TString name) { fJets2Name = name ; }
   void SetStoreTree(bool b)          { fStoreTree = b;     }
   
 protected:
   TString           fJetsName;       //name of jet container
   lwJetContainer   *fJetsCont;       //!jet container
   TString           fJets2Name;      //name of jet container
   lwJetContainer   *fJets2Cont;      //!jet container

   int        fRun;
   int        fLumi;
   int        fEvent;
   int        fNjet;
   float      fJetPt[200];
   float      fJetRawPt[200];
   float      fJetEta[200];
   float      fJetPhi[200];
   float      fChargedSum[200];
   float      fChargedMax[200];
   int        fChargedN[200];
   float      fChargedHardSum[200];
   float      fChargedHardMax[200];
   int        fChargedHardN[200];
   float      fPhotonSum[200];
   float      fPhotonMax[200];
   int        fPhotonN[200];
   float      fNeutralSum[200];
   float      fNeutralMax[200];
   int        fNeutralN[200];
   float      fEmSum[200];
   float      fEmMax[200];
   int        fEmN[200];
   float      fMuSum[200];
   float      fMuMax[200];
   int        fMuN[200];
   int        fNConst[200];
   float      fJetPtMatch[200];
   float      fDeltaR[200];
   bool       fStoreTree;
   TTree     *fTreeOut;
   
   TH1F      *fhEventSel;
   TH1F      *fhCentrality;
   TH1F      *fhNPV;
   
   TH2F      *fh2PtEtaNoMatching;
   TH3F      *fh3PtEtaPhiNotMatched;
   TH3F      *fh3PtEtaPhiMatched;

   TH3F      *fh3PtTrueNPVDeltaPt;
   TH3F      *fh3PtTrueNPVDeltaPtRel;
   TH3F      *fh3PtTrueNPVScalePt;
   TH3F      *fh3PtTruePtSubNPV;

   TH3F      *fh3PtTrueEtaDeltaPt;
   TH3F      *fh3PtTrueEtaDeltaPtRel;
   TH3F      *fh3PtTrueEtaScalePt;
   TH3F      *fh3PtTruePtSubEta;
   
   ClassDef(anaPFvsCaloJet,2)
};
#endif
