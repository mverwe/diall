// task to extract jet tree
// Author: M. Verweij

#ifndef anaCreateJetTree_h
#define anaCreateJetTree_h

#include "TString.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TTree.h"

#include "UserCode/diall/interface/anaBaseTask.h"
#include "UserCode/diall/interface/lwJetContainer.h"
#include <UserCode/diall/interface/rhoMap.h>

#include <UserCode/diall/interface/ForestJets.h>

class anaCreateJetTree : public anaBaseTask {
   
public:
   anaCreateJetTree() {;}
   anaCreateJetTree(const char *name, const char *title);
   virtual ~anaCreateJetTree() {;}
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
   void SetMinJetPtRef(double pt)      { fMinJetPtRef = pt ; }

   void SetStoreSubjets(bool b)        { fStoreSubjets = b; }

   void ClearOutJetTreeVars();
   
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
   double            fMinJetPtRef;            //min jet pt for ref jets

   bool              fStoreSubjets;           //store subjets in tree

   TTree             *fTreeOut;

   struct outJetTreeVars {
     int                fRun;
     float              fCent;
     std::vector<float> fPt;
     std::vector<float> fPtRaw;
     std::vector<float> fEta;
     std::vector<float> fPhi;
     std::vector<float> fM;
     std::vector<float> fPtRef;
     std::vector<float> fEtaRef;
     std::vector<std::vector<float> > fSubJetPt;
     std::vector<std::vector<float> > fSubJetEta;
     std::vector<std::vector<float> > fSubJetPhi;
     std::vector<std::vector<float> > fSubJetM;
     std::vector<std::vector<float> > fSubJetPtRef;
     std::vector<std::vector<float> > fSubJetEtaRef;
     std::vector<std::vector<float> > fSubJetPhiRef;
     std::vector<std::vector<float> > fSubJetMRef;
   };

   outJetTreeVars     fOutJetTreeVars;
   //   ForestJets         fForestJetsOut;         //jet forest type tree output

   ClassDef(anaCreateJetTree,1)
};
#endif
