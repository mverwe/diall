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
   void SetGenJetsName(TString name)   { fJetsGenName = name ;  }
   void SetRecJetsName(TString name)   { fJetsRecName = name ; }
   void SetUseForestMatching(bool b)   { fUseForestMatching = b; }
   void SetRhoMapName(TString n)       { fRhoMapName    = n ; }
   void SetMaxDistance(double dr)      { fMaxDist       = dr; }
   void SetUseRawPt(bool b)            { fUseRawPt      = b;  }     
   void SetRefPartonFlavor(int min, int max)  { fRefPartonFlavorMin = min; fRefPartonFlavorMax = max;} 
   void SetMinJetPtRec(double pt)      { fMinJetPtRec = pt ; }
   void SetMinJetPtRaw(double pt)      { fMinJetPtRaw = pt ; } 
  
 protected:
   Int_t             fNcentBins;
   TString           fJetsGenName;       //name of gen jet container
   lwJetContainer   *fJetsGenCont;       //!gen jet container
   TString           fJetsRecName;      //name of rec jet container
   lwJetContainer   *fJetsRecCont;      //!rec jet container

   bool              fUseForestMatching; //use matched ref jet from forest
   bool              fUseRawPt;          //use raw pt

   TString           fRhoMapName;             //rho map name
   rhoMap           *fRhoMap;                 //!rho map

   double            fMaxDist;                //max distance between matched jets  
   int               fRefPartonFlavorMin;     //required parton flavor
   int               fRefPartonFlavorMax;     //required parton flavor
   double            fMinJetPtRec;            //min jet pt for reco jets
   double            fMinJetPtRaw;            //min RAW jet pt for reco jets

   TH1F      *fhEventSel;
   TH1F      *fhCentrality;
   TH1F      *fhNPV;
   
   TH2F      **fh2PtEtaNoMatching;    //!
   TH3F      **fh3PtEtaPhiNotMatched; //!
   TH3F      **fh3PtEtaPhiMatched;    //!
   TH3F      **fh3PtEtaPhiMatched2;   //!

   TH3F      **fh3PtTrueNPVDeltaPt;   //!
   TH3F      **fh3PtTrueNPVDeltaPtRel;//!
   TH3F      **fh3PtTrueNPVScalePt;   //!
   TH3F      **fh3PtTruePtSubNPV;     //!

   TH3F      **fh3PtTrueEtaDeltaPt;   //!
   TH3F      **fh3PtTrueEtaDeltaPtRel;//!
   TH3F      **fh3PtTrueEtaScalePt;   //!
   TH3F      **fh3PtTrueEtaFineScalePt; //!
   TH3F      **fh3PtRawEtaFineScalePt; //!
   TH3F      **fh3PtRecoEtaFineScalePt; //!
   TH3F      **fh3PtTruePtSubEta;     //!
   TH3F      **fh3PtTrueEtaDeltaR;    //! 
 
   TH3F      **fh3PtTrueEtaDeltaM;    //!
   TH3F      **fh3PtTrueEtaDeltaMRel; //!
   TH3F      **fh3PtTrueEtaScaleM;    //!
   TH3F      **fh3PtTrueMTrueScaleM;  //!

   ClassDef(anaJetEnergyScale,4)
};
#endif
