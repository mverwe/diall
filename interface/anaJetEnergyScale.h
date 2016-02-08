// analysis to extract jet energy scale
// Author: M. Verweij

#ifndef anaJetEnergyScale_h
#define anaJetEnergyScale_h

#include "TString.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TTree.h"

#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include <UserCode/diall/interface/rhoMap.h>

class anaJetEnergyScale : public anaBaseTask {
   
public:
   anaJetEnergyScale() {;}
   anaJetEnergyScale(const char *name, const char *title);
   virtual ~anaJetEnergyScale() {;}
   void Exec(Option_t *option="");
   void CreateOutputObjects();

   void SetNCentBins(Int_t n)          { fNcentBins    = n;  }
   void SetGenJetsName(TString name)   { fJetsName = name ;  }
   void SetRecJetsName(TString name)   { fJets2Name = name ; }
   void SetUseForestMatching(bool b)   { fUseForestMatching = b; }
   void SetRhoMapName(TString n)       { fRhoMapName    = n ; }
   void SetMaxDistance(double dr)      { fMaxDist       = dr; }
   void SetUseRawPt(bool b)            { fUseRawPt      = b;  }     
 
 protected:
   Int_t             fNcentBins;
   TString           fJetsName;       //name of jet container
   lwJetContainer   *fJetsCont;       //!jet container
   TString           fJets2Name;      //name of jet container
   lwJetContainer   *fJets2Cont;      //!jet container

   bool              fUseForestMatching; //use matched ref jet from forest
   bool              fUseRawPt;          //use raw pt

   TString           fRhoMapName;             //rho map name
   rhoMap           *fRhoMap;                 //!rho map

   double            fMaxDist;                //max distance between matched jets  
 
   TH1F      *fhEventSel;
   TH1F      *fhCentrality;
   TH1F      *fhNPV;
   
   TH2F      **fh2PtEtaNoMatching;    //!
   TH3F      **fh3PtEtaPhiNotMatched; //!
   TH3F      **fh3PtEtaPhiMatched;    //!

   TH3F      **fh3PtTrueNPVDeltaPt;   //!
   TH3F      **fh3PtTrueNPVDeltaPtRel;//!
   TH3F      **fh3PtTrueNPVScalePt;   //!
   TH3F      **fh3PtTruePtSubNPV;     //!

   TH3F      **fh3PtTrueEtaDeltaPt;   //!
   TH3F      **fh3PtTrueEtaDeltaPtRel;//!
   TH3F      **fh3PtTrueEtaScalePt;   //!
   TH3F      **fh3PtTruePtSubEta;     //!
  
   TH3F      **fh3PtTrueEtaDeltaM;    //!
   TH3F      **fh3PtTrueEtaDeltaMRel; //!
   TH3F      **fh3PtTrueEtaScaleM;    //!
   TH3F      **fh3PtTrueMTrueScaleM;  //!

   ClassDef(anaJetEnergyScale,3)
};
#endif
